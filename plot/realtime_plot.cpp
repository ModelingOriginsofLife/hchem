#ifndef REALTIME_PLOT_CPP
#define REALTIME_PLOT_CPP

#include <stdio.h>
#include <vector>

#include "../geometry/sphere.cpp"
#include "../geometry/segment.cpp"
#include "../network/udp_socket.cpp"

#define REALTIME_PLOT_LOCAL_PORT 1235
#define REALTIME_PLOT_DEFAULT_PORT 1234
#define REALTIME_PLOT_PACKET_SIZE 1024

struct plot_sphere
{
    sphere s;
    
    unsigned char color[3];
    char label[8];
    
    // Constructors
    
    plot_sphere(sphere s, const unsigned char color[3], const char label[8])
    {
        this->s = s;
        
        memcpy(this->color, color, 3);
        strcpy(this->label, label);
    }
    
    plot_sphere(const plot_sphere & s)
    {
        this->s = s.s;
        
        memcpy(this->color, s.color, 3);
        strcpy(this->label, s.label);
    }
    
    // Operators
    
    void operator = (const plot_sphere & s)
    {
        this->s = s.s;
        
        memcpy(this->color, s.color, 3);
        strcpy(this->label, s.label);
    }
};

struct plot_line
{
    vector2d a;
    vector2d b;
    
    unsigned char color[3];
    
    // Constructors
    
    plot_line(vector2d a, vector2d b, const unsigned char color[3])
    {
        this->a = a;
        this->b = b;
        
        memcpy(this->color, color, 3);
    }
    
    plot_line(const plot_line & l)
    {
        this->a = l.a;
        this->b = l.b;
        
        memcpy(this->color, l.color, 3);
    }
    
    // Operators
    
    void operator = (const plot_line & l)
    {
        this->a = l.a;
        this->b = l.b;
        
        memcpy(this->color, l.color, 3);
    }
};

class realtime_plot
{
    vector <plot_sphere> spheres;
    vector <plot_line> lines;
    
    unsigned int port;
    udp_socket sock;
    
    public:
    
    // Constructors
    
    realtime_plot(unsigned int port = REALTIME_PLOT_DEFAULT_PORT)
    {
        this->port = port;
        this->sock = udp_socket(REALTIME_PLOT_LOCAL_PORT);
    }
    
    // Plotting
    
    void sphere(sphere s, const unsigned char color[3], const char label[8])
    {
        this->spheres.push_back(plot_sphere(s, color, label));
    }
    
    void line(segment s, const unsigned char color[3])
    {
        this->lines.push_back(plot_line(s.a, s.b, color));
    }
    
    void render()
    {
        data plot_data;
        
        plot_data.append((unsigned int) this->spheres.size());
        plot_data.append((unsigned int) this->lines.size());
        
        for(unsigned int i=0; i<this->spheres.size(); i++)
        {
            plot_data.append((float) this->spheres[i].s.x.x);
            plot_data.append((float) this->spheres[i].s.x.y);
            plot_data.append((float) this->spheres[i].s.r);
            plot_data.append((const char *) this->spheres[i].color, 3);
            plot_data.append((const char *) this->spheres[i].label, 8);
        }
        
        for(unsigned int i=0; i<this->lines.size(); i++)
        {
            plot_data.append((float) this->lines[i].a.x);
            plot_data.append((float) this->lines[i].a.y);
            plot_data.append((float) this->lines[i].b.x);
            plot_data.append((float) this->lines[i].b.y);
            plot_data.append((const char *) this->lines[i].color, 3);
        }
        
        plot_data.prepend((unsigned int) plot_data.length());
        
        for(unsigned int i=0; i<=plot_data.length() / REALTIME_PLOT_PACKET_SIZE; i++)
        {
            unsigned int beg = REALTIME_PLOT_PACKET_SIZE * i;
            unsigned int end = REALTIME_PLOT_PACKET_SIZE * (i+1);
            
            this->sock.send(packet(address(), address("127.0.0.1", this->port), plot_data(beg, end > plot_data.length() ? plot_data.length() : end)));
        }
        
        this->spheres.clear();
        this->lines.clear();
    }
};

#endif