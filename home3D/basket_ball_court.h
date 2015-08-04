//
//  basket_ball_court.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/30/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__basket_ball_court__
#define __home3D__basket_ball_court__

#include <stdio.h>
#include "dwelling_level.h"


class glBackboard : public glMolecule
{
public:
    glBackboard();
    ~glBackboard();
    
    virtual void       create_components( );
    
    
};

class glBasketballCourt : public glDwellingLevel
{
public:
    glBasketballCourt ();
    ~glBasketballCourt();
    
    void            select_floor( int mIndex );
    
    void            create_floor   ( );
    void            create_baskets ( );
    
    virtual void    add_light_switches      ( );
    virtual void    create_components       ( );
    

};



#endif /* defined(__home3D__basket_ball_court__) */
