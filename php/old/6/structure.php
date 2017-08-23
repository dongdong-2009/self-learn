<?php
	header("content-type:text/html;charset=utf-8");

	echo "测试if语句：<br>";
	$name="mkk";
	$score=101;
	if($score >= 60){
		echo "<h1 style='color:green'>{$name}及格了！</h1>";
	}else{
		echo "<h1 style='color:red'>{$name}不及格!!!</h1><hr>";
	}

	echo "测试switch语句：<br><h2>得分：";
	switch($score/10){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			echo "不及格";
			break;
		case 6:
			echo "60~70";
			break;
		case 7:
			echo "70~80";
			break;
		case 8:
			echo "80~90";
			break;
		case 9:
			echo "90~100";
			break;
		case 10:
			echo "你太棒了";
			break;
		default:
			echo "得分异常";
	}
	echo "</h2><br><hr>";

	echo "测试for循环<br>";
	for($i = 0;$i < 3;$i++){
		echo "i={$i}<br>";
	}

	$j=1;
	echo "<hr>测试while循环<br>";
	while($j++ < 5){
		echo "hello<br>";
		//sleep(1);
	}
?>