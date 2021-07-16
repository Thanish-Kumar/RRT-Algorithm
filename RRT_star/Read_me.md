# Implementaion of RRT algorithm in Python

RRT_star.py - contains the driver program for executing the code. Starting node, Destination node, obstacles dimensions, number of obstacles and obstacle location are initialised here. 

RRTBase_star.py - is to be imported while running RRT.py and it contains the function to create tree that connects between source to destination and draw to draw tree in the map from source to destination. 

Note - Download both the python files to the same directory and execute.

/*******************************************************************************************************************************/

To simulate and visualise the working, map is created using pygame libraries with obstacles being randomly distributed in the map

And the following reference is been used to implement RRT algorithm in python - https://www.youtube.com/watch?v=TzfNzqjJ2VQ. And additional changes were implemented on the code to implement RRT*.
