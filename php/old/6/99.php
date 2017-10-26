<?php
	header("content-type=html/text;charset=utf-8");

	echo "<table boder='5px' width='500px'>";
	for($i = 1;$i <=9 ;$i++)
	{
		if($i%2 ==0)
			echo "<tr bgcolor='green'>";
		else
			echo "<tr bgcolor=#0x11ffaa>";
		for($j = 1;$j <= $i;$j++)
		{
			echo "<td>{$i}x{$j}=".$i*$j."</td>";
		}
		echo "</tr>";
	}
	echo "</table>";
?>