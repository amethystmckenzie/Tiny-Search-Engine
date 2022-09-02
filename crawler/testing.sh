#!/bin/bash
#
# testing.sh - tests crawler
#
# usage: ./testing.sh or simply: make test 
#
# Amethyst McKenzie, April 29 2022, cs50


# TEST 0: Invalid Arguments 
echo "TEST 0: Invalid Number of Arguments"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 1 99

# TEST 1: Invalid maxDepth (not an integer)
echo "TEST 1: Invalid maxDepth (not an integer)"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 1xx

# TEST 2: Invalid maxDepth (out of range)
echo "TEST 2: Invalid maxDepth (out of range)"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 20

# TEST 3: Invalid Directory
echo "TEST 3: Invalid Directory"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html invalidDirectory 0

# TEST 4: Non-Internal URL
echo "TEST 4: Non-Internal URL"
./crawler https://www.wikipedia.org data 1 

# TEST 5: Non-Existent Server
echo "TEST 5: Non-Existent Server"
./crawler http://cs50t.dartmouth.edu/tse/letters/index.html data 1

# TEST 6: Valid Server, Non-Existent Page
echo "TEST 6: Valid Server, Non-Existent Page"
./crawler http://cs50tse.cs.dartmouth.edu/tse/amethyst/index.html data 0

# TEST 7: Crawl A Simple, Closed Set of Cross-Linked Web Pages
echo "TEST 7: Crawl A Simple, Closed Set of Cross-Linked Web Pages"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 2

# TEST 8: Crawl letters/A with Depth 0
echo "TEST 8: Crawl letters/A with Depth 0"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data 0

# TEST 9: Crawl letters/A with Depth 1
echo "TEST 9: Crawl letters/A with Depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data 1

# TEST 10: Crawl letters/A with Depth 2
echo "TEST 10: Crawl letters/A with Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data 2

# TEST 11: Crawl letters/A with Depth 3
echo "TEST 11: Crawl letters/A with Depth 3"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data 3

# TEST 12: Crawl letters/A with Depth 4
echo "TEST 12: Crawl letters/A with Depth 4"./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html 
data 4

# TEST 13: Crawl letters/A with Depth 5
echo "TEST 13: Crawl letters/A with Depth 5"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html data 5

# TEST 14: Crawl letters/C with Depth 0
echo "TEST 14: Crawl letters/C with Depth 0"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 0

# TEST 15: Crawl letters/C with Depth 1
echo "TEST 15: Crawl letters/C with Depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 1

# TEST 16: Crawl letters/C with Depth 2
echo "TEST 16: Crawl letters/C with Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 2

# TEST 17: Crawl letters/C with Depth 3
echo "TEST 17: Crawl letters/C with Depth 3"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 3

# TEST 18: Crawl letters/C with Depth 4
echo "TEST 18: Crawl letters/C with Depth 4"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 4

# TEST 19: Crawl letters/C with Depth 5
echo "TEST 19: Crawl letters/C with Depth 5"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/C.html data 5

# TEST 20: Crawl Wikipedia with Depth 0
echo "TEST 20: Crawl Wikipedia with Depth 0"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html directory1 0

# TEST 21: Crawl Wikipedia with Depth 1
echo "TEST 21: Crawl Wikipedia with Depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html directory1 1

# TEST 22: Crawl Wikipedia with Depth 2
echo "TEST 22: Crawl Wikipedia with Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html directory1 2
