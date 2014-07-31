#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <vector>

#include "../dynamics/particle.cpp"
#include "../dynamics/bond.cpp"
#include "../dynamics/collision.cpp"
#include "../data/collision_heap.cpp"
#include "../data/checklist.cpp"

class engine
{
    vector <particle> particles;
    
    vector <particle *> ** zones;
    struct
    {
        vector <unsigned int> vec;
        checklist check;
    } active_zones;
    
    vector <bond> bonds;
    
    vector <segment> segments;
    vector <segment> ** scaffolding;
    
    unsigned int size;
    
    unsigned int collision_id;
    collision_heap collisions;
    checklist skiplist;

    double time;
    double timestep;
    
    public:
    
    // Constructors
    
    engine(unsigned int size)
    {
        this->zones = new vector <particle *> * [size];
        this->scaffolding = new vector <segment> * [size];
        
        for(unsigned int i=0; i<size; i++)
        {
            this->zones[i] = new vector <particle *> [size];
            this->scaffolding[i] = new vector <segment> [size];
        }
        
        this->size = size;
        
        this->push_segment(segment(vector2d(0, 0), vector2d(1, 0)));
        this->push_segment(segment(vector2d(1, 0), vector2d(1, 1)));
        this->push_segment(segment(vector2d(1, 1), vector2d(0, 1)));
        this->push_segment(segment(vector2d(0, 1), vector2d(0, 0)));
        
        this->collision_id = 0;
        this->time = 0;
    }
    
    // Methods
    
    void push_segment(segment s)
    {
        this->segments.push_back(s);
        
        double segment_length = !(s.a - s.b);
        unsigned int steps = ceil(segment_length / (1.0 / this->size));

        int last_X = -1;
        int last_Y = -1;
        
        for(unsigned int i=0; i<=steps; i++)
        {
            int X = (s.a + (s.b - s.a) * (1.0 / steps) * i).x * this->size;
            int Y = (s.a + (s.b - s.a) * (1.0 / steps) * i).y * this->size;
            
            if(X != last_X || Y != last_Y)
            {
                for(int x = X - 1; x <= X + 1; x++)
                    for(int y = Y - 1; y <= Y + 1; y++)
                        if(x>=0 && x<this->size && y>=0 && y<this->size)
                            this->scaffolding[x][y].push_back(s);
                
                last_X = X;
                last_Y = Y;
            }
        }
    }
    
    void push_particle(particle p)
    {
        this->particles.push_back(p);
    }
    
    void push_bond(unsigned int a, unsigned int b, double l)
    {
        this->bonds.push_back(bond(this->particles[a], this->particles[b], l));
        bond * new_bond = &(this->bonds[this->bonds.size() - 1]);
        
        this->particles[a].bonds.push_back(new_bond);
        this->particles[b].bonds.push_back(new_bond);
    }
    
    // Internal methods
    
    void assign_zones()
    {
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            int X = this->particles[i].s.x.x * this->size;
            int Y = this->particles[i].s.x.y * this->size;
            
            for(int x = X - 1; x <= X + 1; x++)
                for(int y = Y - 1; y <= Y + 1; y++)
                    if(x >= 0 && x < this->size && y >= 0 && y < this->size)
                    {
                        if(!(this->active_zones.check[y * this->size + x]))
                        {
                            this->active_zones.check.check(y * this->size + x);
                            this->active_zones.vec.push_back(y * this->size + x);
                        }
                        
                        this->zones[x][y].push_back(&(this->particles[i]));
                    }
        }
    }
    
    void detect_particle_collisions(unsigned int i, bool all_collisions = false)
    {
        int X = this->particles[i].s.x.x * this->size;
        int Y = this->particles[i].s.x.y * this->size;
        
        for(unsigned int j=0; j<this->zones[X][Y].size(); j++)
            if((!all_collisions && (&(this->particles[i]) < this->zones[X][Y][j])) || (all_collisions && (&(this->particles[i]) != this->zones[X][Y][j])))
            {
                collision c(this->particles[i], *(this->zones[X][Y][j]), this->time + this->timestep, this->collision_id++);
                if(c)
                {
                    this->collisions.push(c);
                    this->particles[i].collisions.push_back(c.id);
                    this->zones[X][Y][j]->collisions.push_back(c.id);
                }
            }
        
        for(unsigned int j=0; j<this->scaffolding[X][Y].size(); j++)
        {
            collision c(this->particles[i], this->scaffolding[X][Y][j], this->time + this->timestep, this->collision_id++);
            if(c)
            {
                this->collisions.push(c);
                this->particles[i].collisions.push_back(c.id);
            }
        }
    }
    
    void detect_bond_stretch(unsigned int i, bool all_stretches = false, particle * skip = NULL)
    {
        for(unsigned int j=0; j<this->particles[i].bonds.size(); j++)
        {
            bond * b = this->particles[i].bonds[i];
            particle * p = b->a == &(this->particles[i]) ? b->b : b->a;
            if((!all_stretches && p < &(this->particles[i])) || (all_stretches && p != skip))
            {
                collision c((*b), this->time + this->timestep, this->collision_id++);
                if(c)
                {
                    this->collisions.push(c);
                    this->particles[i].collisions.push_back(c.id);
                    p->collisions.push_back(c.id);
                }
            }
        }
    }
    
    void detect_collisions()
    {
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            detect_particle_collisions(i);
            detect_bond_stretch(i);
        }
    }
    
    void compute_collisions()
    {
        while(this->collisions.length())
        {
            collision c = this->collisions.pop();

            if(!this->skiplist[c.id])
            {
                c.compute();
                
                for(unsigned int i=0; i < c.a->collisions.size(); i++)
                    this->skiplist.check(c.a->collisions[i]);
                
                c.a->collisions.clear();
                                        
                if(c.type == collision::PARTICLE)
                {
                    for(unsigned int i = 0; i < c.b->collisions.size(); i++)
                        this->skiplist.check(c.b->collisions[i]);
                    
                    c.b->collisions.clear();
                }
                
                this->detect_particle_collisions((unsigned int) (c.a - &(this->particles[0])), true);

                if(c.type == collision::PARTICLE)
                    this->detect_particle_collisions((unsigned int) (c.b - &(this->particles[0])), true);
            }
        }
    }
    
    void integrate_motion()
    {
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            this->particles[i].s.x += (this->particles[i].p / this->particles[i].m) * (this->time + this->timestep - this->particles[i].t);
            this->particles[i].t = this->time + this->timestep;
        }
        
        this->time += this->timestep;
    }
    
    void clear()
    {
        this->collision_id = 0;
        
        for(unsigned int i=0; i<this->active_zones.vec.size(); i++)
        {
            unsigned int X = this->active_zones.vec[i] % this->size;
            unsigned int Y = this->active_zones.vec[i] / this->size;
            
            this->zones[X][Y].clear();
        }
        
        this->active_zones.vec.clear();
        this->active_zones.check.clear();
        
        this->skiplist.clear();
    }
    
    // Methods
    
    void step()
    {
        this->timestep = INFINITY;
        
        for(unsigned int i = 0; i<this->particles.size(); i++)
            if((0.5 / this->size) / (!this->particles[i].p / this->particles[i].m) < this->timestep)
                this->timestep = (0.5 / this->size) / (!this->particles[i].p / this->particles[i].m);
        
        this->assign_zones();
        this->detect_collisions();
        this->compute_collisions();
        this->integrate_motion();
        this->clear();
    }

    friend class realtime_plot;
    friend int main();
};

#endif