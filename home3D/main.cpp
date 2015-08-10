
/* Home3D Robot Simulator
 * www.beyondkinetics.com
 * Stephen Tenniswood
 */
#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <GL/glut.h>
#include <GLU/glu.h>
#endif

#include <list>
#include <math.h>
#include "imageloader.h"
#include "all_objects.h"
#include "simulator_memory.h"
#include "sequencer_memory.h"
#include "proc_client_cmds.h"
#include "sign.h"

#include "quarter_629.h"
#include "hampton_complex.h"
#include "basket_ball_court.h"
#include "sports_bench.h"
#include "basketball_arena.h"

#include "atom_objects.h"


/*************** OBJECTS ***********************/
//glStairway      stairs;
/*
Palette         palette;
glCabinet       cabinet;
glDrawer            drawer;
glIbeam             ibeam(12.*8.);
*/
glFaceBox           cube;
txtContainer        ground;
glRing              ring;

glLightSwitch       ls;
glBrickWall         brick_wall;
glCameraScreen      screen;
glPaper             paper(1);
glWindow            window;

glOneBuilding       Hcomplex;
glOurHouse          house;
glApartment         apartment;
glApartment         apartment2;
glMap2D             map2D;

glChair             chair;
glMultiRoute        multiRoute;
glMultiRoute        multiRoute2;

glRoute             door_path;
glTable             table;
glTableCenterPole   tableCP;
glCounter           KitchenSinkCounter(5.*18., 26., 37. );
glWalkingRobot      robot;
glSphere            sphere1(NBA_BASKET_BALL_RADIUS, 10, 64);
glSphere            sphere2(FIFA_SOCCER_BALL_RADIUS_SIZE5, 10, 64);
glSphere            sphere3(FIFA_SOCCER_BALL_RADIUS_SIZE4, 10, 64);
glSphere            sphere4(BASEBALL_RADIUS, 10, 64);

glArena             bball_arena;


//CameraTexture       camera;
//glPicnicTable       picnic;
//glSign              sign;
//glTextureCylinder   txt_cylinder(24);
/************ END OF OBJECTS ****************/

#define GROUND_SIZE 800.

using namespace std;

const float BOX_SIZE = 27.0f; // The length of each side of the cube
float _angle  = 0;            // The rotation of the box
float y_angle = 0;            // The rotation of the box
float x_angle = 0;            // The rotation of the box
float z_angle = 0;            // The rotation of the box
GLuint _textureId;            // The OpenGL id of the texture

float x = 0;                // The position of the box
float y = 0;                // The position of the box
float z = 5;                // The position of the box
bool     eye_follows_robots=false;
GLdouble eyeX;
GLdouble eyeY;
GLdouble eyeZ;
MathVector eye;             // not implemented yet.
MathVector center;          // not implemented yet.
GLdouble centerX,centerY,centerZ;
GLdouble upX, upY, upZ;
GLdouble centerRadius = 100.;
GLdouble centerAngle  = 0.0 ;
GLdouble angle_increment = 5.*3.1415/180.;

void rotate( )
{
	if (centerAngle > 2*M_PI)	centerAngle = 0.	;
	if (centerAngle < 0.    )	centerAngle = 2*M_PI;

	centerX = centerRadius * sin(centerAngle) + eyeX;
	centerZ = centerRadius * cos(centerAngle) + eyeZ;
	//printf("centerAngle=%6.3f cx=%6.2f; cy=%6.2f \n", centerAngle, centerX, centerY );
}

void move_forward( float mAmount )
{
	float F_vec_x = eyeX - centerX;
	float F_vec_y = eyeY - centerY;
	float F_vec_z = eyeZ - centerZ;
	float sum = fabs(F_vec_x) + fabs(F_vec_y) + fabs(F_vec_z);
	
	F_vec_x *= 2*mAmount/sum;
	F_vec_z *= 2*mAmount/sum;
	//printf("Delta x=%6.3f \t", F_vec_x );
	//printf("Delta z=%6.3f \n", F_vec_z );

	eyeX 	+= F_vec_x;
	eyeZ 	+= F_vec_z;
	centerX += F_vec_x;
	centerZ += F_vec_z;
}

void move_sideways( float mAmount )
{
    MathVector forward(3);
    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    MathVector perp = forward.get_perp_xz();
    perp.unitize();
    perp *= mAmount;

    eyeX    += perp[0];
    eyeZ    += perp[2];
    centerX += perp[0];
    centerZ += perp[2];
}

void eye_follow_route(glRoute& mRoute)
{
    static int v=0;
    struct Vertex tmp  = mRoute.m_vertices[v];
    eyeX = tmp.position[0];
    eyeY = tmp.position[1];
    eyeZ = tmp.position[2];

//  glm::vec4 center_world = robot.get_eye_target();
    tmp = mRoute.m_vertices[v+5];
    centerX = tmp.position[0];
    centerY = tmp.position[1];
    centerZ = tmp.position[2];
    v++;
    if (v > mRoute.m_vertices.size())
        v = 0;
}

void assume_robot_view()
{
    eye  = robot.get_eye_location();
    eyeX = eye[0];
    eyeY = eye[1];
    eyeZ = eye[2];
    
    glm::vec4 center_world = robot.get_eye_target();
    centerX = center_world[0];
    centerY = center_world[1];
    centerZ = center_world[2];
}

vector<MathVector>   Destinations;
vector<MathVector>   Sources;
int DestinationIndex = 1;
int SourceIndex      = 0;

void change_route( MathVector mSource, MathVector mDestination )
{
    printf( "Now Showing Route : \n" );
    mSource.print();
    mDestination.print();

    map2D.map_route( multiRoute, mSource, mDestination );
    robot.m_route.start_over_at2(mSource);
    robot.m_route.create_from_multi( multiRoute );
    robot.plan_steps            ( );
    robot.gl_register           ( );

    //multiRoute.gl_unregister  ( );
    multiRoute.setup            ( );
    multiRoute.gl_register      ( );

    // Show end points :
    multiRoute2.m_tree.clear    ( );
    multiRoute2.gl_unregister   ( );
    multiRoute2.start_over_at2  ( mSource );
    multiRoute2.add_way_point2  ( multiRoute2.m_tree, mDestination );
    multiRoute2.setup           ( );
    multiRoute2.change_color    ( 0xFFAF2F2F );
    multiRoute2.gl_register     ( );
}

void select_route()
{
    long size = Destinations.size();
    DestinationIndex++;
    if (DestinationIndex>=size)
    {
        DestinationIndex = 0;
        SourceIndex++;
        if (SourceIndex>=size)
            SourceIndex = 0;
    }
    change_route(Sources[SourceIndex], Destinations[DestinationIndex] );
}

// Index into route.m_right_steps[] and left.
int  l_step_index  = 0;
int  r_step_index  = 0;
bool left_leg_turn = true;
bool last_direction_forward = true;
bool show_tree      = false;
bool pause          = true;
glArm*  selected_arm  = &(robot.m_left_arm);          // 0=left; 1=right;
glLeg*  selected_leg  = &(robot.m_left_leg);          // 0=left; 1=right;
bool  last_select_leg = true;

void handleKeypress(unsigned char key, int x, int y)
{
    static bool result = false;
	float tmp;
	switch (key) {
        case 'v':   //show_tree = !show_tree;
                    robot.morph_demos();
                    robot.reset_limbs_path();
                    robot.m_interpolated_index = 0;
                    break;
        case 'b':   result = robot.play_next();
                    if (result) {
                        robot.sample_limbs_path();
                        robot.gl_unregister_limbs_path();
                        robot.gl_register_limbs_path();
                    }
                    else robot.m_interpolated_index = 0;
                    break;
        case 'z':   select_route();
                    break;
        case '1': tmp = KitchenSinkCounter.m_cabinets[0].m_door.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[0].m_door.open( tmp );
					break;
		case '2': tmp = KitchenSinkCounter.m_cabinets[1].m_door.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[1].m_door.open( tmp );
					break;
		case '3': tmp = KitchenSinkCounter.m_cabinets[2].m_door.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[2].m_door.open( tmp );
					break;
		case '4': tmp = KitchenSinkCounter.m_cabinets[3].m_door.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[3].m_door.open( tmp );
					break;
		case '!': tmp = KitchenSinkCounter.m_cabinets[0].m_door.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[0].m_door.open( tmp );
					break;
		case '@': tmp = KitchenSinkCounter.m_cabinets[1].m_door.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[1].m_door.open( tmp );
					break;
		case '#': tmp = KitchenSinkCounter.m_cabinets[2].m_door.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[2].m_door.open( tmp );
					break;
		case '$': tmp = KitchenSinkCounter.m_cabinets[3].m_door.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[3].m_door.open( tmp );
					break;

		case '5': tmp = KitchenSinkCounter.m_cabinets[0].m_drawer.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[0].m_drawer.open( tmp );
					break;
		case '6': tmp = KitchenSinkCounter.m_cabinets[1].m_drawer.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[1].m_drawer.open( tmp );
					break;
		case '7': tmp = KitchenSinkCounter.m_cabinets[2].m_drawer.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[2].m_drawer.open( tmp );
					break;
		case '8': tmp = KitchenSinkCounter.m_cabinets[3].m_drawer.m_fraction_open += 0.05;
					KitchenSinkCounter.m_cabinets[3].m_drawer.open( tmp );
					break;
		case '%': tmp = KitchenSinkCounter.m_cabinets[0].m_drawer.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[0].m_drawer.open( tmp );
					break;
		case '^': tmp = KitchenSinkCounter.m_cabinets[1].m_drawer.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[1].m_drawer.open( tmp );
					break;
		case '&': tmp = KitchenSinkCounter.m_cabinets[2].m_drawer.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[2].m_drawer.open( tmp );
					break;
		case '*': tmp = KitchenSinkCounter.m_cabinets[3].m_drawer.m_fraction_open -= 0.05;
					KitchenSinkCounter.m_cabinets[3].m_drawer.open( tmp );
					break;
		case 'd': centerAngle -= angle_increment; rotate(); break;
		case 'a': centerAngle += angle_increment; rotate(); break;
        case ' ':
                    printf("Angle=%6.2f; centerX=%6.2f; centerY=%6.2f; centerZ=%6.2f;\n", centerAngle,
							centerX, centerY, centerZ );
                    printf("eyeX=%6.2f; eyeY=%6.2f; eyeZ=%6.2f;\n", eyeX, eyeY, eyeZ );
                    pause = !pause;
					break;

        /*   Next 4 - Select a limb for direct control   */
        case '{':   selected_arm = &(robot.m_right_arm);
                    last_select_leg = false;
                    break;
        case '}':   selected_arm = &(robot.m_left_arm);
                    last_select_leg = false;
                    break;

        case '[':   selected_leg = &(robot.m_right_leg);
                    last_select_leg = true;
                    break;
        case ']':   selected_leg = &(robot.m_left_leg);
                    last_select_leg = true;
                    break;

        /*  Direct Limb Control                         */
        case 'i': if (last_select_leg)
                    selected_leg->increase_hip_angle 	( +2.);
                  else
                    selected_arm->increase_shoulder_rotate_angle( +2.);
            break;
        case 'I': if (last_select_leg)
                    selected_leg->increase_hip_angle 	( -2.);
                  else
                    selected_arm->increase_shoulder_rotate_angle( -2.);
            break;

        case 'o': if (last_select_leg)
                selected_leg->increase_hip_rotate_angle( +2.);
            else
                selected_arm->increase_shoulder_angle  ( +2.);
            break;
        case 'O': if (last_select_leg)
                selected_leg->increase_hip_rotate_angle( -2.);
            else
                selected_arm->increase_shoulder_angle ( -2.);
            break;

        case 'p': if (last_select_leg)
                selected_leg->increase_hip_swing_angle( +2.);
            else
                selected_arm->increase_upper_arm_rotate_angle( +2.);
            break;
        case 'P': if (last_select_leg)
                selected_leg->increase_hip_swing_angle( -2.);
            else
                selected_arm->increase_upper_arm_rotate_angle( -2.);
            break;

        case 'k': if (last_select_leg)
                selected_leg->increase_knee_angle  (+2.);
            else
                selected_arm->increase_elbow_angle ( +2.); 	  break;
        case 'K': if (last_select_leg)
                selected_leg->increase_knee_angle  (-2.);
            else
                selected_arm->increase_elbow_angle ( -2.);    break;

        case ',': if (last_select_leg)
                selected_leg->increase_ankle_angle (+2.);
            else
                selected_arm->increase_wrist_angle     ( +2.);       break;
        case '<': if (last_select_leg)
                selected_leg->increase_ankle_angle (-2.);
            else
                selected_arm->increase_wrist_angle     ( -2.);       break;

        case '.': if (last_select_leg==false)
                selected_arm->increase_wrist_rotate_angle  (+2.);    break;
        case '>': if (last_select_leg==false)
                selected_arm->increase_wrist_rotate_angle  (-2.);    break;
        /* END OF DIRECT LIMB CONTROL */
        
        case '-': robot.take_step( false);                           break;
        case '=': robot.take_step( true );   robot.start_walking();  break;

        case 'R':   if (eye_follows_robots==false)
                    {
                        assume_robot_view();
                        eye_follows_robots = true;
                    } else
                        eye_follows_robots = false;
                    break;
            
        // Leg Movements:
		case 'g': selected_leg->increase_knee_angle  (+2.); 		 break;
		case 'G': selected_leg->increase_knee_angle  (-2.); 		 break;
		case 'B': selected_leg->increase_ankle_angle (-2.); 		 break;

		case 'w': centerY += 5.; break;		// look up.
		case 'x': centerY -= 5.; break;
		case 'q': eyeY -= 5.; centerY -= 5.; break;
		case 'r': eyeY += 5.; centerY += 5.; break;

		case 'c': move_forward (  5.);  break;
		case 'e': move_forward ( -5.); 	break;
		case 's': move_sideways(  2.);	break;
		case 'f': move_sideways( -2.); 	break;

		case 27: //Escape key
			exit(0);
	}
	glutPostRedisplay();
}


void initRendering()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 5000.0);
}
void draw_objects()
{
    theWorld.draw();
}

glDoor* tdoor1=NULL;
glDoor* tdoor2=NULL;
glTerrain terra;

void init_objects()
{
    MathVector tmp(3);
    tmp.set_xyz( 50, 2., 50. );             // Living room
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );
    tmp.set_xyz(180, 2., 50.);              // First bedroom
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );
    tmp.set_xyz(350, 2., 50.);              // Master Bedroom
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );
    tmp.set_xyz(375, 2., 180.);             // Master Closest
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );
    tmp.set_xyz(375, 2., 275.);             // Bathroom
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );
    tmp.set_xyz(210, 2., 250.);             // Kitchen
    Sources.push_back     ( tmp );
    Destinations.push_back( tmp );

    ground.width = 5000;
    ground.depth = 5000;
    ground.height = 1.0;
    ground.load_image("textures/grass2.jpg");
    ground.m_repetitions_x = 100;
    ground.m_repetitions_y = 100;
    ground.create();

    cube.width = 10;
    cube.depth = 10;
    cube.height= 20.0;
    cube.set_color( 0xFFFFFF00 );
    Texture* txt  = cube.load_image("textures/me_in_car.bmp",        FACE_TOP_ID );
    Texture* txt2 = cube.load_image("textures/sidewalk_painting.jpg",FACE_LEFT_ID);        // 0 = top
    cube.apply_front( txt);
    cube.apply_back (txt2);
//    cube.apply_left (txt2);
//    cube.apply_right(txt2);

/*    Texture* txt3 = cube.load_image("textures/door_carmelle.jpg",FACE_FRONT_ID);        // 0 = top
    cube.apply_front(txt3,1);
    cube.apply_back (txt3,1); */
    
    cube.m_R_velocity[1] = 25;
    cube.is_participating = true;
    cube.create();
    cube.relocate( 50, 90, 50 );

    
    
	// Fairly well tested units :
    /*fwall.set_length( 22 * 12 );
    struct stSize s;
    s.width = 36.;    s.height = 48.;
    glWindow* twin = new glWindow();
    twin->m_height = s.height;
    twin->m_size   = s;

    
	/* drawer.Initialize( 21., 14., 3.5 );
 	drawer.initialize_faceplate(16., 5.75, 0.75 );
 	drawer.relocate( 75., 30.0, 75.0 );
	drawer.create();

	cabinet.create	( );
	cabinet.relocate( 35., 0, 75. );

	door.m_direction_positive = false;
	door.Initialize( 29.5 );	//	m_door.Initialize		( 16., 22., 1. );
	door.create();
	door.close(1.0);
	door.relocate( 75., 20, 75.);
     bookcase.create();
     bookcase.m_y_angle = -90;
     bookcase.relocate( 150, 0, 20 );
     */
    
    struct stFootPosition LHeel;
    struct stFootPosition RHeel;
    robot.m_show_paths = true;
    robot.create();
    robot.get_left_foot_location ( &LHeel );
    robot.get_right_foot_location( &RHeel );
    robot.relocate( 20, 50, 20 );
    robot.goto_start_of_path();
    robot.hands_on_hip();
//  sphere.load_texture("textures/me_in_car.bmp");

    bball_arena.create();
    bball_arena.relocate( -40*12, 2, -94*12 );
    bball_arena.create_cam_routes();
    
    sphere1.is_participating = true;
    sphere1.coef_of_restitution = COEFF_RESTITUTION_BASKETBALL;
    sphere1.set_color( 0xFF7F7F00);
    sphere1.load_texture("textures/basketball_texture.jpg" );
    sphere1.create  (  );
    sphere1.m_velocity[0] = 40;
    sphere1.m_velocity[2] = -10;
    sphere1.relocate( 60, 70, 100 );

    sphere2.is_participating = true;
    sphere2.coef_of_restitution = COEFF_RESTITUTION_SOCCERBALL;
    sphere2.set_color( 0xFFFF0000);
    sphere2.load_texture("textures/soccer_ball_3.jpg" );
    sphere2.create  (  );
    sphere2.m_velocity[0] = -40;
    sphere2.m_velocity[2] = -20;
    sphere2.m_R_velocity[0] = 3;
    sphere2.m_R_velocity[1] = 3;
    sphere2.relocate( 60, 70, 100 );

    sphere3.is_participating = true;
    sphere3.coef_of_restitution = COEFF_RESTITUTION_SOCCERBALL;
    sphere3.set_color( 0xFF5F7F2F);
    sphere3.load_texture("textures/soccer_ball_world_cup.jpg" );
    sphere3.create  (  );
    sphere3.m_velocity[0] =40;
    sphere3.m_velocity[2] = 10;
    sphere3.m_R_velocity[0] = 2;
    sphere3.m_R_velocity[1] = 3;
    sphere3.relocate( 60, 70, 100 );

    sphere4.is_participating = true;
    sphere4.coef_of_restitution = COEFF_RESTITUTION_BASEBALL;
    sphere4.set_color( 0xFF0F0000);
    sphere4.load_texture("textures/baseball_texture.jpg" );
    sphere4.create  (  );
    sphere4.m_velocity[0] = -45;
    sphere4.m_velocity[2] = 20;
    sphere4.relocate( 60, 70, 100 );
    
    
    chair.m_name = "dinning room";
    chair.m_R_velocity[1] = 1.;
    chair.m_velocity[0] = 0.25;
    chair.m_y_angle     = 90;
    chair.create    ( );
    chair.relocate  ( 20., 0, +150 );

	table.create();
	table.relocate  ( 90.+24, 0, 291.-42.);

    tableCP.create();
    tableCP.relocate( 90.+24, 0, 91.-42.);
    
//    brick_wall.set_length_height(15*12, 8*12);
//    brick_wall.create  ( );
//    brick_wall.relocate( 0. , 0., -150.  );


/*    float scale = 6.0;
	paper.load_texture    ( "textures/me_in_car.bmp" );
    paper.set_width_height( 10*scale, 10*scale       );
    paper.create          (                          );
	paper.relocate        ( 40., 22.*4., 5.0 );
	paper.m_x_angle       = 90.;
	paper.m_y_angle       = 90.; */
    
    screen.set_width_height( 1280/10, 700/10 );
    ((CameraTexture*)screen.m_texture)->generate_PBO();
    ((CameraTexture*)(screen.m_texture))->timeslice();
    screen.create          (                      );
    screen.relocate        ( 10., 22.*4., 9.0     );
    screen.m_x_angle       = 180.;

    window.set_params  (50, 48, 48, 1.5 );
//    window.create      (  );
//    window.relocate( 40, 0, 40 );
    
    printf("\nApartment Create\n\n");
    apartment2.setup();
    apartment2.mirror_image_x();
    apartment2.gl_register();
    apartment2.relocate( 0, 0, 300);
    
    //Hcomplex.create();
    //house.create();
    //house.relocate( 0, 0, 0 );
    
    string searchname     = "master bedroom";
    string searchtypename = "door";
    apartment.create          ( );
    //VerbalObject* ptr = apartment.find_component(searchname, searchtypename);
    
    apartment.update_line_info( );
    apartment.create_map(map2D);
    map2D.create_2D_drawing();      // creates the opengl vertices.	//map2D.scale_drawing(0.125);
	map2D.print_2D_info();
	map2D.gl_register( );
    map2D.m_z_angle = 0.0;
    map2D.m_y       = 0.0;

    select_route( );
    
    
    //  terra.load_image( "textures/stucco-texture.jpg" );
    //  terra.load_image( "textures/Ireland.jpg"     );
    terra.set_stretch( 10, 10 );
    terra.m_y_scale = 1.0 ;
    //terra.load_image( "textures/heightmap6.jpg"  );
    terra.add_omission(&apartment);
//    terra.add_omission(&apartment2);
    //terra.add_omission(&bball_court);
    //terra.load_image( "textures/heightmap_P.png" );
    //terra.create();

    // Sample door handle path :
    //    (exact same method should be available for drawers, light switches)
    glDoor* selected_door = apartment.m_fwalls[1]->m_doors[0];
    selected_door->create_handle_path( door_path );
    door_path.map_vertices( apartment.m_fwalls[1]->get_body_matrix() );
    door_path.gl_register();
    
    
    /*route.start_over_at2    ( FinalPath[0] );
    long size = FinalPath.size();
    for (long i=0; i<size; i++)
        route.add_way_point2( FinalPath[i] );
    route.compute_slopes         ( );
    route.compute_robot_path     ( 30., true, robot );
    route.compute_angles         ( );
    route.generate_steps_vertices( );
    route.generate_VBO           ( );
    route.generate_IBO           ( );
    route.place_robot_over_vertex( 0, robot ); */
    
/*    txt_cylinder.m_radius = 15.;
    txt_cylinder.m_extrusion_length = 20.;
    txt_cylinder.m_extrusion_axis = 1;
    txt_cylinder.setup();
    txt_cylinder.gl_register();*/
    
	//KitchenSinkCounter.set_number_of_cabinets(4);
	//KitchenSinkCounter.create();
	//KitchenSinkCounter.relocate( 144., 0., 160.+36.+95. );

//    picnic.create();

    /*
    stairs.m_extrusion_length = 25*12;
    stairs.set_color( 0xFF0000FF);
    stairs.create();
    stairs.m_y_angle = 0.;
    stairs.relocate( 100, 10., -25*12 ); */
    
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0.0f, 0.0f, -200.0f);
	
	GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat lightPos[]   = { -2 * BOX_SIZE, 200,  4 * BOX_SIZE, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos  );
	
	gluLookAt( eyeX,   	  eyeY,      eyeZ,  
			   centerX,   centerY,   centerZ,  
			   0,   	  1.0,   	0   );	

	//glShadeModel(GL_FLAT);		
	draw_objects();

	// Draw ground:
	glBegin   (GL_QUADS);
	glNormal3f(0.0, 1.0f, 0.0f );
	glColor3f (0.7f, 0.7f, 0.7f);
	glVertex3f(-GROUND_SIZE, 0, -GROUND_SIZE);
	glVertex3f( GROUND_SIZE, 0, -GROUND_SIZE);
	glVertex3f( GROUND_SIZE, 0, GROUND_SIZE );
	glVertex3f(-GROUND_SIZE, 0, GROUND_SIZE );
	glEnd();	

	glutSwapBuffers();
}

#define ms_update_rate 25
/*
	Called every 25 milliseconds
	Runs thru the house along the path specified in "route"
*/
void update(int value) 
{
    float time_period = (ms_update_rate) / 1000.;
	static int pause_count  = 1;
    
    respond_to_client_timeslice( );
    static int counter = 0;
    if ((counter++ % 10)==0)
       ((CameraTexture*)screen.m_texture)->timeslice();
    if (eye_follows_robots)
        assume_robot_view();
    
    //eye_follow_route( *bball_arena.bball_court->m_routes[0] );
    
    if ((pause==false) && (pause_count==0))
	{
        // WAVE THE PAPER:
        static float phase = 0.0;
        phase += 2.*M_PI / 180;     // 2 degrees
//        paper.gl_unregister();
//        paper.wave_it(10, 2, phase );
//        paper.gl_register();
        theWorld.time_slice( time_period*2 );
        robot.update_animation();
        pause_count = 1;
	} else if (pause==false) pause_count--;

	glutPostRedisplay();
	glutTimerFunc(ms_update_rate, update, 0);
}

int     g_bButton1Down;
int     y_when_clicked;
int     x_when_clicked;
float   center_x_when_clicked = 0.;
float   center_y_when_clicked = 0.;
float   center_z_when_clicked = 0.;
double  y_angle_when_clicked  = 0.;
#define TRUE 1
#define FALSE 0

void MouseButton(int button, int state, int x, int y)
{	
  // Respond to mouse button presses.
  // If button1 pressed, mark this state so we know in motion function.
  if (button == GLUT_LEFT_BUTTON)
    {
    	center_x_when_clicked = centerX;
    	center_y_when_clicked = centerY;
    	center_z_when_clicked = centerZ;

        //Mathvector
        y_angle_when_clicked = atan2( centerX - eyeX, centerZ - eyeZ );
        
    	x_when_clicked = x;
    	y_when_clicked = y;
    	g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
    }
}

void MouseMotion(int x, int y)
{
    // If button1 pressed, zoom in/out if mouse is moved up/down.
	if (g_bButton1Down)
    {
        double geo_distance = sqrt((centerX - eyeX)*(centerX - eyeX) + (centerZ - eyeZ)*(centerZ - eyeZ));
        
        float deltaX = ( x - x_when_clicked );
        float deltaY = ( y - y_when_clicked )/2.;
        centerY = (center_y_when_clicked + deltaY);

        float radians_per_pixel = 0.25 / 50.;
        float delta_angle = deltaX * radians_per_pixel;
        float new_angle   = y_angle_when_clicked + delta_angle;
        float delta_center_to_eye_X = geo_distance * sin(new_angle);       // center to eye x / z
        float delta_center_to_eye_Z = geo_distance * cos(new_angle);       // center to eye x / z
        centerX = eyeX + delta_center_to_eye_X;
        centerZ = eyeZ + delta_center_to_eye_Z;
        
        x -= 320;
        y -= 320;
        //robot.m_left_arm.inverse_xyz (x/10., y/10., robot.m_left_arm.m_z );
        //robot.m_right_arm.inverse_xyz(x/10., y/10., robot.m_left_arm.m_z );

      //g_fViewDistance = (y - g_yClick) / 3.0;
      //if (g_fViewDistance < VIEWING_DISTANCE_MIN)
      //g_fViewDistance = VIEWING_DISTANCE_MIN;
      glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 760);
	glutCreateWindow("Putting It All Together - beyond-kinetics.com");
	initRendering();

  //  delete_all_shm();
    seq_allocate_memory();
    seq_attach_memory  ();
    seq_save_segment_id	("seq_segment_id.txt");

    sim_allocate_memory();
    sim_attach_memory  ();
    sim_fill_memory();
    sim_save_segment_id( "sim_segment_id.txt");
    
	init_objects();
    
    eyeX= 80;       eyeY = 120;     eyeZ = 80;
    eyeX= 1.59;     eyeY=120.00;    eyeZ=157.13;
    centerX= 35.80; centerY= 20.00; centerZ= 63.16;
	
	upX = 80.;
	upY = 80.;
	upZ = 80.;

	glutDisplayFunc (drawScene     );
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc   (MouseButton   );
	glutMotionFunc  (MouseMotion   );
	glutReshapeFunc (handleResize  );
	glutTimerFunc   (ms_update_rate, update, 0 );
	
	glutMainLoop();
    
    seq_detach_memory();
    sim_detach_memory();
	return 0;
}

void init_construction_objects()
{
    /*  ibeam.m_ends_color  = 0xFFFF0000;
     ibeam.m_color       = 0xFFFFFFFF;
     ibeam.m_is_closed   = true;
     ibeam.create(12*8., 2);
     ibeam.relocate( 0, 12*8, 0 ); */

    /*
     txtContainer    mebox;
     mebox.width  = CEMENT_BLOCK_LENGTH;
     mebox.height = CEMENT_BLOCK_HEIGHT;
     mebox.depth  = CEMENT_BLOCK_THICKNESS;
     mebox.setup();
     mebox.load_image("textures/cinder-block-texture.jpg");
     mebox.gl_register( );
     mebox.relocate   ( 50., 10., 200. ); */

    /*  barricade.create  ( );
     barricade.relocate( -10., 0., +60. );
  
     palette.create();
     palette.relocate   ( -100., 0., -100.);
     
     brick_wall.create  ( 15.*12, 8        );
     brick_wall.Relocate( -150. , 0., -150.); */
}


/*
 tdoor1  = new glDoor();
 tdoor1->set_params    ( 33.5 );
 tdoor1->set_hinge     (true);
 tdoor1->set_swing_side(false);
 tdoor1->create        (     );
 tdoor1->open          ( 0.5 );
 fwall.add_door        ( 2*12, tdoor1 );
 
 tdoor2 = new glDoor   ( );
 tdoor2->set_params    ( 33.5 );
 tdoor2->set_hinge     (false);
 tdoor2->set_swing_side(false);
 tdoor2->setup();
 tdoor2->open( 0.5 );
 fwall.add_door( 6*12, tdoor2 );
	fwall.add_window( 12*12.,  3.*12., twin );
 fwall.create  (               );
 fwall.relocate( 0.,  0., 275. );
 
 wall.m_wall_length = 22*12;
 wall.set_length_height( 22.*12., 10.*12. );
	wall.add_door     ( 2.*12., 33.     );
	wall.add_door     ( 6.*12., 33.     );
	wall.add_door     ( 10.*12., 28.    );
	wall.add_window   ( 12. * 12., s, 3.*12. );
	wall.create       (  );
	wall.relocate     ( 0.,  0., 475.    );
	wall.m_y_angle = 30.0;  
 */
/*   ring.set_color(0xFFFF0000);
 ring.set_finger_params(9, 40);
 ring.set_circle_params(0.25, 10);
 ring.setup();
 ring.gl_register();
 ring.relocate( 50, 60, 100 );       */

/*
	tray.width  = 18.;
	tray.height = 5.;
	tray.depth  = 12.0;
	tray.m_y = tray.height+2.;
	tray.generate_vertices();
	tray.generate_VBO();
	tray.generate_IBO();
 
	box2.width  = 4.;
	box2.height = 2.;
	box2.depth  = 2.0;
	box2.m_x 	= 12.;
	box2.m_is_closed = true;
	box2.generate_vertices();
	box2.generate_VBO();
	box2.generate_IBO();
 
	cyl.m_radius = 4.;
	cyl.m_height = 8.;
	cyl.generate_vertices();
	cyl.generate_VBO();
	cyl.generate_IBO();
	printf("=============================\n");
 */
