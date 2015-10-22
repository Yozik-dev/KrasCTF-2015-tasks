<?php 

session_start();

unset($_SESSION['login']);
unset($_SESSION['referers']);

session_destroy();

header('Location: ./index.php');