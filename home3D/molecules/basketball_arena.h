//
//  basketball_arena.h
//  home3D
//
//  Created by Stephen Tenniswood on 8/3/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__basketball_arena__
#define __home3D__basketball_arena__

#include <stdio.h>
#include <vector>
#include "glMolecule.h"
#include "glSphere.h"
#include "walking_robot.h"

const float NBA_COURT_WIDTH  = 50*12;
const float NBA_COURT_LENGTH = 94*12;

const float CENTER_COURT_X = 0.0;
const float CENTER_COURT_Y = 94*12/2;

/* Origin is :
 */

class glArena : public glMolecule
{
public:
    glArena ();
    ~glArena();
    
    virtual void	setup            ( );
    
    void            create_vertical_walls   ( );
    void            create_stands           ( );
    void            create_stands_ends      ( );
    
    void            create_team1            ( );
    void            create_team2            ( );

    void            position_lineup_tunnel  ( );
    void            position_team_initial   ( );
    
    virtual void    add_light_switches      ( );
    virtual void    create_components       ( );
    
    vector<glSphere*>          m_balls;
    vector<glRobot*>    m_team1;
    vector<glRobot*>    m_team2;
    
};



#endif /* defined(__home3D__basketball_arena__) */
