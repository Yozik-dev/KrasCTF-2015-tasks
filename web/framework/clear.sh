ls /var/www/controllers/ | grep -v "SiteController.php" | xargs rm
ls /var/www/models/ | grep -v "data.ini" | grep -v "LoginForm.php" | grep -v "NoteForm.php" | grep -v "Page.php" | grep -v "SuperPuperUserPyser.php" | xargs rm
ls /var/www/views/ | grep -v "site" | grep -v "layouts" | xargs rm -rf
mysql -u root yii_kctf_framework -prooTpasSLoL -e "TRUNCATE data_comments;"
