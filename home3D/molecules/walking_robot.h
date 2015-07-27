//
//  walking_robot.h
//  home3D
//
//  Created by Stephen Tenniswood on 6/21/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
// Squatting, kneeling, balancing 1 leg, dancing, sitting, laying, fallen on back, on front,
// getting up.  How to capture entire set?  Keep it simple.
 
// NOTE: Origin of the robot is the center of the Torso.

#ifndef __home3D__walking_robot__
#define __home3D__walking_robot__

#include <stdio.h>
#include "robot.hpp"
#include "robot_leg_piece.hpp"
#include "route_feet.hpp"
#include "robot_interpolator.h"



class glWalkingRobot : public glRobotInterpolator
{
public:
    glWalkingRobot( );
    virtual void            draw_body           ( );
    
    // SEQUENCING :
    void            take_step_backward      ( );
    void            take_step_forward       ( );
    void            take_step               ( bool mForward = true );
    void            goto_start_of_path      ( );

    void            move_to_stand           ( bool mMoveLeftLeg       );    // brings forward leg back together
    void            update_animation        ( );
    
    void            get_foot_print          ( int mVertexIndex, bool mLeft );       // for start and end of path.
    
    void            compute_robot_steps         ( float mStride, bool mLeftFootFirst );
    void            add_standing_feet_position  ( int mIndex);       // for start and end of path.
    void            walk_path                   ( float mStride, bool mLeftFootFirst );
    
    
    void            plan_steps         (                               );  // uses m_route
    void        walk_phase_iv      (                               );
    void        walk_phase_v       ( struct stFootPosition mstancefp );
    void        walk_phase_vi      (                               );  // Intermediate (1/2 way) stance leg straight.  Swing leg toe lift knee bent.
    void        walk_phases        (   );
    void            flat_footed        ( glFoot&  mfoot                );
//    void            interpolate_phase  ( glRouteWithFeet mRoute           );
    void            add_to_sequence         (                        );
    vector<struct stBodyPosition> m_sequence;

    glRouteWithFeet m_route;
    
    glLeg*          get_stance_leg( );
    glLeg*          get_swing_leg ( );

    int             m_phase;
    bool            m_stance_left;      // replace m_left_foot_turn (though opposite!)  turn means moving leg.
    
    int             m_left_step_index;
    int             m_right_step_index;
    bool            m_at_end_of_route;
};


#endif /* defined(__home3D__walking_robot__) */
