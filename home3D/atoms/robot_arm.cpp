//
//  robot_arm.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 2/22/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include "all_objects.h"
#include "robot_arm.hpp"



glArm::glArm( )
: m_upper_arm( UPPER_ARM_LENGTH ),
  m_fore_arm ( FORE_ARM_LENGTH )
{
    Initialize();
}

void glArm::Initialize( )
{
    m_left_arm              = false;
    m_upper_arm.m_color     = 0xFFDF9000;
    m_upper_arm.m_ends_color= 0xFFDF9000;
    m_fore_arm.m_color      = 0xFFD0902F;
    m_fore_arm.m_ends_color = 0xFFD0902F;

    m_shoulder_angle        = 20.0;
    m_shoulder_rotate_angle = 0.0;
    m_elbow_angle           = 30.0;
    m_wrist_angle           = 0.0;
    m_wrist_rotate_angle    = 0.0;
}

void glArm::create_components( )
{
    m_upper_arm.setup();
    m_fore_arm.setup ();
    m_gripper.setup  ();

    m_components.push_back( &m_upper_arm );
    m_components.push_back( &m_fore_arm  );
    m_components.push_back( &m_gripper   );

    float offset = m_upper_arm.m_extrusion_length + FORK_CENTER_LENGTH;
    m_upper_arm.add_offset( 0, -offset, 0 );    // ie hold at top

    // Want it to pivot at the elbow :
    offset = m_fore_arm.m_extrusion_length + FORK_CENTER_LENGTH;
    m_fore_arm.add_offset( 0, -offset, 0 );    // ie hold fore arm at its top
    
    offset += TONGUE_CENTER_LENGTH;
    m_upper_arm.relocate( 0.0,  0.,    0.0 );
    m_fore_arm.relocate ( 0.0,  -UPPER_ARM_LENGTH,  0.0 );

    position_gripper();
}

// How to find the end of the arm?
// What coordinates to put into?
void glArm::position_gripper()
{
/* has no bearing since drawn with draw_body() skipping translation!
 m_gripper.relocate( 50, 5, 50 ); */
    
    m_gripper.m_x_angle = 0.0;
    m_gripper.m_y_angle = 0.0;
    m_gripper.m_z_angle = 0.0;
}

bool glArm::evaluate_angles()
{
    if (m_shoulder_rotate_angle > MAX_SHOULDER_ROTATE_ANGLE)  		return false;
    if (m_shoulder_rotate_angle < MIN_SHOULDER_ROTATE_ANGLE)  		return false;

    if (m_upper_arm_rotate_angle > MAX_UPPERARM_ROTATE_ANGLE)  		return false;
    if (m_upper_arm_rotate_angle < MIN_UPPERARM_ROTATE_ANGLE)  		return false;
    
    if (m_shoulder_angle > MAX_SHOULDER_SWING_ANGLE)  		return false;
    if (m_shoulder_angle < MIN_SHOULDER_SWING_ANGLE)  		return false;
    
    if (m_elbow_angle > MAX_ELBOW_ANGLE)                    return false;
    if (m_elbow_angle < MIN_ELBOW_ANGLE)                    return false;
    
    if (m_wrist_angle > MAX_WRIST_ANGLE)                    return false;
    if (m_wrist_angle < MIN_WRIST_ANGLE)                    return false;
 
    if (m_wrist_rotate_angle > MAX_WRIST_ROTATE_ANGLE)  	return false;
    if (m_wrist_rotate_angle < MIN_WRIST_ROTATE_ANGLE)  	return false;

    return true;
}

bool glArm::set_shoulder_angle( float mAngle )
{
    if (mAngle > MAX_SHOULDER_SWING_ANGLE)  return false;
    if (mAngle < MIN_SHOULDER_SWING_ANGLE)  return false;
    if (m_left_arm)
        m_shoulder_angle = mAngle;
    else
        m_shoulder_angle = -mAngle;
    return true;
}

bool glArm::set_shoulder_rotate_angle( float mAngle )
{
    if (mAngle > MAX_SHOULDER_ROTATE_ANGLE)  return false;
    if (mAngle < MIN_SHOULDER_ROTATE_ANGLE)  return false;
    m_shoulder_rotate_angle = mAngle;
    return true;
}
bool glArm::set_upper_arm_rotate_angle( float mAngle )
{
    if (mAngle > MAX_UPPERARM_ROTATE_ANGLE)  return false;
    if (mAngle < MIN_UPPERARM_ROTATE_ANGLE)  return false;
    if (m_left_arm)
        m_upper_arm_rotate_angle = mAngle;
    else
        m_upper_arm_rotate_angle = -mAngle;
    return true;
}

float glArm::get_shoulder_angle       (  )
{
    if (m_left_arm)
        return m_shoulder_angle;
    else
        return -m_shoulder_angle;
}
float glArm::get_shoulder_rotate_angle( )
{
    return m_shoulder_rotate_angle;
}
float glArm::get_upper_arm_rotate_angle( )
{
    if (m_left_arm)
        return m_upper_arm_rotate_angle;
    else
        return -m_upper_arm_rotate_angle;    
}



bool glArm::set_elbow_angle         ( float mAngle )
{
    if (mAngle > MAX_ELBOW_ANGLE)  return false;
    if (mAngle < MIN_ELBOW_ANGLE)  return false;
    m_elbow_angle = mAngle;
    return true;
}
bool glArm::set_wrist_angle         ( float mAngle )
{
    if (mAngle > MAX_WRIST_ANGLE)  return false;
    if (mAngle < MIN_WRIST_ANGLE)  return false;
    m_wrist_angle = mAngle;
    return true;
}
bool glArm::set_wrist_rotate_angle ( float mAngle )
{
    if (mAngle > MAX_WRIST_ROTATE_ANGLE)  return false;
    if (mAngle < MIN_WRIST_ROTATE_ANGLE)  return false;
    m_wrist_rotate_angle = mAngle;
    return true;
}

bool glArm::increase_shoulder_angle	( float mRelativeAngle )
{
    float  angle = m_shoulder_angle + mRelativeAngle;
    if (m_left_arm) {
        if (angle > MAX_SHOULDER_SWING_ANGLE)  angle = MAX_SHOULDER_SWING_ANGLE;
        if (angle < MIN_SHOULDER_SWING_ANGLE)  angle = MIN_SHOULDER_SWING_ANGLE;
        m_shoulder_angle = angle;
    } else {
        angle = -m_shoulder_angle + mRelativeAngle;
        if (angle > MAX_SHOULDER_SWING_ANGLE)  angle = MAX_SHOULDER_SWING_ANGLE;
        if (angle < MIN_SHOULDER_SWING_ANGLE)  angle = MIN_SHOULDER_SWING_ANGLE;
        m_shoulder_angle = -angle;
    }
    transfer_angles();
    return true;
}
bool glArm::increase_shoulder_rotate_angle( float mRelativeAngle )
{
    m_shoulder_rotate_angle += mRelativeAngle;
    if (m_shoulder_rotate_angle > MAX_SHOULDER_ROTATE_ANGLE)  m_shoulder_rotate_angle = MAX_SHOULDER_ROTATE_ANGLE;
    if (m_shoulder_rotate_angle < MIN_SHOULDER_ROTATE_ANGLE)  m_shoulder_rotate_angle = MIN_SHOULDER_ROTATE_ANGLE;
    transfer_angles();
    return true;
}
bool glArm::increase_upper_arm_rotate_angle( float mRelativeAngle )
{
    m_upper_arm_rotate_angle += mRelativeAngle;
    if (m_upper_arm_rotate_angle > MAX_UPPERARM_ROTATE_ANGLE)
        m_upper_arm_rotate_angle = MAX_UPPERARM_ROTATE_ANGLE;
    if (m_upper_arm_rotate_angle < MIN_UPPERARM_ROTATE_ANGLE)
        m_upper_arm_rotate_angle = MIN_UPPERARM_ROTATE_ANGLE;
    transfer_angles();
    return true;
}

bool glArm::increase_elbow_angle	( float mRelativeAngle )
{
    m_elbow_angle += mRelativeAngle;
    if (m_elbow_angle > MAX_ELBOW_ANGLE)  m_elbow_angle = MAX_ELBOW_ANGLE;
    if (m_elbow_angle < MIN_ELBOW_ANGLE)  m_elbow_angle = MIN_ELBOW_ANGLE;
    return true;
}
bool glArm::increase_wrist_angle( float mRelativeAngle )
{
    m_wrist_angle += mRelativeAngle;
    if (m_wrist_angle > MAX_WRIST_ANGLE)  m_wrist_angle = MAX_WRIST_ANGLE;
    if (m_wrist_angle < MIN_WRIST_ANGLE)  m_wrist_angle = MIN_WRIST_ANGLE;
    return true;
}
bool glArm::increase_wrist_rotate_angle( float mRelativeAngle )
{
    m_wrist_rotate_angle += mRelativeAngle;
    if (m_wrist_rotate_angle > MAX_WRIST_ROTATE_ANGLE)  m_wrist_rotate_angle = MAX_WRIST_ROTATE_ANGLE;
    if (m_wrist_rotate_angle < MIN_WRIST_ROTATE_ANGLE)  m_wrist_rotate_angle = MIN_WRIST_ROTATE_ANGLE;
    return true;
}

float glArm::get_arm_length  ()
{
    float length = m_upper_arm.m_joint_length + m_fore_arm.m_joint_length;
    return length;
}


#include <math.h>

static float degrees( float angle )
{  return angle*180./M_PI; }

/*
   Incoming coordinates must be in robot coords
 
 Assume isoceles triangle (ie. the upper and fore arm lengths are the same)
 mX is robot width axis.
 mZ is fore/aft axis.
 mY is up/down axis.
*/
bool glArm::inverse_xyz( float mX, float mY, float mZ )
{
    float deltaX = (mX - m_x);      // x is robot width!
    float deltaY = (mY - m_y);
    float deltaZ = (mZ - m_z);      // fore/aft
    float distance = sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ );
    float elbow_angle;

    float arm_length = (m_upper_arm.m_joint_length + m_fore_arm.m_joint_length);
    if (distance >= arm_length)
        return false;
    else
        elbow_angle = asin( (distance/2.) / m_upper_arm.m_joint_length) * 2.;

    elbow_angle = degrees( elbow_angle );
    m_elbow_angle    = 180. - elbow_angle;

    float phi        = 90. - elbow_angle/2.;
    float elevation  = degrees( atan2(deltaY,deltaZ) );
    m_shoulder_angle = 90. + elevation - phi;
    
//    printf("mX=%6.3f; mY=%6.3f; mZ=%6.2f; dist=%6.3f   m_elbow=%6.2f; shoulder=%6.3f\n",
//            mX, mY,  mZ, distance, m_elbow_angle, m_shoulder_angle );
    return true;
    // x is robot width
    // m_shoulder_rotate_angle = degrees(atan2( deltaX, deltaZ ));
}

void glArm::forward_xyz( float *mX, float *mY, float *mZ )
{
    glm::mat4 ua_bm = m_upper_arm.get_body_matrix();
    glm::mat4 fa_bm = m_fore_arm.get_body_matrix();
    glm::mat4 bm = ua_bm * fa_bm;
    
    glm::vec4 wrist_loc;
    wrist_loc[0] = 0.0;  wrist_loc[1] = m_fore_arm.m_joint_length;  wrist_loc[2] = 0.0;     wrist_loc[3] = 1.0;
    glm::vec4 arm_coord;
    arm_coord = bm * wrist_loc;
}

// forward calculation :
MathVector glArm::get_wrist_position       (  )
{
    glm::mat4 ua_bm = m_upper_arm.get_body_matrix();
    glm::mat4 fa_bm = m_fore_arm.get_body_matrix();
    glm::mat4 bm = ua_bm * fa_bm;
    
    glm::vec4 wrist_loc;
    wrist_loc[0] = 0.0;  wrist_loc[1] = m_fore_arm.m_joint_length;  wrist_loc[2] = 0.0;  wrist_loc[3] = 1.0;
    
    wrist_loc = bm * wrist_loc;
    MathVector arm_coord(3);
    arm_coord[0] = wrist_loc[0];
    arm_coord[1] = wrist_loc[1];
    arm_coord[2] = wrist_loc[2];
    return arm_coord;
}

// Inverse equations :
void glArm::set_wrist_position       ( MathVector& mArmCoord )
{
    
}



// Call repetitively to scan:
void glArm::move_arms_up_down( float mForeAft )
{
    static float y = 0.0;
    static float x = 0.0;
    static int   state = 0.0;
    const float maxY  = 12.0;
    const float maxX  = 12.0;
    const float increment = 0.5;
    switch ( state )
    {
    case 0: if (y < maxY)  {
            y += increment;
            inverse_xyz( m_x, m_y+y, mForeAft );
        } else {
            state = 1;
        }
        break;
    case 1: if (y > -maxY) {
            y -= increment;
            inverse_xyz( m_x, m_y+y, mForeAft );
        } else {
            state = 2;
        }
        break;
    case 2: if (x < maxX) {
            x  += increment;
            inverse_xyz( m_x, m_y+y, mForeAft+x );
        } else {
            state = 3;
        }
        break;
    case 3: if (x > 0) {
            x  -= increment;
            inverse_xyz( m_x, m_y+y, mForeAft+x );
        } else {
            state = 0;
        }
            break;

        default: break;
    }
}
using namespace glm;

void glArm::transfer_angles  ()
{
    m_upper_arm.m_x_angle = -m_shoulder_rotate_angle;
    m_upper_arm.m_y_angle = m_shoulder_angle;
    m_upper_arm.m_z_angle = m_upper_arm_rotate_angle;
    
    m_fore_arm.m_x_angle = -m_elbow_angle;
    m_fore_arm.m_z_angle = m_wrist_rotate_angle;
    
    m_gripper.m_x_angle  =  m_wrist_angle;
}

void glArm::draw_body ( )
{
    glPushMatrix();
    transfer_angles();
    
    glTranslatef( m_upper_arm.m_x, m_upper_arm.m_y, m_upper_arm.m_z );
    float rx = m_upper_arm.m_x_angle;
    float ry = m_upper_arm.m_y_angle;
    float rz = m_upper_arm.m_z_angle;
    m_upper_arm.m_y_angle = 0.;
    m_upper_arm.m_z_angle = 0.;
    
    MathVector lx(4);  lx.set_xyz(1.0, 0.0, 0.0 );  lx[3] = 0.;
    MathVector ly(4);  ly.set_xyz(0.0, 1.0, 0.0 );  ly[3] = 0.;
    MathVector lz(4);  lz.set_xyz(0.0, 0.0, 1.0 );  lz[3] = 0.;

    glRotatef   ( rx,   1.0, 0.0, 0.0 );
    glRotatef   ( ry, lz[0], lz[1], lz[2] );
    glRotatef   ( rz, ly[0], ly[1], ly[2] );
    m_upper_arm.draw_body();
    
    
    glTranslatef( m_fore_arm.m_x, m_fore_arm.m_y, m_fore_arm.m_z );
    glRotatef   ( m_fore_arm.m_x_angle,    1.0, 0.0, 0.0 );
    m_fore_arm.draw_body();

    glTranslatef( 0.0, -m_fore_arm.m_joint_length+0, 0.0 );
/*    m_gripper.m_x_angle = 0.;
    m_gripper.m_y_angle = 0.;
    m_gripper.m_z_angle = 0.;*/
    glRotatef   ( m_wrist_angle+180,    1.0, 0.0, 0.0 );
    glRotatef   ( m_wrist_rotate_angle, 0.0, 1.0, 0.0 );
    m_gripper.draw_body();
    
    glPopMatrix();
}

//    MathVector mly = map_coords(ly)
//    quat MyQuaternion;
//    MyQuaternion = quat(0.0, m_x_angle, m_y_angle, m_z_angle );
//    mat4 RotationMatrix = quaternion::toMat4(MyQuaternion);
//    glMultMatrixf(RotationMatrix);
