#ifndef ENGINE_CPP
#define ENGINE_CPP

#define MAX_SEGMENTS 1048576

#include "zone.cpp"
#include "../data/event_heap.cpp"
#include "../data/checklist.cpp"

class engine
{
    vector <particle> particles;
    vector <segment> segments;
    
    unsigned int size;
    zone ** zones;

    struct
    {
        vector <unsigned int> vec;
        checklist check;
    } active_zones;
    
    unsigned int event_id;
    event_heap events;

    checklist event_skip;
    
    double time;
    double timestep;
    
    public:
    
    // Constructors
    
    engine(unsigned int size)
    {
        this->size = size;

        this->zones = new zone * [this->size];
        for(unsigned int i=0; i<this->size; i++)
            this->zones[i] = new zone[this->size];
        
        this->segments.reserve(MAX_SEGMENTS);
        
        this->push(segment(vector2d(1.0E-6, 1.0E-6), vector2d(1.0E-6, 1-1.0E-6)));
        this->push(segment(vector2d(1.0E-6, 1-1.0E-6), vector2d(1-1.0E-6, 1-1.0E-6)));
        this->push(segment(vector2d(1-1.0E-6, 1-1.0E-6), vector2d(1-1.0E-6, 1.0E-6)));
        this->push(segment(vector2d(1-1.0E-6, 1.0E-6), vector2d(1.0E-6, 1.0E-6)));
        
        this->time = 0;
    }
    
    // Methods
    
    void push(particle p)
    {
        p.id = (unsigned int) this->particles.size();
        this->particles.push_back(p);
    }
    
    void push(segment s)
    {
        this->segments.push_back(s);
        
        double step = 0.5 / this->size;
        unsigned int steps = ceil(!(s.a - s.b) / step);
        
        checklist c;
        
        for(unsigned int i=0; i<=steps; i++)
        {
            vector2d v = s.a * (float(i) / steps) + s.b * (float((steps - i)) / steps);
            
            int X = v.x * this->size;
            int Y = v.y * this->size;
            
            for(int x = X - 1; x <= X + 1; x++)
                for(int y = Y - 1; y <= Y + 1; y++)
                    if(x>=0 && x<this->size && y>=0 && y<this->size)
                        if(!c[this->size * y + x])
                        {
                            c.check(this->size * y + x);
                            this->zones[x][y].segments.push_back(&(this->segments.back()));
                        }
        }
    }
    
    void bind(unsigned int i, unsigned int j, double l)
    {
        bind_particles(this->particles[i], this->particles[j], l);
    }
    
    // Step methods
    
    private:
    
    void setup_timestep()
    {
        this->timestep = INFINITY;
        
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            double t = 0.5 / this->size / (!this->particles[i].p / this->particles[i].m);
            if(t < this->timestep)
                this->timestep = t;
        }
    }
    
    void setup_zones()
    {
        for(unsigned int i=0; i<this->active_zones.vec.size(); i++)
        {
            unsigned int x = this->active_zones.vec[i] % this->size;
            unsigned int y = this->active_zones.vec[i] / this->size;
            
            this->zones[x][y].particles.clear();
        }
        
        this->active_zones.vec.clear();
        this->active_zones.check.clear();
        
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            int X = this->particles[i].s.x.x * this->size;
            int Y = this->particles[i].s.x.y * this->size;
            
            for(int x=X-1; x<=X+1; x++)
                for(int y=Y-1; y<=Y+1; y++)
                    if(x>=0 && x<this->size && y>=0 && y<this->size)
                    {
                        if(!this->active_zones.check[y*this->size + x])
                        {
                            this->active_zones.check.check(y*this->size + x);
                            this->active_zones.vec.push_back(y*this->size + x);
                        }

                        this->zones[x][y].particles.push_back(&(this->particles[i]));
                    }
        }
    }
    
    void detect_events(unsigned int i, bool all = false, int skip = -1)
    {
        unsigned int X = this->particles[i].s.x.x * this->size;
        unsigned int Y = this->particles[i].s.x.y * this->size;
        
        for(unsigned int j=0; j<this->zones[X][Y].particles.size(); j++)
        {
            particle * p = this->zones[X][Y].particles[j];

            if((!all && this->particles[i].id < p->id) || (all && this->particles[i].id != p->id && skip != p->id))
            {
                event e(this->particles[i], *p, this->time + this->timestep, this->event_id);
                
                if(e)
                {
                    this->particles[i].events.push_back(event_id);
                    p->events.push_back(event_id);
                    
                    this->events.push(e);
                    this->event_id++;
                }
            }
        }
        
        for(unsigned int j=0; j<this->zones[X][Y].segments.size(); j++)
        {
            event e(this->particles[i], *(this->zones[X][Y].segments[j]), this->time + this->timestep, this->event_id);
            if(e)
            {
                this->particles[i].events.push_back(event_id);
                
                this->events.push(e);
                this->event_id++;
            }
        }
        
        for(unsigned int j=0; j<this->particles[i].bonds.size(); j++)
        {
            particle * p = (&(this->particles[i]) == this->particles[i].bonds[j]->a) ? this->particles[i].bonds[j]->b : this->particles[i].bonds[j]->a;
            
            if((!all && this->particles[i].id < p->id) || (all && p->id != skip))
            {
                event e(*(this->particles[i].bonds[j]), this->time + this->timestep, this->event_id);
                if(e)
                {
                    this->particles[i].events.push_back(this->event_id);
                    p->events.push_back(this->event_id);
                    
                    this->events.push(e);
                    this->event_id++;
                }
            }
        }
    }
    
    void setup_events()
    {
        this->event_id = 0;
        this->event_skip.clear();
        
        for(unsigned int i=0; i<this->particles.size(); i++)
            this->detect_events(i);
    }
    
    void compute_events()
    {
        while(this->events.length())
        {
            event e = this->events.pop();
            
            cout<<"Found event "<<e<<endl;
            
            if(this->event_skip[e.id])
            {
                cout<<"Skipping."<<endl;
                continue;
            }

            
            e.compute();
            cout<<"Computed."<<endl;
            
            switch(e.type)
            {
                case event::PARTICLE:
                {
                    for(unsigned int i=0; i<e.a->events.size(); i++)
                    {
                        cout<<"Skipping further event "<<e.a->events[i]<<" for particle A"<<endl;
                        this->event_skip.check(e.a->events[i]);
                    }
                    
                    for(unsigned int i=0; i<e.b->events.size(); i++)
                    {
                        cout<<"Skipping further event "<<e.b->events[i]<<" for particle B"<<endl;
                        this->event_skip.check(e.b->events[i]);
                    }
                    
                    e.a->events.clear();
                    e.b->events.clear();
                    
                    cout<<"Detecting new events.."<<endl;
                    
                    this->detect_events(e.a->id, true);
                    this->detect_events(e.b->id, true, e.a->id);
                    
                    break;
                }
                
                case event::SEGMENT_A:
                case event::SEGMENT_B:
                case event::SEGMENT_MID:
                {
                    for(unsigned int i=0; i<e.a->events.size(); i++)
                    {
                        cout<<"Skipping further event "<<e.a->events[i]<<" for particle."<<endl;
                        this->event_skip.check(e.a->events[i]);
                    }
                    
                    e.a->events.clear();
        
                    this->detect_events(e.a->id, true);
                    
                    break;
                }
                    
                case event::BOND_STRETCH:
                {
                    for(unsigned int i=0; i<e.n->a->events.size(); i++)
                    {
                        cout<<"Skipping further event "<<e.n->a->events[i]<<" for particle A"<<endl;
                        this->event_skip.check(e.n->a->events[i]);
                    }

                    for(unsigned int i=0; i<e.n->b->events.size(); i++)
                    {
                        cout<<"Skipping further event "<<e.n->b->events[i]<<" for particle B"<<endl;
                        this->event_skip.check(e.n->b->events[i]);
                    }
                    
                    
                    e.n->a->events.clear();
                    e.n->b->events.clear();
                    
                    this->detect_events(e.n->a->id, true);
                    this->detect_events(e.n->b->id, true, e.n->a->id);
                    
                    break;
                }
            }
        }
    }
    
    void integrate_motion()
    {
        for(unsigned int i=0; i<this->particles.size(); i++)
        {
            this->particles[i].s.x += this->particles[i].p / this->particles[i].m * (this->time + this->timestep - this->particles[i].t);
            this->particles[i].t = this->time + this->timestep;
        }
        
        this->time += this->timestep;
    }
    
    public:
    
    void step()
    {
        this->setup_timestep();
        
        this->setup_zones();
        this->setup_events();
        this->compute_events();
        this->integrate_motion();
    }
    
    friend class realtime_plot;
    friend ostream & operator << (ostream &, engine &);
};

ostream & operator << (ostream & out, engine & e)
{
    out<<"Time = "<<e.time<<endl<<endl;
    for(unsigned int i = 0; i<e.particles.size(); i++)
        out<<e.particles[i]<<endl;
    
    out<<endl<<endl<<endl;
    
    return out;
}

#endif