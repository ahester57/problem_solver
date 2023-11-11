
# Project 2: Improvements

First, I set up a better programming environment than the command line. Vim is cool, but doing a whole project in Vim was a drag. I say that as I am writing this README in Vim.

## Usage

Build with `make`.

Run with `./P2 <inputFileName.txt>`.

```
  Usage:
    P2 inputFileName.txt | --test  (input file contains list of proposed articles)

```

## Data Structure

How was the data structure improved?


## Algorithm

How was the algorithm improved?

## Architecture

In addition to time and space complexity improvements, I have also improved the code's structure in a few ways.

* Split source and headers into `src/` and `/inc`, respectively.
* New source file `process_news.cpp` takes the load off of `main()` for:
  * File Parsing
  * Solution Scoring

## Tests

Add a testing suite to aid in developing improvements.

* Confirm correct # of articles loaded.
* Confirm score is optimal for the example.

Run test with `$ ./P2 --test`.

Here's an example of a failed test which is simulated by altering the expected optimum to an incorrect value for ex14.txt.

```
$ ./P2 --test
Timer started.
        inputFileName='examples/ex1.txt' PASSED
                Expected 551 to equal 551
        inputFileName='examples/ex2.txt' PASSED
                Expected 580 to equal 580
        inputFileName='examples/ex3.txt' PASSED
                Expected 1028 to equal 1028
        inputFileName='examples/ex4.txt' PASSED
                Expected 870 to equal 870
        inputFileName='examples/ex5.txt' PASSED
                Expected 1478 to equal 1478
        inputFileName='examples/ex6.txt' PASSED
                Expected 1431 to equal 1431
        inputFileName='examples/ex7.txt' PASSED
                Expected 1107 to equal 1107
        inputFileName='examples/ex8.txt' PASSED
                Expected 1069 to equal 1069
        inputFileName='examples/ex9.txt' PASSED
                Expected 1007 to equal 1007
        inputFileName='examples/ex10.txt' PASSED
                Expected 337 to equal 337
_test_files failed on line 100: actualArticleCount == news.numArticles failed
        For inputFileName='examples/ex14.txt'

Timer stopped.
Computation Time: 0 hours, 0 minutes, 17 seconds.
```

----

## Aggregate Analysis

### Time Complexity

What is the time complexity?

#### Before


#### After

### Space Complexity

#### Before


#### After

----

#### Austin Hester - ahester57

