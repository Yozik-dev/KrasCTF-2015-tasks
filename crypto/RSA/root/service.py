#!/usr/bin/env python # -*- coding: utf-8 -*-
# all the imports
import sqlite3
import json
import base64
from flask import Flask, request, session, g, redirect, url_for, \
     abort, render_template, flash, jsonify
from flask.ext.login import LoginManager, login_user , logout_user , current_user , login_required
from flask.ext.sqlalchemy import SQLAlchemy
from models import Users, Items

DEBUG = False
SECRET_KEY = 'development key'
USERNAME = 'admin'
PASSWORD = 'default'
SQLALCHEMY_DATABASE_URI = 'sqlite:///db/main.db'


app = Flask(__name__)
app.config.from_object(__name__)

login_manager = LoginManager()
login_manager.init_app(app)

db = SQLAlchemy(app)

@app.before_first_request
def init_request():
    db.create_all()

@app.before_request
def before_request():
    g.user = current_user

@login_manager.user_loader
def user_loader(user_id):
    user = Users.query.filter_by(id=user_id)
    if user.count() == 1:
        return user.one()
    return None

def sign_item(idcost):
    n = 0x9708fd22957518618db424d5ddb66d1127b7dffe38dddbbf631a31b9088d88383503ca23b06d600b56a46a1b0bd820d3e6ad76d1df57435949ef0c23c6ebeac9
    e = 0x10001
    d = 0x4a6157e077404270908b9df4610add2daca4c63ff99b710410f72e73947db70e4d82d0cb90ccf47d527169583ba49704db3977834ad205f9de910d9cc5f6df81
    return pow(idcost, d, n)

@app.route("/")
def index():
    return render_template("index.html")

@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'GET':
        return render_template('register.html')
    elif request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        user = Users.query.filter_by(login=username)
        if user.count() > 0:
            flash('User with such login is already registered')
            return redirect(url_for('register'))

        user = Users(username, password)
        db.session.add(user)
        db.session.commit()
        flash('User successfully registered')
        return redirect(url_for('login'))
    else:
        abort(405)

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    elif request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        user = Users.query.filter_by(login=username).filter_by(password=password)
        if user.count() == 1:
            login_user(user.one())
            flash('Welcome back {0}'.format(username))
            return redirect(url_for('index'))
        else:
            flash('Invalid login')
            return redirect(url_for('login'))
    else:
        return abort(405)

@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('index'))

@app.route("/market", defaults={'page': 1})
@app.route('/market/<int:page>')
@login_required
def market(page=1):
    return render_template("market.html",
            items = Items.query.paginate(page, 100, False).items,
            page = page
        )

@app.route('/buy/<int:item>')
@login_required
def buy(item):
    item_object = Items.query.filter_by(id=item).one()
    if item_object:
        if int(item_object.cost) <= int(g.user.cash):
            g.user.cash = int(g.user.cash) - int(item_object.cost)
            flash("You bought: " + item_object.title)
            flash("Get you item: " + base64.b64encode(json.dumps({"id": item_object.id, "cost": item_object.cost, "title": item_object.title, "sign_token": item_object.sign})))
            db.session.merge(g.user)
            db.session.commit()
        else:
            flash("Not enough money")
    else:
        flash("No such item")

    return render_template("market.html",
            items = Items.query.paginate(1, 100, False).items,
            page = 1
        )

@app.route("/boundary", methods=['GET','POST'])
@login_required
def boundary():
    if request.method == "GET":
        return render_template("boundary.html")
    elif request.method == "POST":
        key = Items.query.filter_by(title="KEY ELEMENT FOR GOING ABROAD").one().sign
        sign_token = request.form['sign_token']

        if sign_token == key:
            return jsonify(answer=render_template("boundary_answer.html", alert="alert-success", message="Get your flag!"))
        else:
            return jsonify(answer=render_template("boundary_answer.html", alert="alert-danger", message="Invalid sign"), 
                    public_key=open("static/rsa_1024_pub.pem").read()
                )
    else:
        abort(405)

if __name__ == '__main__':
    app.run()