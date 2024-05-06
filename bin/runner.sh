#!/bin/bash

make clean
make all

for i in {1..10}
do
  time=$((i * 1000))
  ./client/client execute ${i} -p "./bin/hello ${i}"
done

for i in {1..10}
do
  time=$((i * 1000))
  ./client/client execute ${i} -p "./bin/void ${i}"
done
