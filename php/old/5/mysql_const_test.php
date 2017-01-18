<?php
	include("inc.php");

	header("content-type:text/html;charset=utf-8");
	//mysql数据库密码：mkk8601352
	//conn代表连接数据库成功之后的资源变量 
	//1、获取资源
	$conn = mysql_connect(HOST,USER,PASS);
	if($conn == null){
		echo "连接mysql数据失败";
	}else{
		echo "连接数据成功";
	}
	//var_dump($conn);
	
	//mysql_query("查看数据库",$conn);

	//mysql_info($conn);
	
	//2、连接数据库
	mysql_select_db(DBNAME);

	//3、执行SQL语句
	$sql = "select * from user";
	$rst = mysql_query($sql);
	while($row = mysql_fetch_assoc($rst)){
		echo "<h1>ID:{$row['id']}</h1>";
		echo "<h1>Name:{$row['name']}</h1>";
	}

	//关闭数据库
	mysql_close($conn);
?>