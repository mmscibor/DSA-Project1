DSA-Project1
============

Summary
-------

This project is a self-made implementation of Stack and Queue objects in C++. The objects, created within the code, 
are stored within an `std::list` and indexed accordingly, so that elements could be readily pushed and popped from
the various Stack and Queue objects.

Functionality
-------------

All of the necessary code is contained within `StacksAndQueues.cpp`. Running the program prompts you for two
files: the first is a `commands.txt` file. This file contains instructions which are to be parsed by the program,
which will tell the program to CREATE (a Stack or Queue), or PUSH or POP a value. All processes performed by the program
are to be output to an `output.txt` file. There is an expected output file: the goal of the assignment is to achieve
an output identical to the provided output.

Implementation
--------------

In order to implement these objects, an abstract class called `SimpleList` was created. This object is a reduced
linked list which contains only the necessary functionality to implement either a Stack or Queue, i.e. only
removing from the front or end of the list and only adding to the end of the list.
