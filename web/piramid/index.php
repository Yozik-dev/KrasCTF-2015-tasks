<?php 
ini_set('display_errors', 1);
error_reporting(E_ALL);

include 'preload.php';


$stmt = $pdo->query('SELECT * FROM users GROUP BY lower(login) ORDER BY referers DESC LIMIT 20');
?>





<?php if(!isset($_SESSION['login'])): ?>
	<a href="./login.php">Вход</a><br>
	<a href="./register.php?referer=<?=$adminLogin;?>">Регистрация</a>
<?php else: ?>
	Ваш логин: <?=$_SESSION['login']?><br>
	Количество подписчиков: <?=$_SESSION['referers']?><br>
	
	<?php if($_SESSION['referers'] > 1000): ?>
		Ваш пароль к новым сервисам : <?php echo $thisIsFlag; ?>
	<?php else: ?>
		Новые функции будут доступны после набора 1000 подписчиков
	<?php endif; ?>

	<br><a href="./logout.php">Выход</a>
<?php endif; ?>

<h2>ТОП 20 пользователей по количеству подписчиков</h2>
<table>
	<tr>
		<td>Место</td>
		<td>ID</td>
		<td>Пользователь</td>
		<td>Количество подписчиков</td>
	</tr>
	<?php $i = 1; ?>
<?php foreach($stmt->fetchAll() as $user):?>
	<tr>
		<td><?=$i++?></td>
		<td><?=$user['id']?></td>
		<td><?=mb_strtolower($user['login'])?></td>
		<td><?=$user['referers']?></td>
	</tr>
<?php endforeach; ?>
</table>


