//
//  robot_arm.h
//  home3D
//
//  Created by Stephen Tenniswood on 2/22/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__robot_arm__
#define __home3D__robot_arm__

#include <stdio.h>
//#include "all_objects.h"
//#include "robot_gripper.h"
#include "glMolecule.h"
#include "robot_leg_piece.hpp"
#include "robot_gripper.h"




const float MAX_SHOULDER_ROTATE_ANGLE = 180.0;          //
const float MIN_SHOULDER_ROTATE_ANGLE = -120.0;          //

const float MAX_SHOULDER_SWING_ANGLE = 100.0;   // UPPER ARM FORWARD/BACKWARD
const float MIN_SHOULDER_SWING_ANGLE =   0.0;

const float MAX_UPPERARM_ROTATE_ANGLE = +90.0;   // UPPER ARM FORWARD/BACKWARD
const float MIN_UPPERARM_ROTATE_ANGLE = -90.0;

const float MAX_ELBOW_ANGLE = 120.0;            //
const float MIN_ELBOW_ANGLE =   0.0;            //

const float MAX_WRIST_ANGLE =  75.0;
const float MIN_WRIST_ANGLE = -75.0;

const float MAX_WRIST_ROTATE_ANGLE =  90.0;
const float MIN_WRIST_ROTATE_ANGLE = -90.0;

const float UPPER_ARM_LENGTH  = 14.0;
const float FORE_ARM_LENGTH   = 14.0;


class glArm : public glMolecule
{
public:
    glArm( );
    void			Initialize       ( );
    void			create_components( );
    virtual void	draw_body        ( );
    
    bool			inverse_xyz              ( float mX,  float mY,  float mZ   );
    void			forward_xyz              ( float *mX, float *mY, float *mZ  );
    
    MathVector      get_wrist_position       (  );
    void            set_wrist_position       ( MathVector& mArmCoord );
    
    void            move_arms_up_down        ( float mX );

    bool			evaluate_angles          ( );

    bool			set_shoulder_angle       ( float mAngle );
    bool			set_shoulder_rotate_angle( float mAngle );
    bool			set_upper_arm_rotate_angle( float mAngle );

    float			get_shoulder_angle        ( );
    float			get_shoulder_rotate_angle ( );
    float			get_upper_arm_rotate_angle( );

    
    bool			set_elbow_angle          ( float mAngle );
    bool			set_wrist_angle          ( float mAngle );
    bool            set_wrist_rotate_angle   ( float mAngle );

    bool			increase_shoulder_angle       ( float mRelativeAngle );
    bool			increase_shoulder_rotate_angle( float mRelativeAngle );
    bool			increase_upper_arm_rotate_angle( float mRelativeAngle );
    
    bool			increase_elbow_angle          ( float mRelativeAngle );
    bool			increase_wrist_angle          ( float mRelativeAngle );
    bool            increase_wrist_rotate_angle   ( float mRelativeAngle );

    float           get_arm_length                ( );
    void            transfer_angles               ( );
    
    void            position_gripper              ( );  // move gripper object to the end of the forearm.
    
    // Each leg piece should have translates relative to previous.
    glLegSegment  	m_upper_arm;
    glLegSegment  	m_fore_arm;
    glGripper       m_gripper;
    // glHand        m_hand;

    float 			m_shoulder_angle;			// degrees  away from body.
    float 			m_shoulder_rotate_angle;    // forward to backward
    float 			m_upper_arm_rotate_angle;   // implement!!
    
    float 			m_elbow_angle;
    float 			m_wrist_angle;
    float 			m_wrist_rotate_angle;
    bool            m_left_arm;
};



#endif /* defined(__home3D__robot_arm__) */
