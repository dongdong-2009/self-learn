<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>日期测试</title>
</head>
<body>
<?php 
	$y = date('Y');
	if($y < 2000){
		echo "<h1>小于2000年，目前是{$y}</h1>";
	}else{
		echo "<h1>大于2000年</h1>";
	}
 ?>
</body>
</html>