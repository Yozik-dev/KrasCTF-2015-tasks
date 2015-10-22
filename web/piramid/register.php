<?php 

include 'preload.php';
include 'functions.php';

function register(){
	global $pdo;
	
	if(!isset($_GET['login']) || !isset($_GET['password'])){
		$error = 'Введите логин и пароль';
		return ['error'=>$error];
	}
	
	if( !$_GET['login'] || !$_GET['password']){
		$error = 'Логин и пароль не должны быть пусты';
		return ['error'=>$error];
	}
	
	if(!isset($_GET['captcha']) || !$_GET['captcha']){
		$error = 'Введите капчу';
		return ['error'=>$error];
	}
	
	if($_GET['captcha'] != $_SESSION['captcha']){
		$error = 'Капча не совпадает';
		return ['error'=>$error];
	}
		
	
	$login = [];
	preg_match('/^[a-z0-9]+$/i', $_GET['login'], $login);
	if( !(count($login) == 1) ){
		$error = 'В логине допустимы только латинские маленькие буквы и цифры';
		return ['error'=>$error];
	}
	$login = $login[0];
	$stmt = $pdo->prepare('SELECT COUNT(*) as count FROM users WHERE login = ?');
	$stmt->execute([$login]);
	$count = $stmt->fetch()['count'];
	if($count != 0){
		$error = 'Такой пользователь уже есть';
		return ['error'=>$error];
	}
	$stmt = $pdo->prepare('
		INSERT INTO `users`(`login`, `password`) 
		VALUES (?,?)
	');
	$stmt->execute([$login, $_GET['password']]);
	if(isset($_COOKIE['referer']) && $_COOKIE['referer']){
		incrementReferer($_COOKIE['referer']);
		setcookie('referer', "");
	}

	return ['success'=>true];
}

if(isset($_GET['reg_ok'])){
	$result = register();
	if(isset($result) && isset($result['success']))
		header('Location: ./login.php?tag=success');
}

?>

<h2> Регистрация </h2>

<?php if(isset($result) && !isset($result['success'])) echo $result['error']; ?>

<script>
	function test_input(){
		login = document.getElementById('login-input').value;
		password = document.getElementById('password-input').value;
		
		if( !login || !password ){
			alert('Введите логин и пароль');
			return false;
		}
		if( !login.match(/^[a-z0-9]+$/) ){
			alert('В логине допустимы только латинские маленькие буквы и цифры');
			return false;
		}
		return true;
	}
</script>

<form action="./register.php" method="GET">

<table>
	<tr>
		<td>Login:</td>
		<td>
			<?php if(isset($_GET['login'])): ?>
			<input type="text" id="login-input" name="login" value="<?=$_GET['login']?>"><br>
			<?php else: ?>
			<input type="text" id="login-input" name="login"><br>
			<?php endif; ?>
		</td>
	</tr>
	<tr>
		<td>Password:</td>
		<td>
			<input id="password-input" type="password" name="password">
		</td>
	</tr>
	<tr>
		<td></td>
		<td><iframe src="./captcha/image.php" height="60px" width="120px"></iframe></td>
	</tr>
	<tr>
		<td>Captcha:</td>
		<td>
			<input type="text" name="captcha">
		</td>
	</tr>
	<tr>
		<td></td>
		<td><input type="submit" name="reg_ok" onclick="return test_input();" value="Отправить"></td>
	</tr>
</table>
</form>
