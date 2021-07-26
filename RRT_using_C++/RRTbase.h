#ifndef RRTBASE_H
#define RRTBASE_H

#include <iostream>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <list>
#include <random>
#include <tuple>
#include <cmath>

struct node{
    int x;
    int y;
    node(int x, int y) 
    {
        x = x;
        y = y;
    }
};

struct dimension
{
    int width, height;
    dimension(int x, int y)
    {
        width = x;
        height = y;
    }
};


struct RRTMap{
    float start_x, start_y, goal_x, goal_y;
    int mapW, mapH;
    int obsDim, obsNum;

    int nodeRad = 2, nodeThickness = 0, edgeThickness = 0;

    std::vector<int> obstacles = {};

    RRTMap(float nstart_x, float nstart_y, float ngoal_x, float ngoal_y, int nmapW, int nmapH, int nobsDim, int nobsNum, std::vector<std::tuple<int, int>> obs_loc) 
    {
        start_x = nstart_x;
        start_y = nstart_y;
        goal_x = ngoal_x;
        goal_y = ngoal_y;

        mapW = nmapW;
        mapH = nmapH;

        obsDim = nobsDim;
        obsNum = nobsNum;
    } 

    void drawMap(std::vector<sf::RectangleShape> obs,sf::RenderWindow& window) 
    {
        sf::CircleShape start(nodeRad+5);
        sf::CircleShape goal(nodeRad+5);
        start.setOrigin(start.getRadius(),start.getRadius());
        goal.setOrigin(start.getRadius(),start.getRadius());
        start.setPosition(sf::Vector2f(start_x,start_y));
        goal.setPosition(sf::Vector2f(goal_x,goal_y));
        start.setFillColor(sf::Color::Blue);
        goal.setFillColor(sf::Color::Red);
        window.draw(start);
        window.draw(goal);
        drawObs(obs, window);
    }

    void drawPath(std::vector<sf::CircleShape> path, sf::RenderWindow& window) 
    {
        sf::CircleShape circle;
        for(auto& circle: path)
        {
            window.draw(circle);
        }
    }

    void drawObs(std::vector<sf::RectangleShape> obs, sf::RenderWindow& window) 
    {
        std::vector<sf::RectangleShape> obstacleList = obs;
        sf::RectangleShape rectangle;
        for(auto& rectangle: obs)
        {
            rectangle.setFillColor(sf::Color(133, 128, 115));
            window.draw(rectangle);
        }
    }
};

struct RRTGraph{
    float start_x, start_y, goal_x, goal_y;
    int mapW, mapH;
    int obsDim, obsNum;
    bool goalFlag = false;

    std::vector<int> x = {};
    std::vector<int> y = {};
    std::vector<int> parent = {};

    std::vector<sf::IntRect> obstacles = {};

    //path
    int goalState = 0;
    std::vector<int> path = {};
    std::vector<std::tuple<int, int>> ObsLoc = {};

    RRTGraph(float nstart_x, float nstart_y, float ngoal_x, float ngoal_y, int nmapW, int nmapH, int nobsDim, int nobsNum, std::vector<std::tuple<int, int>> obs_loc) 
    {
        start_x = nstart_x;
        start_y = nstart_y;
        goal_x = ngoal_x;
        goal_y = ngoal_y;

        mapW = nmapW;
        mapH = nmapH;

        x.push_back(start_x);
        y.push_back(start_y);
        parent.push_back(0);  

        //obstacles
        obsDim = nobsDim;
        obsNum = nobsNum;
        ObsLoc = obs_loc;
    }

    std::tuple<int, int> makeRandomRec() 
    {   
        std::random_device rdx, rdy;

        std::mt19937 engine_x{rdx()};
        std::mt19937 engine_y{rdy()};

        std::uniform_real_distribution<double> dist_x(0, mapW - obsDim);
        std::uniform_real_distribution<double> dist_y(0, mapH - obsDim);

        int uppercornerx = dist_x(engine_x);
        int uppercornery = dist_y(engine_y);
        return std::make_tuple(uppercornerx, uppercornery);
    }

    std::vector<sf::RectangleShape> makeObs() 
    {
        std::vector<sf::RectangleShape> obs = {};
        for (size_t i = 0; i < obsNum; i++)
        {
            sf::RectangleShape rectang;
            bool startgoalcol = true;
            while (startgoalcol)
            {
                int upper_x, upper_y;
                upper_x = std::get<0>(ObsLoc[i]);
                upper_y = std::get<1>(ObsLoc[i]);
                sf::RectangleShape rectangle(sf::Vector2f(obsDim, obsDim));
                rectang = rectangle;
                rectang.setPosition(sf::Vector2f (upper_x, upper_y));
                sf::IntRect r1(upper_x, upper_y, obsDim, obsDim);
                if ((r1.contains(start_x, start_y)) || (r1.contains(goal_x, goal_y)))
                {
                    startgoalcol = false;
                    std::cout<<"Given obstacle position coincides with the start/end nodes. (Removed from map)"<<std::endl;
                    //ObsLoc[i] = (makeRandomRec)(); //uncomment to make random obstacles
                }
                else
                {
                    startgoalcol = false;
                    obstacles.push_back(r1);
                    obs.push_back(rectang);
                }
            }           
        }
        return obs;
    }

    void add_node(int n, int n_x, int n_y) 
    {
        
        if (x.size()<n+1)
        {
            x.resize(n+1);
            y.resize(n+1);
        }
        
        x.insert(x.begin()+n, n_x);
        y.insert(y.begin()+n, n_y);
        x.erase(x.begin()+n+1);
        y.erase(y.begin()+n+1);
    }

    void remove_node(int n) 
    {
        x.erase(x.begin()+n);
        y.erase(y.begin()+n);
    }

    void add_edge(int parent_n, int child) 
    {
        parent.resize(child);
        parent.insert(parent.begin()+child, parent_n);
    }

    void remove_edge(int n) 
    {
        parent.erase(parent.begin()+n);
    }

    int number_of_nodes() 
    {
        return x.size();
    }

    int distace(int n1, int n2) 
    {
        int x1, y1;
        int x2, y2;

        x1 = x[n1];
        y1 = y[n1];

        x2 = x[n2];
        y2 = y[n2];

        int px, py;
        px = (x1-x2)*(x1-x2);
        py = (y1-y2)*(y1-y2);


        return sqrt(px+py);
    }

    std::tuple<int, int> sample_envir()
    {
        int x, y;
        std::random_device rdx, rdy;

        std::mt19937 engine_x{rdx()};
        std::mt19937 engine_y{rdy()};

        std::uniform_real_distribution<double> node_x(0, mapW - obsDim);
        std::uniform_real_distribution<double> node_y(0, mapH - obsDim);

        x = node_x(engine_x);
        y = node_y(engine_y);

        return std::make_tuple(x, y);
    }

    int nearest(int n) 
    {
        int dmin = distace(0, n);
        int nnear = 0;
        for (size_t i = 0; i < n; i++)
        {
            if (distace(i, n) < dmin)
            {
                dmin = distace(i, n);
                nnear = i;
            }
        }
        return nnear;
    }

    bool isFree() 
    {
        int n = number_of_nodes()-1;
        int n_x, n_y;
        n_x = x[n];
        n_y = y[n];
        std::vector<sf::IntRect> obs = obstacles;
        sf::RectangleShape rectangle;
        for(auto& rectangle : obs)
        {
            if(rectangle.contains(n_x, n_y))
            {
                remove_node(n);
                return false;
            }
        }
        return true;
    }

    bool crossObstacles(int x1, int x2, int y1, int y2) 
    {
        std::vector<sf::IntRect> obs = obstacles;
        sf::RectangleShape rectangle;
        for(auto& rectangle : obs)
        {
            for (float i = 0; i < 100; i++)
            {
                float u = float(i/100);
                double px = x1*u + x2*(1-u);
                double py = y1*u + y2*(1-u);
                if(rectangle.contains(px, py))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool connect(int n1, int n2) 
    {
        int x1, y1, x2, y2;
        x1 = x[n1]; y1 = y[n1];
        x2 = x[n2]; y2 = y[n2];
        //std::cout<<"Connect debug 1 "<<"From "<<x1<<","<<y1<<" to "<<x2<<","<<y2<<std::endl;
        if(crossObstacles(x1, x2, y1, y2))
        {
            //std::cout<<"Connect debug obstacle "<<"From "<<x1<<","<<y1<<" to "<<x2<<","<<y2<<std::endl;
            remove_node(n2);
            return false;
        }
        else
        {
            add_edge(n1, n2);
            //std::cout<<"Connect debug okay "<<"From "<<x1<<","<<y1<<" to "<<x2<<","<<y2<<std::endl;
            return true;
        }
        std::cout<<" "<<std::endl;
    }

    void step(int nnear, int nrand) 
    {
        int dmax = 35;
        int d = distace(nnear, nrand);
        if (d>dmax)
        {
            int xnear=x[nnear], ynear=y[nnear], xrand=x[nrand], yrand=y[nrand];
            int px = (xrand-xnear), py = (yrand-ynear);
            double theta = atan2(py, px);
            int n_x=0,n_y=0;
            n_x = int(xnear+dmax*cos(theta));
            n_y = int(ynear+dmax*sin(theta));
            
            remove_node(nrand);

            if((abs(n_x-goal_x)<35) && (abs(n_y-goal_y)<35))
            {
                if(!crossObstacles(goal_x, xnear, goal_y, ynear))
                {
                    add_node(nrand, goal_x, goal_y);
                    goalState = nrand;
                    goalFlag = true;
                }
                else
                {
                    add_node(nrand, n_x, n_y);
                }
            }
            else
            {
                add_node(nrand, n_x, n_y);
            }
        }
        else
        {
            int xnear=x[nnear], ynear=y[nnear];
            int xrand=x[nrand], yrand=y[nrand];
            if((abs(xrand-goal_x)<35) && (abs(yrand-goal_y)<35))
            {
                if(!crossObstacles(goal_x, xnear, goal_y, ynear))
                {
                    remove_node(nrand);
                    add_node(nrand, goal_x, goal_y);
                    goalState = nrand;
                    goalFlag = true;
                }
            }
        }
        
    }

    int extract_path()
    {
        if (goalFlag)
        {
            int element = goalState;
            while (element!=0)
            {
                path.resize(path.size()+1);
                path.push_back(element);
                element = parent[element];
            }
            path.push_back(0);
        }
        return goalFlag;
    }
    
    std::vector<int> path_to_goal() 
    {
        std::vector<int> path_coords_x = {};
        std::vector<int> path_coords_y = {};
        int i;
        for(auto& i : path)
        {
            path_coords_x.resize(path_coords_x.size()+1);
            path_coords_y.resize(path_coords_y.size()+1);
            path_coords_x.push_back(x[i]);
            path_coords_y.push_back(y[i]);
        }
        return path_coords_x;
    }

    void get_path_coords() {}

    void bias(sf::RenderWindow& window) 
    {
        int n = number_of_nodes();
        add_node(n, goal_x, goal_y);
        int nnear = nearest(n);
        step(nnear, n);
        connect(nnear, n);
    }

    void expand(sf::RenderWindow& window) 
    {
        int n = number_of_nodes();
        int n_x, n_y;
        std::tie(n_x, n_y) = sample_envir();
        add_node(n, n_x, n_y);
        if(isFree())
        {
            int nnearest = nearest(n);
            step(nnearest, n);
            connect(nnearest, n);    
        }
    }

    void cost() {}
};

#endif