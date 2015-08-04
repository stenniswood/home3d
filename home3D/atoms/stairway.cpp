#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
//#include "all_objects.h"

#include "stairway.hpp"
#include "extrusion.hpp"


#define Debug 0
#define TOP_AND_BOTTOM 2
#define CLOSE_OUT 1
#define VERTICES_PER_STEP 2

glStairway::glStairway(  )
{
    m_name = "stairway";
	m_rise	=  8.;		// inches standard height
	m_run	= 10.;		// rise + run == 18.
//	m_width =  3.*12;		//

    m_extrusion_axis  = 2;
	m_number_of_steps = 15;
	m_color = 0xFFFFFFFF;
	m_is_closed = true;		// always for a stairway!
}

// Sample stairway.  Each vertex is a dimension in inches.
/*
____
	|___
		|___
*/
//		if (Debug) printf("stair:  x,y,z = %6.3f, %6.3f, %6.3f \n",
//			v.position[0], v.position[1], v.position[2] );

void glStairway::generate_layer_vertices( )
{
    // GENERATE LEFT SIDE:
    // 2 vertices per step :
    struct Vertex_pnc v,v2;
    v2.color[0]= 0xFF;
    v2.color[1]= 0x2F;
    v2.color[2]= 0x7F;
    v2.color[3]= 0x7F;
    set_vertex_color( v );
    v.position[2] = 0;
    v2.position[2] = 0;
    
	// Zero will be the bottom of the lowest step
	for (int s=0; s<m_number_of_steps; s++)
	{
        v.position[0] = m_run *s;
        v.position[1] = m_rise*s;
        m_vertices.push_back(v);
        
		v2.position[0] = m_run*(s+1);
		v2.position[1] = m_rise*s;
        m_vertices.push_back( v2 );
    }
    
	// One more at base of stairs:
	v.position[0] = m_run*(m_number_of_steps);
	v.position[1] =  0.;
    m_vertices.push_back(v);
    m_layer_one_vertices = m_vertices.size();
}


float glStairway::get_height( int mstep )
{
	return (m_rise * mstep);
}

// default is top step
float glStairway::get_front_edge( int mstep )
{
    return (m_run * mstep)+0;
}

/*void glStairway::draw()
{
	glPushMatrix();

	glTranslatef(m_x, m_y, m_z);
//	glRotatef 	(m_angle, 0.0f, 1.0f, 0.0f );
	
	//Make the new VBO active. Repeat here incase changed since initialisation
	glBindBuffer(GL_ARRAY_BUFFER, 		  m_VBO	);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO );

	glVertexPointer(3, GL_FLOAT, 		 sizeof(struct Vertex), NULL);
	glColorPointer (4, GL_UNSIGNED_BYTE, sizeof(struct Vertex), (GLvoid*)(offsetof(struct Vertex,color)));
	
	//Establish array contains vertices (not normals, colours, texture coords etc)
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// We draw the top and bottoms as GL_POLYGON and the sides as GL_QUAD_STRIP.
	// Both sets of indices stored in GL_INDEX_ARRAY.  With a known offset for each 
	// of the 3 sets.

	// Draw Floor:
	glDrawElements(GL_LINE_LOOP, m_floor_indices, GL_UNSIGNED_BYTE, 0 );

	// Draw the sides:
	int side_indices = m_floor_indices*2;
	glDrawElements(GL_QUAD_STRIP, side_indices, GL_UNSIGNED_BYTE,
						(GLvoid*)BUFFER_OFFSET( m_floor_indices ) );

	// Draw Ceiling:
	if (m_is_closed)
		glDrawElements(GL_LINE_LOOP, m_floor_indices, GL_UNSIGNED_BYTE, 
						(GLvoid*)BUFFER_OFFSET(m_floor_indices+side_indices) );

	glPopMatrix();
}*/
	