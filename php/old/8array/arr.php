<?php
	header("content-type:text/html;charset=utf-8");
	//索引数组
	/*
	$arr = array(1,2,3);
	echo "索引数组:<br><pre>";
	print_r($arr);
	echo "</pre>";

	//关联数组
	$arr1 = array("name"=>"mkk",2,"age"=>"30",4,100=>5,6,7);
	echo "<br><hr>关联数组：<br><pre>";
	print_r($arr1);
	echo "</pre>";

	//数组赋值的方式定义数组
	$arr3[1] = 1;
	$arr3[] = 2;
	$arr3['age'] = 30;
	echo "<br><hr>数组赋值定义数组:<pre>";
	print_r($arr3);
	echo "</pre>";
	*/

	//数组的遍历
	/*
	$arr[]="mkk";
	$arr[]="koukou";
	$arr[]="qiezi";
	$arr[]="nana";
	echo "<h1 style='color:red'><center>数组索引遍历</center></h1>"."<br>";
	for($i = 0;$i < 4;$i++){
		if($i%2 == 0)
			echo "<h4 style='color:green'>第".($i+1)."个人的名字：{$arr[$i]}</h4>";
		else
			echo "<h4 style='color:yellow'>第".($i+1)."个人的名字：{$arr[$i]}</h4>";
	}
	echo "<hr>";
	
	//foreach遍历数组
	$arr['name'] = "mkk";
	$arr['age'] = 25;
	$arr['sex'] = "男";
	$arr[] = "abc";
	echo "<h1 style='color:red'><center>数组遍历关联数组</center></h1>"."<br>";
	foreach($arr as $key=>$var){
		echo "<h3>{$key}:{$var}</h3>";
	}
	echo "<hr>";
	*/
	$arr['name'] = "mkk";
	$arr['age'] = 30;

	/*
	$arr1 = each($arr);
	echo "<pre>";
	print_r($arr1);
	echo "</pre>";

	$arr2 = each($arr);
	echo "<hr><pre>";
	print_r($arr2);
	echo "</pre>";*/
	/*while($row = each($arr)){
		echo "<pre>";
		print_r($row);
		echo "</pre><hr>";
	}*/
	while(list($key,$var) = each($arr)){
		echo $key.":".$var;
		echo "<hr>";
	}
?>







