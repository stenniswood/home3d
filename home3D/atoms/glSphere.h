//
//  glSphere.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/2/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__glSphere__
#define __home3D__glSphere__

#include <stdio.h>
#include <vector>
#include "gl_atom.hpp"


class glSphere : public glAtom
{
public:
    glSphere    ( float radius, unsigned int rings, unsigned int sectors);
    ~glSphere   ( );
    
    void            load_texture         ( string mImageFilename );
    
    virtual void    generate_vertices   ( );
    virtual void 	generate_indices    ( );
    
    void            draw_primitive      ( );
    
    std::vector<GLfloat> m_normals;
    float 	m_radius;
    float   m_rings;
    float   m_sectors;
};



#endif /* defined(__home3D__glSphere__) */
