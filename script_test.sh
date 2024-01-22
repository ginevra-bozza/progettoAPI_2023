#!/bin/bash

for i in {1..111}; do
    # Run the command and redirect input/output
    ./main < "open_$i.txt" > "output_$i.txt"
    
    # Compare the output with the expected output
    if diff "output_$i.txt" "open_$i.output.txt" > /dev/null; then
        echo "Test $i passed"
    else
        echo "Test $i failed. Differences:"
        diff "output_$i.txt" "open_$i.output.txt"
    fi
done

