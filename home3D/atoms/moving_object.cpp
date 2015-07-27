//
//  moving_object.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/27/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "moving_object.h"
#include "all_objects.h"
#include "physical_world.h"



glMovingObject::glMovingObject ( )
: m_net_forces(3),
m_acceleration("accel",    3),
m_velocity    ("velocity", 3),
m_position    ("position", 3),

m_R_net_torques (3),
m_R_acceleration(3),
m_R_velocity    (3),
m_R_position    (3)
{
    
}

glMovingObject::~glMovingObject( )
{
    
}

void glMovingObject::clear_net_forces()
{
    m_net_forces[0] = 0;
    m_net_forces[1] = 0;
    m_net_forces[2] = 0;
    
    m_R_net_torques[0] = 0;
    m_R_net_torques[1] = 0;
    m_R_net_torques[2] = 0;
}
// net forces / inertia
// All forces should have been added via add_force() prior to calling this function!
void glMovingObject::f_ma( )
{
    float inv_inertia = 1/Inertia;
    m_acceleration = m_net_forces * inv_inertia;
    m_velocity += m_acceleration;
    m_position += m_velocity;
    
    m_R_acceleration = m_R_net_torques * inv_inertia;
    m_R_velocity    += m_R_acceleration;
    m_R_position    += m_R_velocity;
}

void glMovingObject::update_rotation( )
{
    m_R_velocity += m_R_acceleration;
    m_R_position += m_R_velocity;
}

void glMovingObject::update_position()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
}

void glMovingObject::add_force( MathVector mNewForce )      // acting on the object.
{
    m_net_forces += mNewForce;
}

