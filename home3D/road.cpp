//
//  road.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 8/14/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "road.h"


glRoad::glRoad()
{
    
}
glRoad::~glRoad()
{
    
}

void glRoad::generate_vertices()
{
    struct Vertex_pnc vert;
    long size = m_path.m_vertices.size();
    for (int i=0; i<size; i++)
    {
        vert = m_path.m_vertices[i];
        
    }
    
}

void glRoad::generate_indices ( )
{
    
}



