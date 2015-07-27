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


Texture::Texture()
{    
    m_TBO = 0;
    m_tiles = 1.0;  // 1 image per object
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TBO);
}

void Texture::load_image( string mFilename )
{ 
	printf("Texture::load_image( %s )\n", mFilename.c_str() );	
	m_image = imread(mFilename);

    //imshow( "Display Image2", m_image );
	//cvtColor( dst, cdst, COLOR_GRAY2BGR );	
}

/* Allocate a TBO id and upload the image to video memory */
GLuint	Texture::generate_TBO	( )
{ 
    //int bytes_per_pixel = m_image.elemSize();
    uint8_t* pixelPtr   = (uint8_t*)m_image.data;
    int channels        = m_image.channels();
    int type;
    switch(channels) {
        case 1: break;
        case 2: break;
        case 3: format  = GL_RGB;	type = GL_UNSIGNED_BYTE;
                format2 = GL_BGR;
                break;
        case 4: format  = GL_RGBA;	type = GL_UNSIGNED_BYTE; //type = GL_UNSIGNED_INT_8_8_8_8;
                format2 = GL_RGBA;
                break;
        default: break;
    }
    printf("Texture::generate_TBO: rows=%d; cols=%d\n", m_image.rows, m_image.cols);
    glEnable     (GL_TEXTURE_2D );
    glGenTextures(1, &m_TBO     );
    glBindTexture(GL_TEXTURE_2D, m_TBO);
    printf("Texture::generate_TBO: m_TBO=%d\n", m_TBO);		

    /* may need these for odd sized images: */
    glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
             0,
             format,
             m_image.cols, m_image.rows,
             0,
             format2,	 type,
             pixelPtr );
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	return m_TBO;
}

// data to match vertexes.  Unique to each object!
GLuint	Texture::generate_grid_coords( int mWidth, int mHeight )
{
    struct stTextCoord tc;
    float x_incr = m_tiles / mWidth;
    float y_incr = m_tiles / mHeight;
    
    for (int y=0; y<mHeight; y++)
    {
        tc.v = y*y_incr;
        for (int x=0; x<mWidth; x++)
        {
            tc.u = x*x_incr;
            m_TexCoords.push_back(tc);
        }
    }
    return (GLuint)m_TexCoords.size();
}

GLuint	Texture::generate_texture_coords	( )
{
    /* Fill in in derived class.  
       Here only a simple square coordinates.
     */
    float fs = m_tiles;
    struct stTextCoord tc;

    // U								V
    tc.u = 0.0;      tc.v = 0.0;    m_TexCoords.push_back( tc );
    tc.u = fs;       tc.v = 0.0;    m_TexCoords.push_back( tc );
    tc.u = fs;       tc.v = fs;     m_TexCoords.push_back( tc );
    tc.u = 0.0;      tc.v = fs;     m_TexCoords.push_back( tc );
    return (GLuint)m_TexCoords.size();
}


void Texture::generate_VBOTexCoords()
{
	// Generate And Bind The Texture Coordinate Buffer
    glGenBuffers( 1, &m_TBOTexCoords ); 	                // Get A Valid Name
    glBindBuffer( GL_ARRAY_BUFFER, m_TBOTexCoords );        // Bind The Buffer
    glBufferData( GL_ARRAY_BUFFER, m_TexCoords.size()*sizeof(float)*2,
    				 m_TexCoords.data(), GL_STATIC_DRAW );
}

void Texture::draw()
{ 
    glEnable            (GL_TEXTURE_2D              );
    glBindTexture       (GL_TEXTURE_2D, m_TBO       );
    glBindBuffer        (GL_ARRAY_BUFFER, m_TBOTexCoords );
    glEnableClientState (GL_TEXTURE_COORD_ARRAY     );
    glTexCoordPointer   (2, GL_FLOAT,   0, NULL     );
}

void	Texture::after_draw()
{
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable           (GL_TEXTURE_2D);
}

void Texture::print_info()
{ 

}


//      glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	// or GL_MODULATE.
//		glTexEnvi	   (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
