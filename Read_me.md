# Implementaion of RRT algorithm in Python

RRT.py - contains the driver program for executing the code. Starting node, Destination node are initialised here. And to simulate and visualise the working, map is created using pygame libraries with obstacles being randomly distributed in the map. Thus parameter defining map dimensions, obstacles dimension, number of obstacles are also initalised in the RRT.py file.

RRTBase.py - is to be imported while running RRT.py and it contains the function to create tree that connects between source to destination and draw to draw tree in the map from source to destination. 

Note - Download both the python files to the same directory and execute.

And the following reference is been used to implement RRT algorithm in python - https://www.youtube.com/watch?v=TzfNzqjJ2VQ. And an additional update is done on the reference code in  step() function in the RRTGraph class of RRTBase.py file to avoid index out of range exception. Done by checking the clearance of path from nearest node to destination before adding the destination node to tree.
