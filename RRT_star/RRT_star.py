import pygame
from RRTbase_star import RRTGraph
from RRTbase_star import RRTMap


def main():
    dimensions = (600, 1000)  # (height, width)
    start = (50, 50)
    goal = (510, 510)
    obsdim = 30
    obsnum = 5
    iteration = 0
    obs_location = [(17, 12), (52, 58), (470, 470), (500, 470), (520, 520)]  # (x, y), for (x < width) and (y < height)

    pygame.init()
    map_1 = RRTMap(start, goal, dimensions, obsdim, obsnum)
    graph = RRTGraph(start, goal, dimensions, obsdim, obsnum, obs_location, map_1)

    obstacles = graph.makeObs()
    map_1.drawMap(obstacles)

    while not graph.extract_path() or iteration < 4000:

        if graph.extract_path():
            map_1.drawPath(graph.path_to_goal())
            pygame.display.update()

        if iteration % 10 == 0:
            graph.bias(goal)

        else:
            graph.expand()

        if iteration % 5 == 0:
            pygame.display.update()

        iteration += 1

    map_1.map.fill((255, 255, 255))
    map_1.drawMap(obstacles)
    map_1.drawPath(graph.path_to_goal())
    pygame.display.update()
    pygame.event.clear()
    pygame.event.wait(0)


if __name__ == '__main__':
    main()
