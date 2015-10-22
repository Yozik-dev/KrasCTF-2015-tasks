<?php 

include 'preload.php';

function login(){
	global $pdo;
	
	if(!isset($_GET['login']) || !isset($_GET['password'])){
		$error = 'Введите логин и пароль';
		return ['error'=>$error];
	}
	
	if( !$_GET['login'] || !$_GET['password']){
		$error = 'Логин и пароль не должны быть пусты';
		return ['error'=>$error];
	}
	
	$stmt = $pdo->prepare('SELECT * FROM users WHERE login = ? AND password = ?');
	$stmt->execute([
		$_GET['login'], 
		$_GET['password']
	]);
	$user = $stmt->fetch();
	if(!$user){
		$error = 'Неверный логин или пароль';
		return ['error'=>$error];
	}
	
	$_SESSION['login'] = $user['login'];
	
	return ['success'=>true];
	
}


if(isset($_GET['log_ok'])){
	$result = login();
	if(isset($result) && isset($result['success']))
		header('Location: ./index.php');
}

?>

<h2> Вход </h2>
<a href="./index.php">Возврат на главную</a>
<br>
<br>
<?php if(isset($_GET['tag'])): ?>
<br>
<br>
Регистрация прошла успешно, войдите с регистрационными данными
<?php endif; ?>

<?php if(isset($result) && !isset($result['success'])) echo $result['error']; ?>

<form action="./login.php" method="GET">
<table>
	<tr>
		<td>Login:</td>
		<td>
			<?php if(isset($_GET['login'])): ?>
			<input type="text" name="login" value="<?=$_GET['login']?>"><br>
			<?php else: ?>
			<input type="text" name="login"><br>
			<?php endif; ?>
		</td>
	</tr>
	<tr>
		<td>Password:</td>
		<td>
			<input type="password" name="password">
		</td>
	</tr>
	<tr>
		<td></td>
		<td><input type="submit" name="log_ok" value="Отправить"></td>
	</tr>
</table>






</form>