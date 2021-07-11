# Implementaion of RRT algorithm in Python
Pygame library is used to visulalise the algorithm for path planning.

RRT.py - contains the driver program for executing the code. Starting node, Destination node are initialised here. And to simulate and visualise the working, map is created using pygame libraries with obstacles being randomly distributed in the map. Thus parameter defining map dimensions, obstacles dimension, number of obstacles are also initalised in the RRT.py file.

RRTBase.py - is to be imported while running RRT.py and it contains the function to create tree that connects between source to destination and draw to draw tree in the map from source to destination. 

Reference - https://www.youtube.com/watch?v=TzfNzqjJ2VQ

Additional update developed on the reference - step() function in the RRTGraph class of RRTBase.py file is updated to avoid index out of range exception. By checking the clearance of path from nearest node to destination before adding the goal node to tree.
