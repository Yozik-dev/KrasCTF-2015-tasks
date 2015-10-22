<?php

include 'db.php';
include 'functions.php';

$adminLogin = "superstatos";

while(true){
	incrementReferer($adminLogin);
	usleep(rand(50000,100000));
}