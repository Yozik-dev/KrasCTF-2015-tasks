from flask.ext.login import UserMixin
from flask.ext.sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Users(db.Model, UserMixin):
    __tablename__ = "users"
    id = db.Column(db.Integer, primary_key=True)
    login = db.Column(db.String, unique=True, index=True)
    password = db.Column(db.String)
    cash = db.Column(db.Integer)

    def __init__(self, login, password):
        self.login = login
        self.password = password
        self.cash = 10000

class Items(db.Model):
    __tablename__ = 'items'
    id = db.Column('id', db.Integer, primary_key=True)
    cost = db.Column('cost', db.String)
    title = db.Column('title', db.String)
    sign = db.Column('sign', db.String)
