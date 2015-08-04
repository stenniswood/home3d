//
//  sports_bench.h
//  home3D
//
//  Created by Stephen Tenniswood on 8/3/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__sports_bench__
#define __home3D__sports_bench__

#include <stdio.h>
#include "glMolecule.h"


class glBench : public glMolecule
{
public:
    glBench ();
    ~glBench();
    
    void            create_vertical_walls   ( );
    void            create_horizontal_walls ( );
    virtual void    add_light_switches      ( );
    virtual void    create_components       ( );
    
    float m_length;
    float m_height;
    
};




#endif /* defined(__home3D__sports_bench__) */