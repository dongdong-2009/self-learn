<?php 
	//测试变量是存在
	//没有定义、设置为null
	$name = null;
	echo "变量存在与否测试（isset()）：";
	var_dump(isset($name));
	echo "<br>";
	echo "<br>";

	//测试变量是否为空
	//false、null、没有定义、空字符串、字符串中是0、整数0、空数组
	//$name = array();
	// $name = 0;
	// $name = "0";
	// $name = null;
	$name = "";
	echo "变量是否为空测试(empty()):";
	var_dump(empty($name));
	echo "<br>";
	echo "<br>";

	//是否为数组
	$arr = array("kun");
	echo "是否为数组测试(is_array()):";
	var_dump(is_array($arr));
	echo "<br>";
	echo "<br>";

	//语言结构测试
	//var_dump(is_callable("echo"));
	//var_dump(is_callable("print"));
	//var_dump(is_callable("list"));
	//var_dump(is_callable("array"));
	var_dump(is_callable("var_dump"));	//函数
?>