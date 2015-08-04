//
//  face_box.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 8/1/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>

// Include OpenCV here for the imread function!
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// Include OpenCV here for the imread function!

#include "face_box.h"
#include "all_objects.h"

using namespace cv;
using namespace std;


glFaceBox::glFaceBox(  )
{
    for (int i=0; i<6; i++) {
        m_side[i] = NULL;
        m_orientations[i] = 0;
    }
    
    m_repetitions_x = 1.0;
    m_repetitions_y = 1.0;
    m_object_type_name = "textured box";
    m_object_class  = 9;
    m_side_applied	= TOP_SIDE_ID;
}

/* 4*6=24 vertices total. */
void glFaceBox::generate_vertices	( )
{
    struct Vertex_pnc v;
    set_vertex_color( v );
    
    // FACE #1: (back)
    v.normal[0]   = 0.0;
    v.normal[1]   = 0.0;
    v.normal[2]   = -1.0;
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    
    // FACE #2: (front)
    v.normal[0]   = 0.0;
    v.normal[1]   = 0.0;
    v.normal[2]   = +1.0;
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );

    // FACE #3: (left)
    v.normal[0]   = -1.0;
    v.normal[1]   = 0.0;
    v.normal[2]   = 0.0;
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );

    // FACE #4: (right)
    v.normal[0]   = +1.0;
    v.normal[1]   = 0.0;
    v.normal[2]   = 0.0;
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );

    // FACE #4: (top)
    v.normal[0]   = 0.0;
    v.normal[1]   = +1.0;
    v.normal[2]   = 0.0;
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = +height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );

    // FACE #4: (bottom)
    v.normal[0]   = 0.0;
    v.normal[1]   = -1.0;
    v.normal[2]   = 0.0;
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = +depth/2;
    m_vertices.push_back( v );
    v.position[0] = +width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    v.position[0] = -width/2;
    v.position[1] = -height/2;
    v.position[2] = -depth/2;
    m_vertices.push_back( v );
    compute_min_max();
}

void glFaceBox::generate_indices ( )
{
    for (int i=0; i<24; i++)
        m_indices.push_back(i);
}

Texture* glFaceBox::load_image( string mFilename, int mSide )
{
    m_side[mSide] = new Texture();
    m_side[mSide]->load_image(mFilename);
    return m_side[mSide];
}

void glFaceBox::apply_top( Texture* mTexture, char mOrientation )     // Show the texture just on 1 side ()
{
    m_orientations[FACE_TOP_ID] = mOrientation;
    m_side[FACE_TOP_ID] = mTexture;
}
void glFaceBox::apply_bottom( Texture* mTexture, char mOrientation )
{
    m_orientations[FACE_BOTTOM_ID] = mOrientation;
    m_side[FACE_BOTTOM_ID] = mTexture;
}
void glFaceBox::apply_front( Texture* mTexture, char mOrientation )
{
    m_orientations[FACE_FRONT_ID] = mOrientation;
    m_side[FACE_FRONT_ID] = mTexture;
}
void glFaceBox::apply_back( Texture* mTexture, char mOrientation )
{
    m_orientations[FACE_BACK_ID] = mOrientation;
    m_side[FACE_BACK_ID] = mTexture;
}
void glFaceBox::apply_left( Texture* mTexture, char mOrientation )
{
    m_orientations[FACE_LEFT_ID] = mOrientation;
    m_side[FACE_LEFT_ID] = mTexture;
}
void glFaceBox::apply_right( Texture* mTexture, char mOrientation )
{
    m_orientations[FACE_RIGHT_ID] = mOrientation;
    m_side[FACE_RIGHT_ID] = mTexture;
}

// Show the texture just on 1 side ()
/*void glFaceBox::wrap_3_sides( )
{
    
}
void glFaceBox::wrap_4_sides( )
{
    
}
void glFaceBox::wrap_5_sides( )
{
    
}
void glFaceBox::wrap_6_sides()
{
    
}*/

void glFaceBox::gl_register(  )
{
    glAtom::gl_register();       // Base class
    
    for (int s=0; s<6; s++)
        if (m_side[s])
        {
            m_side[s]->generate_TBO();
            m_side[s]->generate_texture_coords(m_orientations[s]);
            m_side[s]->generate_VBOTexCoords();
        }
    //generate_texture_coords_4_side_stretch();
}

/*
    Top, front, bottom, back sides.
 */
GLuint glFaceBox::generate_texture_coords_4_side_stretch( )
{
    float hdh = 1.0/(height+depth+height+depth);
    float hwh = 1.0/(width);
    const float fs = 1.0;
    struct stTextCoord tc;
    
    // U                                            V
    tc.u = hwh*height;          tc.v = hdh*height;            m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*height;          tc.v = hdh*(height+depth);    m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*(height+width);  tc.v = hdh*(height+depth);    m_texture->m_TexCoords.push_back( tc );
    tc.u = hwh*(height+width);  tc.v = hdh*height ;           m_texture->m_TexCoords.push_back( tc );
    
    tc.u = 0.0;                 tc.v = 0.0;         m_texture->m_TexCoords.push_back( tc );
    tc.u = 0.0;                 tc.v = fs;          m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;                  tc.v = fs;          m_texture->m_TexCoords.push_back( tc );
    tc.u = fs;                  tc.v = 0.0;         m_texture->m_TexCoords.push_back( tc );
    return m_texture->m_TexCoords.size()/2.;
}


void glFaceBox::draw_body()
{
    //Make the new VBO active. Repeat here incase changed since initialisation
    glBindBuffer    (GL_ARRAY_BUFFER, 		  m_VBO	);
    glBindBuffer    (GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    
    glVertexPointer (3, GL_FLOAT, 		  sizeof(struct Vertex), NULL );
    glColorPointer  (4, GL_UNSIGNED_BYTE, sizeof(struct Vertex), (GLvoid*)(offsetof(struct Vertex,color)));
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY  );

    int offset = 0;
    for (int s=0; s<6; s++)
    {
        if (m_side[s])
        {
            m_side[s]->draw();
            glDrawElements(GL_QUADS,(int)4, GL_UNSIGNED_INT,
                           (GLvoid*)(offset*sizeof(GL_UNSIGNED_INT)));
            m_side[s]->after_draw();
        }
        else
            glDrawElements(GL_QUADS, (int)4, GL_UNSIGNED_INT,
                           (GLvoid*)(offset*sizeof(GL_UNSIGNED_INT)));
        offset += 4;
    }
}

/* The problem with putting different images on each face:
 a) Every vertex has to have a corresponding TexCoord.
 
 the order of vertices must match the order of the TexCoords.
 
 However, the order of the vertices wont match the order needed by GL_QUADS.
 the index buffer fixes this.
 
 When doing the front (for instance),  we are using vertex (0,1,5,4)
 And the TexCoords are (0,1,2,3, 4,5,6,7)
 
 so 0=<0,0>;   1=<1,0>;  2=<1,1>;  3=<1,0>
 
 How do we fix this, b/c loading the texture, it would not know the set (0,1,5,4).
 
 Best way really is to store duplicates of the vertices for all 6 faces.
 This would allow normals for all faces too.
 
 A whole new class needed?  FaceBox created.
 We need 24 TexCoords too.
 
 */
void glFaceBox::print_info()
{
    printf("glFaceBox()  <x,y,z> = <%6.3f %6.3f %6.3f> \n",
           m_x,m_y,m_z );
    printf(" width=%6.3f;  height=%6.3f; depth=%6.3f> \n",
           width, height, depth );
}


/* this does a */
GLuint glFaceBox::generate_texture_coords( int mSide )
{
    // Texture coords [0.0..1.0]   1.0 represents the far edge of the image.
    // Pretty sure we need 1 coordinate for each vertex.
    //   So if we're just doing 1 face.  It should mean just 4 text coords.
    //m_NumTexCoords = 8;
    /* the texCoords have to be all in 1 array.  
        There has to be 24 of them to match the vertices!
        so we combine them into m_TexCoords
     */
    struct stTextCoord tc;
    for (int s=0; s<6; s++)
    {
        if (m_side[s]) {
            m_side[s]->generate_texture_coords();
            m_TexCoords.push_back( m_side[s]->m_TexCoords[0]);
            m_TexCoords.push_back( m_side[s]->m_TexCoords[1]);
            m_TexCoords.push_back( m_side[s]->m_TexCoords[2]);
            m_TexCoords.push_back( m_side[s]->m_TexCoords[3]);
        }
        else
        {
            tc.u = 0.0;   tc.v = 0.0;    m_TexCoords.push_back ( tc );     // 4 vertices Bottom
            tc.u = 0.0;   tc.v = 1.0;    m_TexCoords.push_back ( tc );
            tc.u = 1.0;   tc.v = 1.0;    m_TexCoords.push_back ( tc );
            tc.u = 1.0;   tc.v = 0.0;    m_TexCoords.push_back ( tc );
        }
    }
    return 0;
}


