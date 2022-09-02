#!/bin/bash
#
# testing.sh - tests indexer
#
# usage: ./testing.sh or simply: make test 
#
# Amethyst McKenzie, May 9 2022, cs50

# crawler path
crawler=../crawler

# TEST 0-2: Invalid Tests
echo "TEST 0: Invalid Number of Arguments"
./indexer

echo "TEST 1: Non-Crawler Directory"
./indexer randomdirectory file

echo "TEST 2: Invalid File"
touch invalid
chmod -w invalid
./indexer $crawler/letters-3 invalid

# TEST 3-5: Indexer Functionality Testing

echo "TEST 3: Index letters at depth 3"
./indexer $crawler/letters-3 indexer-l3

echo "sorting files..."
sort indexer-l3 > sorted-indexer-l3
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-3 > ORIGINAL-index-l3

echo "comparing files..."
diff -q sorted-indexer-l3 ORIGINAL-index-l3

#####

echo "TEST 4: Index toscrape at depth 1"
./indexer $crawler/toscrape-1 indexer-ts1

echo "sorting files..."
sort indexer-ts1 > sorted-indexer-ts1
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1 > ORIGINAL-index-ts1

echo "comparing files..."
diff -q sorted-indexer-ts1 ORIGINAL-index-ts1

#####

echo "TEST 5: Index wikipedia at depth 1"
./indexer $crawler/wikipedia-1 indexer-w1

echo "sorting files..."
sort indexer-w1  > sorted-indexer-w1
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-1 > ORIGINAL-index-w1 

echo "comparing files..."
diff -q sorted-indexer-w1 ORIGINAL-index-w1

# TEST 6-8: Indextest Functionality Testing
echo "TEST 6: Indextest letters-3"
./indextest sorted-indexer-l3 indextest-l3

echo "sorting files..."
sort indextest-l3 > sorted-indextest-l3

echo "comparing files..."
diff -q sorted-indexer-l3 sorted-indextest-l3

echo "TEST 7: Indextest toscrape-1"
./indextest sorted-indexer-ts1 indextest-ts1

echo "sorting files..."
sort indextest-ts1 > sorted-indextest-ts1

echo "comparing files..."
diff -q sorted-indexer-ts1 sorted-indextest-ts1

echo "TEST 8: Indextest wikipedia-1"
./indextest sorted-indexer-w1 indextest-w1

echo "sorting files..."
sort indextest-w1  > sorted-indextest-w1

echo "comparing files..."
diff -q sorted-indexer-w1 sorted-indextest-w1