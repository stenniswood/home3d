#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include "all_objects.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


float magnitude( struct Vertex m1)
{
    float magnitude = sqrt( m1.position[0]*m1.position[0]  +
                           m1.position[1]*m1.position[1]  +
                           m1.position[2]*m1.position[2] );
    return magnitude;
}

struct Vertex subtract( struct Vertex m1, struct Vertex m2)
{
    struct Vertex v;
    v.position[0] = (m1.position[0] - m2.position[0]);
    v.position[1] = (m1.position[1] - m2.position[1]);
    v.position[2] = (m1.position[2] - m2.position[2]);
    return v;
}

float degrees( float mRad )
{
    return 180. * mRad / (M_PI);
}


/* My method will be to use the glRobot as a stamp.
        ie. You put the robot into the position you want it.
        And then extract feet position or robot angles to store away
        a sequence or path.     */
glRobot::glRobot( )
{
    m_object_class = 10;
	Initialize();
}

void glRobot::Initialize( )
{
	m_torso.width  = 18;
	m_torso.height = 24;
	m_torso.depth  = 8;
    m_last_direction_forward = true;
}

bool glRobot::check_limits( )
{
	bool retval = m_left_leg.check_angle_limits( );
	if (retval)
		retval  = m_right_leg.check_angle_limits( );
	return retval;
}

// FIX AND USE THIS ONE - was located in Route.
void glRobot::place_over_vertex( struct Vertex mVertex, struct Vertex mVertex2 )
{
    float leg_height   = m_left_leg.get_length();
    float torso_height = m_torso.height/2.;
    
    relocate( mVertex.position[0], mVertex.position[1]+leg_height+torso_height, mVertex.position[2] );

    float rise = mVertex2.position[0] - mVertex2.position[0];
    float run  = mVertex2.position[2] - mVertex2.position[2];
    m_y_angle = atan2( rise, run ) * 180./M_PI;
}

void glRobot::create_components()
{
    m_torso.setup    ( );
    m_left_leg.setup ( );
    m_right_leg.setup( );
    m_left_arm.m_left_arm = true;
    
    m_left_arm.setup ( );
    m_right_arm.setup( );
    m_head.setup     ( );

    m_components.push_back( &m_torso     );
    m_components.push_back( &m_left_leg  );
    m_components.push_back( &m_right_leg );
    m_components.push_back( &m_left_arm  );
    m_components.push_back( &m_right_arm );
    m_components.push_back( &m_head      );
    
    // Adjust their relative locations:
    float body_width_pos = ( m_torso.width/2. + m_left_arm.m_upper_arm.m_radius );
    m_left_arm.relocate (  body_width_pos, m_torso.height/2., 0. );
    m_right_arm.relocate( -body_width_pos, m_torso.height/2., 0. );

    // was just m_left_leg.relocate before
    // oldway
    m_left_leg.relocate ( +m_torso.width/2., -m_torso.height/2., 0 );
    m_right_leg.relocate( -m_torso.width/2., -m_torso.height/2., 0 );
    m_head.relocate( 0.0, m_torso.height/2.+m_head.m_head->m_radius, 0.0 );
}

/*MathVector glRobot::map_to_world_coords( MathVector mRobotCoordinate )
{
    glm::mat4 robot_mat      = get_body_matrix( );
    glm::vec4 hip_location     = m_left_leg.get_position( );
    glm::vec4 hip_coords_world = robot_mat * hip_location;  mRobotCoordinate
    return hip_coords_world ; *
} */



MathVector glRobot::get_eye_location    ( )        // in world cooridnates for viewing.
{
    MathVector eyeC = m_head.get_cyclops_coordinate();
    glm::vec4 eye;
    eye[0] = eyeC[0];
    eye[1] = eyeC[1];
    eye[2] = eyeC[2];
    eye[3] = 1.0;
    
    glm::mat4 head_rotate = m_head.get_body_matrix();
    glm::vec4 eyeRobotCoords = head_rotate * eye;

    glm::mat4 robot_matrix = get_body_matrix();
    glm::vec4 world_coords = robot_matrix * eyeRobotCoords;
    eyeC[0] = world_coords[0];
    eyeC[1] = world_coords[1];
    eyeC[2] = world_coords[2];
    return eyeC;
}

glm::vec4  glRobot::get_eye_target( )        // in world cooridnates for viewing.
{
    glm::vec4 center_head  = m_head.get_cyclops_fov_center_coordinate();
    glm::vec4 center_robot = m_head.map_coords(center_head);
    glm::vec4 center_world = map_coords( center_robot );
    return center_world;
}

/* Each leg has been relocated relative to the torso.  So m_x,m_y,m_z for m_left_leg identify
 where the leg attaches to the torso.  To get real world coordinates, need to map these thru
 the body matrix for the torso.  (calc_hip_positions())
 */
glm::vec4 glRobot::get_left_hip_coord( )
{
    // Take Hip position prior to torso translate/rotates.
    // LegSegments are extruded on the 1 axis (y axis)
    //glm::mat4 torso            = m_torso.get_body_matrix( );
    
    //glm::mat4 robot_mat        = get_body_matrix( );
    //glm::vec4 hip_coords_world = robot_mat * hip_location;
    glm::vec4 hip_location     = m_left_leg.get_position( );
    return hip_location;
}

glm::vec4 glRobot::get_right_hip_coord( )
{
    // Take Hip position prior to torso translate/rotates.
    // LegSegments are extruded on the 1 axis (y axis)
    //glm::mat4 torso            = m_torso.get_body_matrix( );
    //glm::mat4 robot_mat         = get_body_matrix( );
    //glm::vec4 hip_coords_world  = robot_mat * hip_location;
    glm::vec4 hip_location      = m_right_leg.get_position( );
    return hip_location;
}

glm::vec4 glRobot::get_left_heel_coord      ( )
{
    glm::vec4 heel_location = m_left_leg.get_heel_in_robot_coords(  );
    glm::vec4 heel_world = map_coords( heel_location );
    return heel_world;
}
glm::vec4 glRobot::get_right_heel_coord     ( )
{
    glm::vec4 heel_location = m_right_leg.get_heel_in_robot_coords( );
    glm::vec4 heel_world = map_coords( heel_location );
    return heel_world;
}


// especially calc_world_left_foot_positions on angles.
void glRobot::calc_world_left_foot_positions( )
{
    glm::vec4 heel_location = m_left_leg.get_heel_in_robot_coords(  );
    glm::vec4 toe_location  = m_left_leg.get_toe_in_robot_coords(  );

    glm::vec4 heel_world = map_coords( heel_location );
    glm::vec4 toe_world  = map_coords( toe_location  );

    // STUFF THE RESULTS:
    m_left_leg.m_foot.m_world_coords.heel[0] = heel_world[0];
    m_left_leg.m_foot.m_world_coords.heel[1] = heel_world[1];
    m_left_leg.m_foot.m_world_coords.heel[2] = heel_world[2];

    m_left_leg.m_foot.m_world_coords.toe[0] = toe_world[0];
    m_left_leg.m_foot.m_world_coords.toe[1] = toe_world[1];
    m_left_leg.m_foot.m_world_coords.toe[2] = toe_world[2];

    // GET ANGLE:
    m_left_leg.m_foot.calc_angle( m_left_leg.m_foot.m_world_coords );
}

void glRobot::calc_world_right_foot_positions( )
{
    glm::vec4 heel_location = m_right_leg.get_heel_in_robot_coords( );
    glm::vec4 toe_location  = m_right_leg.get_toe_in_robot_coords ( );

    glm::vec4 heel_world = map_coords( heel_location );
    glm::vec4 toe_world  = map_coords( toe_location  );
    
    // STUFF THE RESULTS :
    m_right_leg.m_foot.m_world_coords.heel[0] = heel_world[0];
    m_right_leg.m_foot.m_world_coords.heel[1] = heel_world[1];
    m_right_leg.m_foot.m_world_coords.heel[2] = heel_world[2];

    m_right_leg.m_foot.m_world_coords.toe[0] = toe_world[0];
    m_right_leg.m_foot.m_world_coords.toe[1] = toe_world[1];
    m_right_leg.m_foot.m_world_coords.toe[2] = toe_world[2];

    // GET ANGLE :
    m_right_leg.m_foot.calc_angle( m_right_leg.m_foot.m_world_coords );
}

void  glRobot::extract_robot_position( struct stBodyPosition* bp)
{
    //static struct stBodyPosition bp;

    bp->l_hip_rotate     = m_left_leg.get_hip_rotate_angle   ();
    bp->r_hip_rotate     = m_right_leg.get_hip_rotate_angle  ();
    bp->l_hip_swing      = m_left_leg.get_hip_ss_swing_angle ();
    bp->r_hip_swing      = m_right_leg.get_hip_ss_swing_angle();
    bp->l_hip_fb_swing   = m_left_leg.get_hip_angle     ();
    bp->r_hip_fb_swing   = m_right_leg.get_hip_angle    ();

    bp->l_knee           = m_left_leg.get_knee_angle    ();
    bp->r_knee           = m_right_leg.get_knee_angle   ();
    bp->l_ankle          = m_left_leg.get_ankle_angle   ();
    bp->r_ankle          = m_right_leg.get_ankle_angle  ();

    // ARMS:
    bp->l_shoulder_fore_rotate   = m_left_arm.get_upper_arm_rotate_angle();
    bp->l_shoulder_rotate        = m_left_arm.get_shoulder_rotate_angle();
    bp->l_shoulder_swing         = m_left_arm.get_shoulder_angle();
    bp->l_elbow                  = m_left_arm.m_elbow_angle;
    bp->l_wrist_swing            = m_left_arm.m_wrist_angle;
    bp->l_wrist_rotate           = m_left_arm.m_wrist_rotate_angle;
    
    bp->r_shoulder_fore_rotate   = m_right_arm.get_upper_arm_rotate_angle();
    bp->r_shoulder_rotate        = m_right_arm.get_shoulder_rotate_angle();
    bp->r_shoulder_swing         = m_right_arm.get_shoulder_angle();
    bp->r_elbow                  = m_right_arm.m_elbow_angle;
    bp->r_wrist_swing            = m_right_arm.m_wrist_angle;
    bp->r_wrist_rotate           = m_right_arm.m_wrist_rotate_angle;
}

void glRobot::set_robot_position( struct stBodyPosition* mBPosition )
{
    // LEGS:
    m_left_leg.set_hip_rotate_angle     ( mBPosition->l_hip_rotate );
    m_left_leg.set_hip_swing_angle      ( mBPosition->l_hip_swing );
    m_left_leg.set_hip_angle            ( mBPosition->l_hip_fb_swing  );
    m_left_leg.set_knee_angle           ( mBPosition->l_knee  );
    m_left_leg.set_ankle_angle          ( mBPosition->l_ankle );

    m_right_leg.set_hip_rotate_angle    ( mBPosition->r_hip_rotate );
    m_right_leg.set_hip_swing_angle     ( mBPosition->r_hip_swing );
    m_right_leg.set_hip_angle           ( mBPosition->r_hip_fb_swing  );
    m_right_leg.set_knee_angle          ( mBPosition->r_knee  );
    m_right_leg.set_ankle_angle         ( mBPosition->r_ankle );

    // ARMS:
    m_left_arm.set_upper_arm_rotate_angle( mBPosition->l_shoulder_fore_rotate );
    m_left_arm.set_shoulder_rotate_angle ( mBPosition->l_shoulder_rotate );
    m_left_arm.set_shoulder_angle        ( mBPosition->l_shoulder_swing );
    m_left_arm.set_elbow_angle           ( mBPosition->l_elbow          );
    m_right_arm.set_wrist_angle          ( mBPosition->l_wrist_swing    );
    m_right_arm.set_wrist_rotate_angle   ( mBPosition->l_wrist_rotate   );
    
    m_right_arm.set_upper_arm_rotate_angle( mBPosition->r_shoulder_fore_rotate );
    m_right_arm.set_shoulder_rotate_angle ( mBPosition->r_shoulder_rotate );
    m_right_arm.set_shoulder_angle        ( mBPosition->r_shoulder_swing );
    m_right_arm.set_elbow_angle           ( mBPosition->r_elbow          );
    m_right_arm.set_wrist_angle           ( mBPosition->r_wrist_swing    );
    m_right_arm.set_wrist_rotate_angle    ( mBPosition->r_wrist_rotate   );
    
    m_head.look_left( mBPosition->head_turn );
    m_head.set_tilt ( mBPosition->head_tilt );

    // more to come...
}


void glRobot::get_left_foot_location( struct stFootPosition* mFoot )
{
	memcpy(mFoot, &(m_left_leg.m_foot.m_world_coords), sizeof(struct stFootPosition) );
}

void glRobot::get_right_foot_location( struct stFootPosition* mFoot )
{
	memcpy(mFoot, &(m_right_leg.m_foot.m_world_coords), sizeof(struct stFootPosition) );
}

// Does nothing with the robot height:
void glRobot::center_body_between( float mLeftFoot[3], float mRightFoot[3] )
{
    float diff[3];
    // We go halfway between the two feet:
    diff[0] = (mLeftFoot[0] - mRightFoot[0])/2.0;
    diff[1] = (mLeftFoot[1] - mRightFoot[1])/2.0;
    diff[2] = (mLeftFoot[2] - mRightFoot[2])/2.0;

    m_x = mRightFoot[0] + diff[0];
    m_z = mRightFoot[2] + diff[2];
    printf("Robot Torso at : (%6.3f,%6.3f,%6.3f) \n", m_x, m_y, m_z );
}

void glRobot::angle_body_vertex( int mVertexIndex1, int mVertexIndex2 )
{
    
}

/* 
 mLeftFoot              - xyz positions
 mRightFoot             - xyz positions (not angles!)
 mPerpendicularFraction - fraction of the way between perpendicular and parallel.  */
void glRobot::angle_body_between( struct stFootPosition mSwingFoot, struct stFootPosition mStanceFoot,
                                  float mAngleOffset )
{
    // How to do this exactly?  We may not want it perpendicular to the feet.  In walking there
    // is a natural twist of the torso.  And we can streth the feet farther by making it parallel
    // to the difference.
    // We go halfway between the two feet:
    // 2*PI
    // 360 - 90 - 134 = 
    float angle = (mSwingFoot.angle[1] - mStanceFoot.angle[1]) / 2.0;
    m_y_angle   =  degrees(angle + mAngleOffset + mStanceFoot.angle[1]);
    printf("SwingFootAngle=%5.3f; StanceFootAngle=%5.3f;  delta/2=%5.3f;  m_y_angles=%5.3f \n",
           mSwingFoot.angle[1], mStanceFoot.angle[1], angle, m_y_angle );
}

// vector pointing straight in front of torso (based on m_xyz_angles )
glm::vec4 glRobot::get_robot_front_vector( )
{
    glm::mat4 bm = get_body_matrix();
    glm::vec4 bv( 0.0, 0.0, 1.0, 1.0);
    glm::vec4 rbv = bm * bv;
    return rbv;
}

bool glRobot::is_in_front_of( MathVector mWorldLoc )
{
    MathVector body_position(3);
    MathVector front_vector(3);
    MathVector front_perp(3);
    glm::vec4 front = get_robot_front_vector();
    
    Line body(3);
    Line test_pt(3);
    
    body.m_origin[0] = m_x;
    body.m_origin[1] = m_y;
    body.m_origin[2] = m_z;
    body.m_vector[0] = front[0];
    body.m_vector[1] = front[1];
    body.m_vector[2] = front[2];
    
    front_perp = front_vector.get_perp_yz();
    test_pt.m_origin[0] = mWorldLoc[0];
    test_pt.m_origin[1] = mWorldLoc[1];
    test_pt.m_origin[2] = mWorldLoc[2];
    test_pt.m_vector[0] = front_perp[0];
    test_pt.m_vector[1] = front_perp[1];
    test_pt.m_vector[2] = front_perp[2];

    float t;
    float s;
    body.line_intersection(test_pt, t, s );
    
    if (t>0)                // projection along front_vector.
        return true;
    else
        return false;
}


/* There are different styles of walking.
        One common way (ala U of M's Mabel) you don't need to push off with your back leg's toes.
        So we'll match the foot to be level with the ground at the end of the stride.
 */
/* There are 3 angles:  left,right, and body.
 even if left & right are the same.  (we may want body to be slightly twisted -
 but for now, we'll make a squarish walk. Legs just forward and backward.)
 This makes calculations easier and the real legs on the stand will be clearer.
 When turning a corner (ie. 90 deg)
 we'll split the body angle between the left and right.  */
void glRobot::place_feet_at( struct stFootPosition lfp, struct stFootPosition rfp, bool mLeftTurn )
{
    /* BODY XYZ : (more involved here than just mid-point.  without side swing ability on legs,
                    a foot which has 0 hip rotation has to lie on the foot print; and let the other leg swing more.
                    but this is somewhat close for now.  let's add the other phases of walking first.
    */
    center_body_between( lfp.heel, rfp.heel );  // x,y,z only.  No orientation

    // BODY ORIENTATION:
    // want to order lfp and rfp so that the front leg is first parameter : (fixes some turns)
    MathVector mv(3);
    mv[0] = lfp.heel[0];    mv[1] = lfp.heel[1];    mv[2] = lfp.heel[2];
    if ( is_in_front_of( mv ) == true )
        angle_body_between ( lfp, rfp, 0.0 );
    else
        angle_body_between ( rfp, lfp, 0.0 );

    // HIP ROTATE ANGLES :
    float body_angle     = glm::radians( m_y_angle );
    float angle_diff_deg = degrees( lfp.angle[1] - body_angle );
    m_left_leg.m_hip_rotate_angle  = +angle_diff_deg;
    m_right_leg.m_hip_rotate_angle = -angle_diff_deg;

    // HIP SWING ANGLES :   Calc the Swing angle of the leg
    float hypotenus = m_left_leg.get_length ();
    glm::vec4 lhip  = get_left_hip_coord();
    lhip = map_coords( lhip );    //calc_left_hip_world_coords();
    float dx        = (lfp.heel[0]-lhip[0]);
    float dz        = (lfp.heel[2]-lhip[2]);
    float floor_distance = sqrt( dx*dx + dz*dz );
    float hangle_rad     = asin( floor_distance / hypotenus );
    printf("place_left_foot: floor_dist=%6.3f; hyp=%6.3f  hangle_rad=%6.3f\n", floor_distance, hypotenus, hangle_rad );

    if (mLeftTurn)
    {
        m_left_leg.m_knee_angle  = 0.;
        m_left_leg.set_hip_angle  (  degrees( hangle_rad ));
        m_right_leg.set_hip_angle ( -degrees( hangle_rad ));
        m_right_leg.make_foot_level();
        m_left_leg.set_ankle_angle( 0 );
    } else {
        m_right_leg.m_knee_angle  = 0.;
        m_right_leg.m_ankle_angle = 0.;
        m_left_leg.set_hip_angle  ( -degrees( hangle_rad ));
        m_right_leg.set_hip_angle ( +degrees( hangle_rad ));
        m_left_leg.make_foot_level();
        m_right_leg.set_ankle_angle( 0 );
    }
}

void glRobot::place_foot_at ( struct stFootPosition fp, glLeg& mSwing, glLeg& mStance )
{
    mSwing.m_foot.m_world_coords = fp;

    center_body_between( fp.heel, mStance.m_foot.m_world_coords.heel );

    // HIP ROTATE ANGLES:   ( Angle between left & right feet )
    // TAKE EXAMPLE:  0,90. Degrees.  PUT BODY AT 45
    //      THEN LEFT LEG IS : -45 INWARD
    //       AND RIGHT LEG IS: 45 INWARD (OPPOSITE SIGN ON RIGHT LEG)
/*  float angle_diff = ( fp.angle[1] - m_right_leg.m_foot.m_world_coords.angle[1] ) /2.0 ;
    float angle_diff_deg = degrees( angle_diff );
    m_y_angle = 90. - degrees((angle_diff + m_right_leg.m_foot.m_world_coords.angle[1]));
    m_left_leg.m_hip_rotate_angle  = -angle_diff_deg;
    m_right_leg.m_hip_rotate_angle = +angle_diff_deg;

    // NOW HIP ANGLES (ASSUMING SQUARE WALKING - leg is straight)
    // We know the delta X and the hypotenus :

    m_y = hypotenus * cos(hangle_rad) + fp.heel[1];
    m_y += m_torso.height/2.;

    m_left_leg.m_hip_angle  = degrees( hangle_rad );
    m_right_leg.m_hip_angle = - m_left_leg.m_hip_angle; */
}


/* Use for the heel strike portion of the walking */
void	glRobot::place_left_heel_at( float mHeel[3] )
{
	// Calc distance between feet.
	//struct Vertex Heel;
	//Heel.position[0] = mHeel[0];	Heel.position[1] = mHeel[1];	Heel.position[2] = mHeel[2];

	//struct Vertex delta = subtract( Heel,  m_right_leg.m_foot.m_fp.heel );
    float delta[3];
    delta[0] = mHeel[0] - m_right_leg.m_foot.m_world_coords.heel[0];
    delta[1] = mHeel[1] - m_right_leg.m_foot.m_world_coords.heel[1];
    delta[2] = mHeel[2] - m_right_leg.m_foot.m_world_coords.heel[2];

    float  distance_between_feet = sqrt( delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2] );

    //magnitude ( delta );
	printf("distance_between_feet %6.1f\n", distance_between_feet);
	
	// isosceles triangle  = 2 right triangles
	float dist = distance_between_feet / 2.0;
    float total_leg_length = m_left_leg.get_length();
    
    float angle = acos( dist / total_leg_length );		// radians !
	m_left_leg.set_hip_angle ( degrees(angle));
	m_left_leg.m_knee_angle = 0.0;
	
	m_right_leg.set_hip_angle ( - degrees(angle));

	// vertical distance to base of robot body:
	m_y = sin(angle) * total_leg_length;
	m_x = (delta[0]/2.0);
	m_z = (delta[2]/2.0);

	// Calculate the angle between the Robot legs - split angle in half.
	// Keep simple here.  Let's calculate the move, and then evaluate the angles.
	// Back calculate hip angle :
}

void	glRobot::place_right_heel_at( float mHeel[3] )
{
    
}
void	glRobot::place_left_toe_at( struct Vertex mHeel )
{
    
}
void	glRobot::place_right_toe_at( struct Vertex mHeel )
{ 
    
}




#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
void glRobot::test()
{
    glm::vec4 RobotCoordinate( 1.0, 2.0, 3.0, 1.0 );
    printf("\n%s\n", to_string(RobotCoordinate).c_str());
    
    glm::vec4 tmp = map_coords(RobotCoordinate);
    printf("%s\n", to_string(tmp).c_str());
    
    glm::vec4 tmp2 = map_to_object_coords(tmp);
    printf("%s\n", to_string(tmp2).c_str());        // should be same as starting
}
