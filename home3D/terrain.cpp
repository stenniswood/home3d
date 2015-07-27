//
//  terrain.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/9/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "terrain.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <vector>
#include <stdio.h>
#include "all_objects.h"

// Include OpenCV here for the imread function!
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// Include OpenCV here for the imread function!
using namespace cv ;
using namespace std;


glTerrain::glTerrain()
{
    m_stretch_x = 5.0;
    m_stretch_y = 5.0;
    m_y_scale = 0.5;
}
glTerrain::~glTerrain()
{
    
}

void glTerrain::load_image( string mFilename )
{
    printf("TerrainMap::load_image( %s )\n", mFilename.c_str() );
    m_image = imread(mFilename, IMREAD_GRAYSCALE);      // Note imread can't use "~/" for a pathname!  results in 0 rows,cols.
    
   // Mat dst;
   // cvtColor( m_image, dst, COLOR_BGR2GRAY );
}

void glTerrain::generate_vertices()
{
    long x_size = m_image.cols;
    long y_size = m_image.rows;
    long index;
    struct Vertex v;
    set_vertex_color(v );
    
    for (int x=0; x<x_size; x++)            // cols
        for (int y=0; y<y_size; y++)        // rows
        {
            index = y*m_image.cols + x;
            v.color[0] = 0xFF;
            v.color[1] = m_image.data[index] & 0xFF;
            v.color[2] = m_image.data[index] & 0xFF;
            v.color[3] = 0xFF;
            
            v.position[0] = x*m_stretch_x;
            v.position[2] = y*m_stretch_y;
            v.position[1] = m_image.data[index] * m_y_scale;
            m_vertices.push_back( v );
        }
}

void glTerrain::generate_indices()
{
    GLuint x_size = m_image.cols;
    GLuint y_size = m_image.rows;
    GLuint col1_start = 0;
    GLuint col2_start = y_size;

    long index;
    struct Vertex v;
    set_vertex_color( v );
    
    for (int x=0; x<(x_size/2-2); x++)          // cols
    {
        for (int y=0; y<(y_size-2); y++)        // rows
        {
            m_indices.push_back( col1_start+y );
            m_indices.push_back( col2_start+y );
        }
        col1_start += y_size;     // leap frog
        col2_start += y_size;     //

        for (int y=y_size-2; y>0; y--)        // rows
        {
            m_indices.push_back( col1_start+y );
            m_indices.push_back( col2_start+y );
        }
        
        // how many vertices does it jump form 1 col to the next?  answer: y_size
        col1_start += y_size;     // leap frog
        col2_start += y_size;     //
    }
}

void glTerrain::draw_primitive( )
{
    glDrawElements(GL_QUAD_STRIP, (int)m_indices.size(), GL_UNSIGNED_INT, 0 );
}

