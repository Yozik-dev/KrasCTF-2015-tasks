<?php

$right_password = 'f0r_the_$ta1in!';
$right_access_card = 'MK139K_XE686C';
$flag = 'its_not_easy_to_be_an_agent_of_kgb!';


//setcookie("access_card","MK139K_XE686C",time()+999999);

if (isset($_POST['pass']) && isset($_COOKIE['access_card']) && 
	$_POST['pass'] == $right_password && $_COOKIE['access_card'] == $right_access_card)
{
	echo 'okay, comrad! take a flag: ' . $flag;
	exit;
}
?>

<form method="POST">
	<input type="password" name="pass" placeholder="password">
	<button type="submit">Go</button>
</form>
