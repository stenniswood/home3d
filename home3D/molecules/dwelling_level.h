//
//  dwelling_level.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/7/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__dwelling_level__
#define __home3D__dwelling_level__

#include <stdio.h>
#include "Map2D.hpp"
#include "full_wall.h"
#include "gl_object.hpp"
#include "glMolecule.h"
#include "floor.h"
#include "glComputableWall.h"


struct room {
    float sx;
    float sz;
    float ex;
    float ez;
    string name;
};

/* One level of the dwelling.  glDwelling will be multiple levels. */
class glDwellingLevel  : public glMolecule
{
public:
    glDwellingLevel ();
    ~glDwellingLevel();

    float   get_max_x       ();
    float   get_max_z       ();
    
    void    mirror_image_x( );
    void    mirror_image_z( );    
    
    void             create_floor();        // to cover all.
    glDwellingLevel* create_copy();
    glDwellingLevel* create_mirror_image();
    
    void            generate_exterior_walls();
    
    void            update_line_info   ( );      // wall origins and vectors.
    void            create_room        ( float width, float length );
    glDoor*         map_wall_coord     ( MathVector mWC );

    virtual bool    evaluate_collision( glSphere* mOther );
    
    // FOR 2D Drawing (and maze solving):
    void            create_map         ( glMap2D& mMap );

    // Not implemented yet... but not difficult.
    vector<Texture*>        m_door_textures;     // hold here instead of each door for memory savings.
    
    glFloor*                m_floor;
    vector<glFullWall*>     m_fwalls;
  //  vector<glComputeableWall*>     m_ext_walls;
    vector<glObject*>       m_furniture;
    vector<glObject*>       m_things;
};

// Find_object ( "asdfs", "door", true );
// Find_object ( "asdfs", "window", true );


#endif /* defined(__home3D__dwelling_level__) */
