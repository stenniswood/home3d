/* 
	Loads bitmap texture and attaches to a box object.
	
	This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 	
*/
#ifndef _TEXTURE_1234_HPP_
#define _TEXTURE_1234_HPP_

//#include "all_objects.h"
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <GLUT/glut.h>


using namespace cv;
using namespace std;

struct stTextCoord
{
    float u;
    float v;
};

class Texture
{
public:
	Texture ( );
	~Texture( );

	void			load_image				(string mFilename);
    
	// The object can either create a class derived from this and override this function.
	// Or ignore this function and add coordinates to "m_TexCoords" - better usually.
    GLuint          generate_grid_coords    ( int mWidth, int mHeight );
    GLuint			generate_texture_coords	( int mRotation = 0);		// data to match vertexes.  Unique to each object!
	GLuint			generate_TBO			( );		// handle
	void			generate_VBOTexCoords	( );		// handle

	virtual void	draw(); 
	virtual void    after_draw();
	virtual void	print_info();

	GLuint			m_TBO;				// The image 
	GLuint			m_TBOTexCoords;		// The coordinates
	vector<struct stTextCoord>	m_TexCoords;

    int             format,format2;
    int             m_repetitions_x;
    int             m_repetitions_y;
//    float           m_tiles;
	Mat 			m_image;
    
};


#endif
