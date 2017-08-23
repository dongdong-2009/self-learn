<?php 
	$name = "小坤哥";
	echo "<center>几种输出方式</center><br>";
	echo ("echo:{$name}<br>");
	print("print:{$name}<br>");
	printf("printf:%s<br>",$name);
	echo "var_dump:";
	var_dump($name);
	echo "<br>print_r:";
	print_r($name);
?>