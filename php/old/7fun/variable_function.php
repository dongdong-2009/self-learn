<?php
	header("content-type:text/html;charset=utf-8");

	//变参函数
	function fun(){
		//echo func_num_args();
		//echo "<pre>";
		//print_r(func_get_args());
		//echo "</pre>";
		//return array_sum(func_get_args());
		$tot = func_num_args();
		$arr = func_get_args();

		for($i = 0;$i < $tot;$i++)
		{
			$sum+=$arr[$i];
		}
		return $sum;
	}

	echo fun("1","7","1")."<br><hr>";

	//变量函数
	function sum($i,$j){
		return $i+$j;
	}

	$a = sum;
	echo $a(1,2);

	//引用参数
	$num = 1;
	function test(&$i){
		$i = 10;
	}
	
	test($num);
	echo "<br><hr>引用参数<br>num=".$num;

	//回调函数
	echo "<br><hr>回调函数:<br>";
	function sum1($i,$j){
		return $i+$j;
	}

	function show($i,$j,$k){
		echo $k($i,$j);
	}

	show(1,2,sum1);

	echo "<br><hr>递归函数:<br>";
	$num1=5;
	function sum2($i){
		if($i == 1)
		{
			return $i;
		}
		return $i*sum2($i-1);
	}

	echo "{$num1}的阶乘：".sum2($num1);
?>







