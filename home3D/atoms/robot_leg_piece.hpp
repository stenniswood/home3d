/* This class will generate OpenGL Vertex data
	for a Robot leg piece.  Indented on the one end and forked on the other.
	This can be used for both upper and lower legs (at least as far as simulation 
	is concerned!)

	It actually consists of 2 extrusion pieces.
	The thin part for the outside and fork.
	and the thick inner part.
*/

#include <vector>
//#include "all_objects.h"
#include "extrusion.hpp"
#include "cylinder.hpp"
#include "robot_foot.hpp"


using namespace std;

#ifndef _ROBOT_LEG_PIECE_HPP_
#define _ROBOT_LEG_PIECE_HPP_

const float LEG_SEGMENT_LENGTH   = 17.;
const float FORK_CENTER_LENGTH   = 3.;
const float TONGUE_CENTER_LENGTH = 3.;
const float TONGUE_WIDTH 		 = 1.5;

const float ROBOT_HIP_WIDTH    =  1.5;
const float ROBOT_TORSO_HEIGHT =  5.0;
const float UPPER_LEG_LENGTH   = 17.0;		// This is joint to joint
const float LOWER_LEG_LENGTH   = 17.0;		// This is joint to joint

const float HEEL_HEIGHT		   = 3.0;


class glInner : public glExtrusion
{
public:
	glInner();
			void	generate_shank_vertices( );		// 2D
			void	generate_round_vertices( );		// 2D
	virtual void	generate_layer_vertices( );		// 2D
	virtual GLbyte 	generate_disc_indices  ( GLbyte mStartingVertexIndex=0 );
	virtual void	generate_vertices		( );

	int		m_vertex_count;
	float	m_leg_length  ;
	// rectangle and 1/2 circle and small inner circle (for shank)
};


class glLegSegment : public glCylinder
{
public:
	glLegSegment( float mLength );

	//void	Initialize	( );

	void 	generate_fork_vertices( float mKeyWidth, float mYlocation );
	void 	generate_round_vertices( );
	void 	generate_stud_vertices ( float mRadius, float mLength, float mWidth );	
	void	generate_fork_indices( );
	
	void	setup();
	void 	draw_body();
	
	float 	m_joint_length;					// distance joint to joint.
	float	m_number_fork_indices;			// both sides of the fork.  *2 for top&bottom.
	int		m_fork_start_vertex;
	int		m_stud_start_vertex;	
	int		m_number_stud_indices;	
};


const float MAX_KNEE_ANGLE      = 120.0;
const float MIN_KNEE_ANGLE      =   0.0;

const float MAX_HIP_ANGLE       = 160.0;
const float MIN_HIP_ANGLE       = -60.0;

const float MAX_HIP_ROTATE_ANGLE =  45.0;
const float MIN_HIP_ROTATE_ANGLE = -20.0;

const float MAX_HIP_SS_SWING_ANGLE =  45.0;
const float MIN_HIP_SS_SWING_ANGLE = -20.0;

const float MAX_ANKLE_ANGLE      =  45.0;
const float MIN_ANKLE_ANGLE      = -45.0;


#define m_hip_rotate_angle     m_upper_leg.m_y_angle
#define m_hip_fb_swing_angle   m_upper_leg.m_x_angle            // front to back swing
#define m_hip_ss_swing_angle   m_upper_leg.m_z_angle            // side to side swing

#define m_knee_angle           m_lower_leg.m_x_angle
#define m_ankle_angle          m_foot.m_x_angle


class glLeg : public glMolecule     // was glObject!
{
public:
	glLeg( );
	void			Initialize          ( );
	void			create_components   ( );
	virtual void	draw_body           ( );

	bool			check_angle_limits  ( );
	
	bool			set_hip_angle		( float mDegrees );
	bool			set_hip_rotate_angle( float mDegrees );
    bool			set_hip_swing_angle ( float mDegrees );

    bool			set_knee_angle		( float mDegrees );
    bool			set_ankle_angle		( float mDegrees );

    float			get_knee_angle		  ( )  { return m_knee_angle;         };
    float			get_hip_angle		  ( )  { return m_hip_fb_swing_angle; };
    float			get_hip_rotate_angle  ( )  { return m_hip_rotate_angle;   };
    float			get_ankle_angle		  ( )  { return m_ankle_angle;        };
    float           get_hip_ss_swing_angle( )  { return -m_hip_fb_swing_angle; };

	bool			increase_hip_angle	( float mRelativeAngle );
	bool			increase_hip_rotate_angle( float mRelativeAngle );
    bool			increase_hip_swing_angle( float mRelativeAngle );
    bool			increase_knee_angle	( float mRelativeAngle );
    
	bool			increase_ankle_angle( float mRelativeAngle );

    // All these are in relation to the heel:
    float           get_vertical_height ( bool mHeelOnly=true );      // distance from hip to heel vertically
    float           get_fore_aft        ( bool mHeelOnly=true );      // distance from hip to heel horizontally
    void            make_foot_level     (                     );      // with ground

    void            move_heel_to        ( MathVector mheel    );
    
    // STANDARD POSITIONS :
    void            standing_position       ( );
    void            sitting_position        ( );
    void            floor_sitting_position  ( );  // legs stretched out in front
    
    float           get_length              ( );  // from hip to heel
    void            transfer_angles         ( );  //

    /* Note:  Leg and robot coordinates differ only by a translation  */
    glm::vec4       map_foot_to_leg_coords  ( glm::vec4 mFootCoord );    // result in m_robot_coords
    glm::vec4       map_foot_to_robot_coords( glm::vec4 mFootCoord );    // result in m_robot_coords

    glm::vec4       map_leg_to_foot_coords  ( glm::vec4 mFootCoord );    // result in m_robot_coords
    glm::vec4       map_robot_to_foot_coords( glm::vec4 mFootCoord );    // result in m_robot_coords

    glm::vec4       get_toe_in_robot_coords ( );
    glm::vec4       get_heel_in_robot_coords( );
    
    bool			m_foot_planted;             // Means immovable
    // Each leg piece should have translates relative to previous.
    // None for Leg.
    // upper_leg has the torso width.
    // lower_leg has upperleg_length translate
    // foot has lowerleg_length translate
	glLegSegment  	m_upper_leg;
	glLegSegment  	m_lower_leg;		
	glFoot			m_foot;
    
    bool            m_free_hanging;         // or supporting weight    
};


#endif
