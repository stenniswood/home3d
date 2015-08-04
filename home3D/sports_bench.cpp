//
//  sports_bench.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 8/3/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "sports_bench.h"
#include "gl_container.hpp"
#include "cylinder.hpp"


const float LEG_SPACING = 4*12;

glBench::glBench ()
{
    
}
glBench::~glBench()
{
    
}

void glBench::create_vertical_walls( )
{
    glBox* tmp = new glBox();
    tmp->width = m_length;             // length of bench
    tmp->height = 1.0;          // 1 inch deep
    tmp->depth  = 18;           // 18 inches wide
    tmp->setup();
    tmp->relocate( 0, m_height-tmp->height/2, 0);
    tmp->grab_right();
    m_components.push_back(tmp);
    
    int num_legs = m_length / (LEG_SPACING);           // every 5 feet.
    glCylinder* leg=NULL;
    float offset = LEG_SPACING/2;
    for (int i=0; i<num_legs; i++)
    {
        leg = new glCylinder(24);
        leg->m_radius = 1;
        leg->m_extrusion_axis = 1;
        leg->m_extrusion_length = m_height-1.0;
        leg->setup();
        leg->relocate( offset, 0, 0);
        m_components.push_back( leg );
        offset += LEG_SPACING;
    }
}

void    glBench::create_horizontal_walls ( )
{
    
}
void    glBench::add_light_switches      ( )
{
    
}
void    glBench::create_components       ( )
{
    create_vertical_walls();
    
}
