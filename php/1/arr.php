<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>var_dump输出测试</title>
</head>
<body>
<?php 
	$arr = array('a'=>"xiaokun",'b'=>"xiaoxu" );
	echo "<pre>";
	echo "<h3>var_dump:</h3>";
	var_dump($arr);
	echo "<h3>print_r:</h3>";
	print_r($arr);
	echo "</pre>";
 ?>
</body>
</html>
