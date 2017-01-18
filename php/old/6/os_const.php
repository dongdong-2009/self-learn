<?php
	function fun(){
		echo "<br><hr>当前所在函数：";
		echo __FUNCTION__;
	}

	echo "PHP当前系统：".PHP_OS;
	echo "<br><hr>PHP当前版本：".PHP_VERSION;
	echo "<br><hr>当前所在行数（4）：".__LINE__;
	echo "<br><hr>当前文件名：".__FILE__;
	fun();
	echo "<br><hr>圆周率：".M_PI;
	
?>