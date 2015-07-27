//
//  moving_object.h
//  home3D
//
//  Created by Stephen Tenniswood on 6/27/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__moving_object__
#define __home3D__moving_object__

#include <stdio.h>
#include "vector.hpp"


#define m_x m_position[0]
#define m_y m_position[1]
#define m_z m_position[2]

#define	m_x_angle m_R_position[0]
#define m_y_angle m_R_position[1]
#define m_z_angle m_R_position[2]


class glMovingObject
{
public:
    glMovingObject ( );
    ~glMovingObject( );

    virtual void    f_ma            ( );                        // net forces / inertia
    virtual void    update_position ( );
    virtual void    update_rotation ( );
    void            add_force       ( MathVector mNewForce );   // acting on the object.
    void            clear_net_forces( );
    
    // TRANSLATIONAL:
    MathVector m_net_forces;
    MathVector m_acceleration;
    MathVector m_velocity;
    MathVector m_position;

    // ROTATIONAL:
    float Inertia = 0.0;
    MathVector m_R_net_torques;
    MathVector m_R_acceleration;
    MathVector m_R_velocity;
    MathVector m_R_position;
    
    bool    is_active;
};




#endif /* defined(__home3D__moving_object__) */
