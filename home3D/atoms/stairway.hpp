#ifndef _STAIRWAY_H_
#define _STAIRWAY_H_

/* This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 
*/

class glStairway : public glExtrusion
{
public:
	glStairway	(  );

	virtual void	generate_layer_vertices  (  );
	void	generate_side2_vertices ( );
	void 	generate_vertices_colors( );
	float	get_height();
	
	float 	 m_rise;
	float 	 m_run;		
	float 	 m_width;
	int		 m_number_of_steps;

//	GLbyte 	generate_side_IBO	( GLubyte* mptr, GLbyte mStartingVertexIndex );
//	GLbyte 	generate_side2_IBO	( GLubyte* mptr, GLbyte mStartingVertexIndex );
//	void 	generate_stair_IBO	( GLubyte* isptr );
//	void 	generate_IBO( );
//	void 	generate_VBO( );
//	void	draw();
//  GLbyte	 m_floor_indices;
//	int		 m_total_vertices;
//	GLbyte   m_number_of_floor_vertices;
//	Vertex*  m_vertices;
    
};


#endif
