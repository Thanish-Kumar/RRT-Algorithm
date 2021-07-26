# Implementaion of RRT algorithm in C++

RRT.cpp - contains the driver program for executing the code. Starting node, Destination node, obstacles dimensions, number of obstacles and obstacle location are initialised here. 

RRTbase.h - is to be imported while running RRT.py and it contains the function to create tree that connects between source to destination and draw to draw tree in the map from source to destination. 

Note - Download both the C++ files to the same directory and execute.

# Reqirements
C++11+ and SFML is required to run the codes.

SFML is used to simulate and visualise the working, map is created using pygame libraries with obstacles being randomly distributed in the map.

# How to Run
$ g++ -std=c++11 -c RRbase.h RRT.cpp 
$ g++ RRT.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
$ ./sfml-app

# Reference
https://www.youtube.com/watch?v=TzfNzqjJ2VQ - implementation of RRT in python  - 

