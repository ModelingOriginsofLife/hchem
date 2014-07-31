#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/engine.cpp"
#include "plot/realtime_plot.cpp"

using namespace std;

int main()
{
    engine e(10);
    
    realtime_plot plt;
    
    e.push_particle(particle(sphere(vector2d(0.97, 0.5), 0.01), 1, vector2d(-1, 0.23454375743)));
    e.push_particle(particle(sphere(vector2d(0.90, 0.5), 0.01), 1, vector2d(1.7, -0.23454375743)));
    e.push_particle(particle(sphere(vector2d(0.83, 0.5), 0.01), 1, vector2d(-0.4, 0.23454375743)));
    e.push_particle(particle(sphere(vector2d(0.80, 0.5), 0.01), 1, vector2d(0.2, 0.23454375743)));
    e.push_particle(particle(sphere(vector2d(0.70, 0.5), 0.01), 1, vector2d(-1.8, -0.23454375743)));
    e.push_particle(particle(sphere(vector2d(0.50, 0.5), 0.01), 1, vector2d(10, 0.23454375743)));
    while(true)
    {
        plt.plot(e);
        plt.render();
        
        e.step();
        
        double energy = 0;
        for(unsigned int i=0; i<e.particles.size(); i++)
            energy += ~e.particles[i].p / (2.0 * e.particles[i].m);
        
        cout<<energy<<endl;
        
        usleep(1.0E4);
    }
    
}