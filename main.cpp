#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>

#include "engine/engine.cpp"
#include "plot/realtime_plot.cpp"

using namespace std;

int main()
{
    /*particle a(sphere(vector2d(0.5, 0.5), 0.01), 1, vector2d(0, 0));
    particle b(sphere(vector2d(0.7, 0.7), 0.01), 1, vector2d(1, 0));
    
    particle c(sphere(vector2d(0.1, 0.1), 0.01), 10, vector2d(13.5453, 3.875682));
    
    engine e(30);

    e.push(a);
    e.push(b);
    e.push(c);
    
    e.bind(0, 1, 0.4);
    
    realtime_plot plt;
    
    for(unsigned int i=0; i<1000000; i++)
    {
        cout<<"STEP "<<i<<endl;
        plt.plot(e);
        plt.render();
        
        usleep(5.0E4);
        
        e.step();
    }*/
    
    
}