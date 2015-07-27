//
//  walking_robot.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/21/15.
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

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glWalkingRobot::glWalkingRobot( )
//:glMovesRobot()
{
    m_left_step_index  = 0;
    m_right_step_index = 0;
    m_phase         = 0;
    m_stance_left   = true;
}


// Intermediate stance leg - knee bent (flat footed forward).
void glWalkingRobot::walk_phase_iv(  )
{
    
}


// Intermediate (1/2 way) stance leg straight.  Swing leg toe lift knee bent.
void glWalkingRobot::walk_phase_v( struct stFootPosition mstancefp )
{
    glLeg* stance = get_stance_leg();
    glLeg* swing  = get_swing_leg ();
    
    // Move body directly above the stance leg
    //      The hip above the heel.
    // m_x,m_z are in the center of the torso.
    m_y = stance->get_length() + m_torso.height/2.0 + mstancefp.heel[1];

    printf("  fp: %5.3f, %5.3f, %5.3f\n", mstancefp.heel[0], mstancefp.heel[1], mstancefp.heel[2]);
    float x = mstancefp.heel[0];  //mStance.m_foot.m_world_coords.heel[0];
    float z = mstancefp.heel[2];  //mStance.m_foot.m_world_coords.heel[2];
    float m_y_angle_rad    = glm::radians(m_y_angle-90.);
    float m_y_angle_rad_fa = glm::radians(m_y_angle);

    float heel_offset_x    = 2.*cos(m_y_angle_rad_fa);
    float heel_offset_z    = 2.*sin(m_y_angle_rad_fa);
    
    m_x = x - stance->m_x - heel_offset_x;
    m_z = z - stance->m_z - heel_offset_z;
    
    glm::mat4 mt = get_body_matrix_inverse();
    
//        m_x = x - m_torso.width/2. * sin(m_y_angle_rad) + heel_offset_x;
//        m_z = z - m_torso.width/2. * cos(m_y_angle_rad) + heel_offset_z;
    
    stance->set_hip_angle   ( 0.0);
    stance->set_knee_angle  ( 0.0);
    stance->set_ankle_angle ( 0.0);
    
    // Swing Leg knee/ankle bend.
    swing->set_hip_angle   (  5. );
    swing->set_knee_angle  ( 20. );
    swing->set_ankle_angle ( -20.);
}

void glWalkingRobot::goto_start_of_path( )
{
    printf("\n\nStarting path over!\n");
    if (m_route.m_vertices.size()==0)  return; // No path yet!
    
    struct Vertex v1;
    struct Vertex v2;
    v1 = m_route.m_vertices[0];
    v2 = m_route.m_vertices[1];
    place_over_vertex( v1, v2 );    // change this to use above function!

    // Adjust body angle.
    m_y_angle          = m_route.get_angle_degrees( 0 );
    m_stance_left = false;
    m_left_step_index  = 0;     // 0 is the standing position.  1 is the first step.
    m_right_step_index = 0;
    m_at_end_of_route  = false;
}

void glWalkingRobot::add_standing_feet_position(int mIndex)       // for start and end of path.
{
    place_over_vertex( m_route.m_vertices[mIndex], m_route.m_vertices[mIndex+1] );
    angle_body_vertex( mIndex, mIndex+1 );
    
    struct stFootPosition foot;
    stand();
    calc_world_left_foot_positions ();
    calc_world_right_foot_positions();
    get_left_foot_location(&foot);
    m_route.m_left_steps.push_back( foot );

    get_right_foot_location(&foot);
    m_route.m_right_steps.push_back( foot );
}

//void  get_left_foot_print          ( int mVertexIndex );       // for start and end of path.
//void  get_right_foot_print         ( int mVertexIndex );       // for start and end of path.

void glWalkingRobot::get_foot_print( int mVertexIndex, bool mLeft )       // for start and end of path.
{
    m_route.colorize_center(mVertexIndex);
    
    // We'll put the foot print parallel to the path.  So we need 2 path points to find the angle.
    MathVector rr = m_route.compute_slope( mVertexIndex );

    // If all path points really are 2 inches apart, can eliminate sqrt and just / 2.0
    rr.unitize();
    MathVector perp = rr.get_perp_yz();
    float path_center_to_heel = get_feet_separation()/2.;
    if (mLeft==false)
        path_center_to_heel = -path_center_to_heel;

    MathVector heel = m_route.get_perpendicular(mVertexIndex, path_center_to_heel);
    
    // HEEL :
    struct stFootPosition fp;
    fp.heel[0] = heel[0];
    fp.heel[1] = heel[1];
    fp.heel[2] = heel[2];
    fp.angle[1]= atan2( rr[0], rr[2] );
    
    // TOE :
    float multiplier = ( m_left_leg.m_foot.m_foot_length );
    fp.toe[0] = heel[0] + rr[0] * multiplier;
    fp.toe[1] = heel[1];
    fp.toe[2] = heel[2] + rr[2] * multiplier;
    if (mLeft)
        m_route.m_left_steps.push_back ( fp );
    else
        m_route.m_right_steps.push_back( fp );
}
void glWalkingRobot::compute_robot_steps( float mStride, bool mLeftFootFirst )
{
    struct Vertex v;
    v.position[1] = 0.0;
    bool  left_foot_turn  = mLeftFootFirst;
    int   stride_vertices = round(mStride/2.0);
    
    // First step = vertex 7 (half stride)
    for (int i=7; i<m_route.m_vertices.size()-1; i+=stride_vertices)
    {
        get_foot_print( i, left_foot_turn );
        left_foot_turn = !left_foot_turn;
    }
}
void glWalkingRobot::walk_path( float mStride, bool mLeftFootFirst )
{
    // Start by standing in the m_start position
    add_standing_feet_position( 0 );
    
    // WALK TO DESTINATION :
    compute_robot_steps( mStride, true );
    // ONE EXTRA on the RIght Foot here!!
    
    // END STANDING POSITION :
    add_standing_feet_position( m_route.m_number_path_vertices-2 );
}

void glWalkingRobot::plan_steps(   )
{
    calc_world_feet_positions(  );                      // from current angles

    walk_path( 30., true );     //
    m_route.generate_steps_vertices();                  //
    m_route.gl_register();
    // m_components.push_back(&m_route);
    // done separately because route needs to reverse the rotations before drawing.  Absolute positioning only!

    goto_start_of_path();
}

void glWalkingRobot::walk_phase_vi     (  )      // Intermediate (1/2 way) stance leg straight.  Swing leg toe lift knee bent.
{
    
}

void glWalkingRobot::walk_phases(  )
{
/*    switch (m_phase)
    {
        case 0: // Heel strike
            place_feet_at( m_route.m_left_steps[m_left_step_index], m_route.m_right_steps[m_right_step_index], m_stance_left );
            m_phase = 1;
            break;
        case 1:
            m_phase = 2;
            break;
        case 2: // single support stance leg straight
            if (m_stance_left)
                walk_phase_v( m_route.m_left_steps[m_left_step_index] );
            else
                walk_phase_v( m_route.m_right_steps[m_left_step_index] );
            if (m_stance_left)
                m_right_step_index++;
            else
                m_left_step_index++;

            m_stance_left = !m_stance_left;
            m_phase = 0;
            break;
        case 3: m_phase = 0;
            break;
        default:
            break;
    }  */
}

glLeg* glWalkingRobot::get_swing_leg ( )
{
    if (m_stance_left==false)
        return &(m_left_leg);
    else
        return &(m_right_leg);
}
glLeg* glWalkingRobot::get_stance_leg()
{
    if (m_stance_left)
        return &(m_left_leg);
    else
        return &(m_right_leg);
}

void glWalkingRobot::take_step_backward( )
{
/*    // Assumed "and it's already backward mode" :
    if (m_stance_left) {
        m_left_step_index--;   if (m_left_step_index<0) m_left_step_index=0;
        printf("B Left:%5d (r=%d)\n", m_left_step_index, m_right_step_index );
        m_route.print_step( m_left_step_index, true );
        place_feet_at( m_route.m_left_steps[m_left_step_index], m_route.m_right_steps[m_right_step_index], m_stance_left);
        //place_foot_at( mRoute.m_left_steps[m_left_step_index], m_left_leg, m_right_leg );
    } else {
        m_right_step_index--;   if (m_right_step_index<0) m_right_step_index=0;
        printf("B Right:(l=%d) r=%5d\n", m_left_step_index, m_right_step_index );
        m_route.print_step(m_right_step_index, false);
        place_feet_at( m_route.m_left_steps[m_left_step_index], m_route.m_right_steps[m_right_step_index], m_stance_left);
        //place_foot_at( mRoute.m_right_steps[m_right_step_index], m_right_leg, m_left_leg  );
    } */
}

void glWalkingRobot::take_step_forward (  )
{
  /*  if (m_at_end_of_route)
        goto_start_of_path();

    calc_world_left_foot_positions ();      // left foot position.  only 1.
    calc_world_right_foot_positions();
    
    printf("Foot index: l,r = (%d, %d)\n", m_left_step_index, m_right_step_index );
    m_route.print_step( m_left_step_index,  true );
    m_route.print_step( m_right_step_index, false );
    
    walk_phases( );

    if ((m_left_step_index>=m_route.m_left_steps.size())  &&
        (m_right_step_index>=m_route.m_right_steps.size()))
        m_at_end_of_route = true;
   */
}

void glWalkingRobot::take_step( bool mForward )
{
    if (mForward)
    {
        if (m_last_direction_forward==false)
        {
            move_to_stand( !m_stance_left);
        } else
            take_step_forward();
    } else {
        if (m_last_direction_forward==true)
        {
            move_to_stand( !m_stance_left );
        } else
            take_step_backward();
    }
    m_last_direction_forward = mForward;
}

// Add current body position to the sequencer.
void glWalkingRobot::add_to_sequence()
{
    struct stBodyPosition bp;
    extract_robot_position(&bp);
    m_sequence.push_back( bp );
}

void glWalkingRobot::flat_footed( glFoot&  mfoot )
{
    // For starters - adjust world coordinates in the foot.
    // see foot
    // Then see about angles
}

/*void glWalkingRobot::interpolate_phase(glRouteWithFeet mRoute)
{
    switch(m_walk_phase_left)
    {
        case 0: // Heel strike:
            take_step_forward();
            add_to_sequence();
            break;
        case 1: // knee bend (ankle floor)
            add_to_sequence();
            break;
        case 2: // leg straight
            add_to_sequence();
            break;
        case 3: // trailing
            add_to_sequence();
            break;
        case 4: // toe off
            add_to_sequence();
            break;
        default:
            break;
    }
}*/

/*  Brings forward leg back together
 This is different from just putting legs down straight.
 It moves the specified foot (forward or backward) to
 meet the other foot.
 */
void glWalkingRobot::move_to_stand( bool mMoveLeftLeg )
{
    printf("Moving to Standing position\t");
    
    // Calculate where to put it
    struct stFootPosition stationary;
    struct stFootPosition new_loc   ;
    
    if (mMoveLeftLeg)
    {
        printf("Left Leg\n");
        // Get right leg.  Extrapolate acros width of torso.
        get_right_foot_location( &stationary );
        new_loc.heel[1] = stationary.heel[1];
        new_loc.toe[1]  = stationary.toe[1];
    } else {
        printf("Right Leg\n");
        get_left_foot_location( &stationary );
        
        new_loc.heel[1] = stationary.heel[1];
        new_loc.toe[1]  = stationary.toe[1];
        // Calc slope to torso center:
    }
    printf("stationary heel at %6.3f %6.3f %6.3f \n", stationary.heel[0], stationary.heel[1], stationary.heel[2] );
    printf("stationary toe  at %6.3f %6.3f %6.3f \n", stationary.toe[0],  stationary.toe[1],  stationary.toe[2]  );
    
    // Calc slope to torso center:
    float run  = stationary.heel[0] - stationary.toe[0];
    float rise = stationary.heel[2] - stationary.toe[2];
    float magnitude = sqrt(run*run + rise*rise);
    run  /= magnitude;
    rise /= magnitude;
    
    new_loc.heel[0] = stationary.heel[0] - rise* m_torso.width;
    new_loc.heel[2] = stationary.heel[2] + run * m_torso.width;
    
    new_loc.toe[0] =  -(run  * magnitude - new_loc.heel[0]);
    new_loc.toe[2] =  -(rise * magnitude - new_loc.heel[2]);
    
    new_loc.angle[0] = stationary.angle[0];
    new_loc.angle[1] = stationary.angle[1];
    new_loc.angle[2] = stationary.angle[2];
    
    if (mMoveLeftLeg)
        place_foot_at(new_loc, m_left_leg, m_right_leg);
    else
        place_foot_at(new_loc, m_right_leg, m_left_leg);
    
    printf("new heel at %6.3f %6.3f %6.3f \n", new_loc.heel[0], new_loc.heel[1], new_loc.heel[2] );
    printf("new toe  at %6.3f %6.3f %6.3f \n", new_loc.toe[0],  new_loc.toe[1],  new_loc.toe[2]  );
}

const float HEAD_LOOK_INCREMENT = 5.0;
float head_delta = HEAD_LOOK_INCREMENT;

const int DELAYS = 4*3;  // called every 25ms = 0.2 seconds
void glWalkingRobot::update_animation( )
{
    static int delay_counter = DELAYS;
    delay_counter--;
    if (delay_counter)  return;
    delay_counter = DELAYS;

    // SCAN HEAD :
    static float head_angle = 0;
    head_angle += head_delta;
    if (head_angle>80.0)
        head_delta = -HEAD_LOOK_INCREMENT;
    if (head_angle<-80.0)
        head_delta = HEAD_LOOK_INCREMENT;
    m_head.look_left( head_angle );

    //       take_step();
}



void glWalkingRobot::draw_body()
{
    // Note: Here the ViewMatrix has already been translated and rotated to the robot frame.
    glMolecule::draw_body();
    
    glRotatef(-m_z_angle, 0.0, 0.0, 1.0 );
    glRotatef(-m_y_angle, 0.0, 1.0, 0.0 );
    glRotatef(-m_x_angle, 1.0, 0.0, 0.0 );
    glTranslatef( -m_x, -m_y, -m_z );
    m_route.draw();
}
