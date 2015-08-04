#ifndef _STAIRWAY_H_
#define _STAIRWAY_H_

#include "extrusion.hpp"
//#include "

/* This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 
*/

class glStairway : public glExtrusion
{
public:
	glStairway	(  );

	virtual void	generate_layer_vertices  (  );
    
    float	get_height    ( int mstep=-1 );     // default is max height
    float	get_front_edge( int mstep=-1 );     // default is top step
    
	float 	 m_rise;
	float 	 m_run;		
	//float 	 m_width;           USE m_extrusion_length!
	int		 m_number_of_steps;
    
};


#endif
