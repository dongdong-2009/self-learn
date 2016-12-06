<?php 
	//echo 输出一个布尔类型，true:1，false：空
	echo "<h5>布尔类型</h5>";
	$a = false;
	$b = true;
	echo "false:".$a."<br>";
	echo "true:".$b."<br>"; 
	var_dump($a);

	//数组
	echo "<h5>数组类型</h5>";
	$arr = array('穆昆空'=>"25",'张文超'=>"26","蔻蔻"=>"24");
	echo "<pre>";
	//var_dump($arr);
	//自动作为数组最后一个元素
	$arr[娜娜] = "22";
	print_r($arr);
	echo "</pre>";
?>