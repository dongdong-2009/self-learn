<?php
	//类型转换
	//1、自动换换
		//整型转字符串
	$num = 123;
	echo "整数num=123--->字符串：".$num."abc";

		//字符串转整型,abc123会转为0
	$str = "123";
	echo "<hr><br>字符串转整型：";
	echo $str+1;

		//整型转布尔类型
	echo "<hr><br>整型转布尔（num=0）：";
	$num = 0;
	if($num){
		echo "num=".$num."假";
	}else{
		echo "num=".$num."真";
	}
?>