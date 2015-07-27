#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include "all_objects.h"

// Include OpenCV here for the imread function!
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// Include OpenCV here for the imread function!

using namespace cv;
using namespace std;


txtContainer::txtContainer(  )
:glBox()
{
    m_object_type_name = "textured box";    
    m_texture = new Texture();
    m_object_class  = 9;
	m_side_applied	= TOP_SIDE_ID;
}

void txtContainer::load_image( string mFilename )
{
    m_texture->load_image(mFilename);
}

void txtContainer::gl_register(  )
{
    glAtom::gl_register();       // Base class

    m_texture->generate_TBO();
    generate_texture_coords( );
    m_texture->generate_VBOTexCoords();
    //generate_texture_coords_4_side_stretch();
}


GLuint txtContainer::generate_texture_coords_4_side_stretch( )
{
	float hdh = 1.0/(height+depth+height);
	float hwh = 1.0/(height+width+height);
    const float fs = 1.0;
    struct stTextCoord tc;
    
	// U                                            V
    tc.u = hwh*height;          tc.v = hdh*height;            m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*height;          tc.v = hdh*(height+depth);    m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*(height+width);  tc.v = hdh*(height+depth);    m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*(height+width);  tc.v =hdh*height ;            m_texture->m_TexCoords.push_back( tc );

    tc.u = 0.0;                 tc.v = 0.0;         m_texture->m_TexCoords.push_back( tc );
    tc.u = 0.0;                 tc.v = fs;          m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;                  tc.v = fs;          m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;                  tc.v = 0.0;         m_texture->m_TexCoords.push_back( tc );
	return m_texture->m_TexCoords.size()/2.;
}

GLuint txtContainer::generate_texture_coords( )
{
	// Texture coords [0.0..1.0]   1.0 represents the far edge of the image.
	// Pretty sure we need 1 coordinate for each vertex.
	//   So if we're just doing 1 face.  It should mean just 4 text coords.
	//m_NumTexCoords = 8;
	const float fs = 1.0;
	// U									V
    struct stTextCoord tc;
    tc.u = 0.0;  tc.v = 0.0;    	m_texture->m_TexCoords.push_back( tc );
    tc.u = 0.0;  tc.v = fs;         m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;   tc.v = fs;     	m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;   tc.v = 0.0;        m_texture->m_TexCoords.push_back( tc );

    tc.u = 0.0;  tc.v = 0.0;    	m_texture->m_TexCoords.push_back( tc );
    tc.u = 0.0;  tc.v = fs;         m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;   tc.v = fs;     	m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;   tc.v = 0.0;        m_texture->m_TexCoords.push_back( tc );
	return m_texture->m_TexCoords.size()/2.;
}

void txtContainer::draw_body()
{
	//Make the new VBO active. Repeat here incase changed since initialisation
	glBindBuffer    (GL_ARRAY_BUFFER, 		  m_VBO	);
	glBindBuffer    (GL_ELEMENT_ARRAY_BUFFER, m_IBO );

	glVertexPointer (3, GL_FLOAT, 		  sizeof(struct Vertex), NULL );
	glColorPointer  (4, GL_UNSIGNED_BYTE, sizeof(struct Vertex), (GLvoid*)(offsetof(struct Vertex,color)));

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY  );
	
	if (m_is_closed)                        // GL_QUADS
	{
        m_texture->draw();
		glDrawElements	   (GL_QUADS,  (int)m_indices.size(), GL_UNSIGNED_INT, (GLvoid*)((char*)NULL));
        m_texture->after_draw();
	}
}

void txtContainer::print_info()
{
	printf("txtContainer()  <x,y,z> = <%6.3f %6.3f %6.3f> \n", 
			m_x,m_y,m_z );
	printf(" width=%6.3f;  height=%6.3f; depth=%6.3f> \n", 
			width, height, depth );
}



//cvtColor( src, m_src, CV_BGR2RGB ); Data goes:BGR
//imshow( "Display Image", m_src );
