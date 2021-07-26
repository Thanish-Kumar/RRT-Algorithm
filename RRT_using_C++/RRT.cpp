#include <iostream>
#include "RRTbase.h"

int main()
{
    float start_x = 10; 
    float start_y = 10;
    
    float goal_x = 510;
    float goal_y = 510;

    int mapW = 1200, mapH = 800;
    int obsDim = 30, obsNum = 0;

    std::cout<<"Enter number of obstacles"<<std::endl;
    std::cin>>obsNum;

    std::vector<std::tuple<int, int>> obs_loc;
    for (size_t i = 0; i < obsNum; i++)
    {
        int obs_x, obs_y;
        std::cout<<"Enter postion for the obstacle "<<i+1<<", in the format X Y"<<std::endl; 
        std::cin>>obs_x;
        std::cin>>obs_y;
        obs_loc.push_back(std::make_tuple(obs_x, obs_y));
    }
    
    /* 
    Reference obstacle position for the result shown with 5 obstacles,
    [(17, 12), (52, 58), (470, 470), (500, 470), (520, 520)]
    */

    RRTMap map(start_x, start_y, goal_x, goal_y, mapW, mapH, obsDim, obsNum, obs_loc);
    RRTGraph graph(start_x, start_y, goal_x, goal_y, mapW, mapH, obsDim, obsNum, obs_loc);
    sf::RenderWindow window(sf::VideoMode(mapW, mapH), "RRT");

    std::vector<sf::RectangleShape> obstacles = graph.makeObs();

    int iteration = 0;

    while (window.isOpen())
    {
        map.drawMap(obstacles, window);

        if (iteration % 10 == 0)
        {
            //std::cout<<"To bias"<<std::endl;
            graph.bias(window);
            sf::CircleShape node(4);
            node.setOrigin(node.getRadius(),node.getRadius());
            node.setPosition(sf::Vector2f(graph.x.back(),graph.y.back()));
            node.setFillColor(sf::Color::Blue);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(graph.x.back(), graph.y.back())),
                sf::Vertex(sf::Vector2f(graph.x[graph.parent.back()], graph.y[graph.parent.back()]))
            };
            window.draw(node);
            window.draw(line, 2, sf::Lines);
        }

        else
        {
            //std::cout<<"To expand"<<std::endl;
            graph.expand(window);
            sf::CircleShape node(4);
            node.setOrigin(node.getRadius(),node.getRadius());
            node.setPosition(sf::Vector2f(graph.x.back(), graph.y.back())); 
            node.setFillColor(sf::Color::Blue);
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(graph.x.back(), graph.y.back())),
                sf::Vertex(sf::Vector2f(graph.x[graph.parent.back()], graph.y[graph.parent.back()]))
            };
            window.draw(node);
            window.draw(line, 2, sf::Lines);
        }

        if (graph.goalFlag)
        {
            graph.extract_path();
            std::vector<sf::CircleShape> path_to_goal;
            path_to_goal.resize(graph.path.size());
            int i;

            for(auto& i : graph.path)
            {
                sf::CircleShape node(4);
                node.setOrigin(node.getRadius(),node.getRadius());
                node.setPosition(sf::Vector2f(graph.x[i],graph.y[i]));
                node.setFillColor(sf::Color::Red);
                path_to_goal.push_back(node);
            }

            map.drawPath(path_to_goal, window);

        }
         
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
            	window.close();
            	return 0; exit(0);
            }
        }
        
        window.display();
		sf::sleep(sf::seconds(0.05f));
        //window.clear();
        //window.draw(rectang);
        iteration++;
    }
    return 0;
}

