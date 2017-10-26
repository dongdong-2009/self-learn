<?php
	//echo "aaa";
	//header、cookie、session之前不能有任何输出
	header("content-type:text/html;charset=utf-8");
	//定义类
	class Person{
		function say(){
			echo "<h1><center>我正在说话</center></h1>";
		}

		function eat(){
			echo "我正在吃饭";
		}

		function sleep(){
			echo "我正在睡觉";
		}

		function run(){
			echo "我正在跑步";
		}
	}

	//创建对象
	$user1 = new Person();
	//var_dump($user1);
	$user1->say();
	//$user1->eat();
?>