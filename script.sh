#!/bin/bash

for i in $(seq 1 111); do
  echo ./main < open_$i.output.txt > output.txt
  echo diff ./open_$i.output.txt ./output.txt
done
