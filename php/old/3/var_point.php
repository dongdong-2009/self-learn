<?php
	$a = 10;
	$b = &$a;	//变量引用
	$b = 20;
	echo "a = {$a}<br>b = {$b}";
?>