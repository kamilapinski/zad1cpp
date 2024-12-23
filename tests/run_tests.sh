#!/bin/bash

for i in {1..6}
do
    echo "Running test $i"
    ../medals < test_$i.in >temp.out 2>temp.err

    if diff -q test_$i.out temp.out && diff -q test_$i.err temp.err; then
        echo "Test $i passed"
    else
        echo "Test $i failed"
    fi
done;

rm temp.out temp.err