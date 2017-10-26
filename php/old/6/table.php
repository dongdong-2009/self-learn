<?php
	header("content-type:text/html;charset=utf-8");
	
	$days=31;
	$j = 1;
	echo "<table  bgcolor='yellow' width='700px' border='10px'>";	
	for($i = 1;$i <= 31;){
		echo "<tr>";
		for($j = 1;$j <= 7;$j++){
			echo "<td>{$i}</td>";
			if($i++ >30)
				break;
		}	
		echo "</tr>";
	}
	echo "</table>";
?>