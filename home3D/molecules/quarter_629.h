//
//  quarter_629.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/12/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__quarter_629__
#define __home3D__quarter_629__

#include <stdio.h>
#include "dwelling.h"
#include "glMolecule.h"


class glUpstairs : public glDwellingLevel
{
public:
    glUpstairs ();
    ~glUpstairs();

    void            create_vertical_walls   ( );
    void            create_horizontal_walls ( );
    virtual void    add_light_switches      ( );
    virtual void    create_components       ( );
};

class glBasement : public glDwellingLevel
{
public:
    glBasement ();
    ~glBasement();
    
    void            create_vertical_walls   ( );
    void            create_horizontal_walls ( );
    virtual void    add_light_switches      ( );
    virtual void    create_components       ( );
    
};


class glOurHouse : public glMolecule
{
public:
    glOurHouse ( );
    ~glOurHouse( );
    
    virtual void    create_components       ( );
    
    glUpstairs* up;
    glBasement* bm;
    glStairway* sw;

};


#endif /* defined(__home3D__quarter_629__) */
