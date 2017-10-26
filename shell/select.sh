#!/bin/bash

echo "Please choice os:"

select os in "unix" "windows" "linux" "bsd"; do
	break;
done

echo "Your os is ${os}"
