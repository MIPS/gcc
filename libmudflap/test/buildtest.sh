#!/bin/sh

cat mf-driver.c > $2
echo 'void test() {' >> $2
cat $1 >> $2
echo '}' >> $2
