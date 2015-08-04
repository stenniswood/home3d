//
//  face_box.h
//  home3D
//
//  Created by Stephen Tenniswood on 8/1/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__face_box__
#define __home3D__face_box__

#include <stdio.h>
#include "gl_container.hpp"
#include <string>
#include "imageloader.h"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

/*
	Loads bitmap texture and attaches to a box object.
	
	This class will generate OpenGL Vertex data
	for a box of given width, height, depth.
 */

using namespace cv;
using namespace std;

const int FACE_BACK_ID   = 0;
const int FACE_FRONT_ID  = 1;
const int FACE_LEFT_ID   = 2;
const int FACE_RIGHT_ID  = 3;
const int FACE_TOP_ID    = 4;
const int FACE_BOTTOM_ID = 5;


/*  This "cube" stores duplicate vertices for each face.
    This allows normals on each face, and independant textures.
 
 */
class glFaceBox : public glAtom
{
public:
    glFaceBox();
    
    virtual void            generate_vertices( );
    virtual void            generate_indices ( );

    // Show the texture just on 1 side ()
    Texture*        load_image				(string mFilename, int mSide=1);
    void			apply_front				( Texture* mTexture, char mOrientation=0 );
    void			apply_back				( Texture* mTexture, char mOrientation=0 );
    void			apply_top				( Texture* mTexture, char mOrientation=0 );
    void			apply_bottom			( Texture* mTexture, char mOrientation=0 );
    void			apply_left				( Texture* mTexture, char mOrientation=0 );
    void			apply_right				( Texture* mTexture, char mOrientation=0 );
    
    GLuint			generate_texture_coords_4_side_stretch( );
    GLuint			generate_texture_coords	( int mSide );
    virtual void	gl_register             ( );
    
    virtual void	draw_body	();
    virtual void	print_info	();
    
    int				m_side_applied;
    
    int             m_repetitions_x;
    int             m_repetitions_y;
    
    float 	width;          // X axis
    float 	height;         // Y axis
    float 	depth;          // Z axis
    
    vector<struct stTextCoord>	m_TexCoords;    
    char             m_orientations[6];
    Texture*         m_side[6];     // each face can have a different texture    
};

/*
 void			wrap_3_sides            ();     // top, front, bottom
 void			wrap_4_sides            ();     // top, front, bottom
 void			wrap_5_sides            ();     // top, front, bottom
 void			wrap_6_sides            ();     // top, front, bottom
 
*/
#endif /* defined(__home3D__face_box__) */
