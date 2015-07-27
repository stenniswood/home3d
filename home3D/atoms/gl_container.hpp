/* This class will generate OpenGL Vertex data for

	a   Box   of given width, height, depth.
*/
#ifndef _GL_BOX_
#define _GL_BOX_

#include "gl_atom.hpp"

#define  BUFFER_OFFSET(i) ((GLuint*)NULL + (i))

class glBox : public glAtom
{
public:
	glBox();

	virtual void 	setup  		( );
	virtual void 	create 		( );
	void            generate_vertices	( );

	void            change_top_color   	( long mColor );
	void            change_bottom_color	( long mColor );

	virtual void	draw_body   ( );
	void            print_info  ( );

	float 	width;
	float 	height;
	float 	depth;
	bool	m_is_closed;
};



// DO THIS LATER AFTER vertices are generated.
const int TYPE_CLOSED 		= 1;
const int TYPE_OPEN   		= 1;
const int TYPE_WIRE_FRAME 	= 1;


#endif
