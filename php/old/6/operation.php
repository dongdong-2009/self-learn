<?php
	echo "<h1>测试运算符</h1>";
	//@：屏蔽函数的报错
	$conn = @mysql_connect("localhost","root","mkk860135");
	echo "测试@：aaaaaa";

	//数组运算符:=>
	$arr = array("name"=>"user1",age=>"30");
	echo "<pre>测试数组运算符：<br>";
	print_r($arr);
	echo "</pre>";

	//对象运算符
	
	//测试三元运算符：（? :）
	$a = 2;
	$num = $a > 3 ? $a : 3;
	echo "<br><hr>测试三元运算符：".$num;
?>