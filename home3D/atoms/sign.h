//
//  sign.h
//  home3D
//
//  Created by Stephen Tenniswood on 6/25/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__sign__
#define __home3D__sign__

#include <stdio.h>
#include "gl_atom.hpp"
#include "Texture.hpp"
#include "glMolecule.h"


class glSign : public glAtom
{
public:
    glSign(  );
    
    virtual void    generate_vertices       ( );
    virtual void 	setup                   ( );
    void            generate_vertices_colors( );
    virtual void	draw_primitive( );          // Override this with open gl commands.

    float 	 m_height;
    float 	 m_width;
};

class StreetSign : public glMolecule
{
public:
    StreetSign();
    ~StreetSign();
    
};

#endif /* defined(__home3D__sign__) */
