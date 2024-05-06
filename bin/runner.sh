#!/bin/bash

printf " === Cleaning and compiling the project === \n\n"
make -s clean > /dev/null 2>&1
make -s all > /dev/null 2>&1

# First Argument: number of tasks to run
if [ -z "$1" ]
then
  printf "Enter the number of tasks to run: "
  read number
else
  number=$1
fi

# Second Argument: order of the tasks (asc, desc, random)
if [ -z "$2" ]
then
  printf "Enter the order (asc, desc, random): "
  read order
else
  order=$2
fi

# Validate the second argument
if [ "$order" != "asc" ] && [ "$order" != "desc" ] && [ "$order" != "random" ]
then
  printf "Invalid order. Please enter 'asc', 'desc' or 'random'.\n"
  exit 1
fi

# Generate sequence based on the order
if [ "$order" = "asc" ]
then
  seq=$(seq 1 $number)
elif [ "$order" = "desc" ]
then
  seq=$(seq $number -1 1)
elif [ "$order" = "random" ]
then
  seq=$(shuf -i 1-$number)
fi

for i in $seq
do
  time=$((i * 1000))
  if (( $i % 2 == 1 ))
  then
    command="hello"
  else
    command="void"
  fi

  ./client/client execute ${i} -p ./bin/"${command}" ${i}
done