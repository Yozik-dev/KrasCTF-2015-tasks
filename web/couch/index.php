<?php 
include "HttpGetter.php";

if (isset($_POST['url'])) { 
	$link = $_POST['url']; 
	$data = HttpGetter::Get($link);
	
	$db = HttpGetter::Post('http://127.0.0.1:5984/kctf', [ 
		'ip' => [
			'REMOTE_ADDR' => isset($_SERVER['REMOTE_ADDR']) ? 
				$_SERVER['REMOTE_ADDR'] : '',
			'HTTP_USER_AGENT' => isset($_SERVER['HTTP_USER_AGENT']) ? 
				$_SERVER['HTTP_USER_AGENT'] : '',
			'HTTP_COOKIE' => isset($_SERVER['HTTP_COOKIE']) ? 
				$_SERVER['HTTP_COOKIE'] : ''
		],
		'date' => time(),
		'link' => $link
	]);
} 
?>
<!DOCTYPE html>
<html lang="ru-Ru">
	<head>
		<meta charset="UTF-8">
		<meta property="og:description" content="Time to relax.">
		<title>Simple scaner</title>
		<link href="./style.css" rel="stylesheet">
	</head>

	<body>
		<div>
			<?php //echo isset($db)?$db:''; ?>
		</div>
		<form method="POST" action="./">

			<?php if (isset($_POST['url'])): ?>
				<input type="text" value="<?=$_POST['url']?>" name="url">
			<?php else: ?>
				<input type="text" name="url">
			<?php endif; ?>
			
			<input type="submit" value="Сканировать">
		</form>

		<?php if(isset($data)): ?>
			<div class="result">
			<?php if($data === false): ?>
					Введите валидный http адрес. Помните, все ваши действия логируются, не совершайте противоправных действий.
				<?php else: ?>
					<?=$data?>
				<?php endif; ?>
			</div>
		<?php endif; ?>

	</body>

</html>