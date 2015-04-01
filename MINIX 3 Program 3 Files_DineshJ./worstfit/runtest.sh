#!/bin/sh
#WORST_FIT test
cc -o test50 test50.c
cc -o test51 test51.c
cc -o test52 test52.c 

#NAME
#chmem - change memory allocation

#SYNOPSIS
#chmem [+] [-] [=] amount file

#EXAMPLE
#chmem =50000 a.out Give a.out 50K stack space

chmem =50000 test50
chmem =40000 test51

echo "----------10 second sleep---------------"

sleep 10 

echo "----------WORST FIT RUNNING-------------"
./test50 1500 &
./test51 &


echo "----------ALERT killing test50----------"
echo "----------20 second sleep---------------"

sleep 20 

kill $(ps | grep "test50" | awk '{ print $1 }')
echo "----------ALERT test50 killed-----------"


echo "starting another test with less hole size of 30000"
echo "----------20 second sleep---------------"
sleep 20 
chmem =30000 test52
./test52 1500 &


