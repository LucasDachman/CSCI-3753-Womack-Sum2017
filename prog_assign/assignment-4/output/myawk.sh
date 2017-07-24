#! /bin/bash

cat table-data | awk '
{if ($1=="##########") {name=$2;}} 
{if ($1=="Number") {num=$4}} 
{if ($1=="System") {iosys= $3; cpusys= $4; msys=$5}}
{if ($1=="User")  
	{iototal=iosys+$3; cputotal=cpusys+$4; mtotal=msys+$5;
	print name;
	printf "totals: %f %f %f\n", iototal, cputotal, mtotal;
	printf "avg: %f %f %f\n", iototal/num, cputotal/num, mtotal/num; 
} }
'


