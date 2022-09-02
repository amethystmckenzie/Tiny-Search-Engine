#!/bin/bash
#
# testing.sh - tests querier
#
# usage: ./testing.sh or simply: make test 
#
# Amethyst McKenzie, May 18 2022, cs50

dir=/thayerfs/courses/22spring/cosc050/cs50tse/tse-output

# TEST 0-2: Invalid Command-Line Arguments
echo "TEST 0: Invalid Number of Arguments"
./querier $dir/letters-depth-6 $dir/letters-index-6 1000

echo "TEST 1: Non-Crawler Directory"
./querier data $dir/letters-index-6

echo "TEST 2: Invalid File"
./querier ../crawler/letters-6 invalid_file

echo "-----------------------------------------------"
# TEST 3-8: Invalid Inputs
echo "TEST 3-4: Starting with an operator"
echo "and playground" | ./querier $dir/letters-depth-6 $dir/letters-index-6 
echo "or playground" | ./querier $dir/letters-depth-6 $dir/letters-index-6 

echo "TEST 5-6: Ending with an operator"
echo "playground and" | ./querier $dir/letters-depth-6 $dir/letters-index-6 
echo "playground or" | ./querier $dir/letters-depth-6 $dir/letters-index-6 

echo "TEST 7: Operators next to each other"
echo "playground and or page" | ./querier $dir/letters-depth-6 $dir/letters-index-6 

echo "TEST 8: An invalid character"
echo "an invalid-character" | ./querier $dir/letters-depth-6 $dir/letters-index-6 

# TEST 9: No Documents Match
echo "TEST 9: No Documents Match"
echo "notaword" | ./querier $dir/letters-depth-6 $dir/letters-index-6 

# TEST 10-12: fuzzquery testing
echo "TEST 10: fuzzquery letters at depth 6"
./fuzzquery $dir/letters-index-6 100 1 | ./querier $dir/letters-depth-6 $dir/letters-index-6 

echo "TEST 11: fuzzquery toscrape at depth 2"
./fuzzquery $dir/toscrape-index-2 30 1 | ./querier $dir/toscrape-depth-2 $dir/toscrape-index-2

echo "TEST 12: fuzzquery wikipedia at depth 1"
./fuzzquery $dir/wikipedia-index-1 30 1 | ./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1

