
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
#include <string>
#include <string.h>

#include "imageloader.h"
#include "all_objects.h"
#include "simulator_memory.h"
#include "sequencer_memory.h"
#include "client_memory.hpp"
#include "proc_client_cmds.h"
#include "sign.h"
#include "quarter_629.h"
#include "hampton_complex.h"
#include "basket_ball_court.h"
#include "sports_bench.h"
#include "basketball_arena.h"
#include "atom_objects.h"
#include "truss.h"
#include "rafter.h"
#include "road.h"
#include "sql_load.hpp"

#include "init_samples.hpp"
#include "asset_importer.hpp"       // sjt spin on it.



bool pauser          = true;
union uGlobalVars theGlobals;

void init_construction_objects();


#define GROUND_SIZE 800.
#define eye_follows_robots theGlobals.raw_array[1]

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
    eye  = robot.get_eye_location_world_coords();
    eyeX = eye[0];
    eyeY = eye[1];
    eyeZ = eye[2];
    
    glm::vec4 center_world = robot.get_eye_target_world_coords();
    centerX = center_world[0];
    centerY = center_world[1];
    centerZ = center_world[2];
}



void change_route( MathVector mSource, MathVector mDestination )
{
    printf( "Now Showing Route : \n" );
    mSource.print();
    mDestination.print();

    map2D.map_route         ( multiRoute, mSource, mDestination );
    robot.m_route.create_from_multi( multiRoute );
    robot.plan_steps        ( 2.0*12 );
    
    robot.gl_register       ( );
    robot.m_glide_index = 0;
}



// Index into route.m_right_steps[] and left.
int  l_step_index  = 0;
int  r_step_index  = 0;
bool left_leg_turn = true;
bool last_direction_forward = true;
bool show_tree      = false;

glArm*  selected_arm  = &(robot.m_left_arm);          // 0=left; 1=right;
glLeg*  selected_leg  = &(robot.m_left_leg);          // 0=left; 1=right;
bool  last_select_leg = true;


void handleKeypress(unsigned char key, int x, int y)
{
    static int  sequence_number = 0;
    string seq_filename;
    static bool result = false;
    static float lift_height=0.0;
	float tmp;
    
	switch (key) {
        case 'v':   //show_tree = !show_tree;
                    robot.morph_demos();
                    robot.reset_limbs_path();
                    robot.m_interpolated_index = 0;
                    seq_filename = "sequence_"+std::to_string(sequence_number)+".txt";
                    robot.save_sequence_file( seq_filename );
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
        case ' ':   robot.start_walking();
                    printf("Angle=%6.2f; centerX=%6.2f; centerY=%6.2f; centerZ=%6.2f;\n", centerAngle,
							centerX, centerY, centerZ );
                    printf("eyeX=%6.2f; eyeY=%6.2f; eyeZ=%6.2f;\n", eyeX, eyeY, eyeZ );
                    pauser = !pauser;
                    //create_nose_line      ();
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

        // Standing Turn of Robot:
        case 'j': robot.standing_turn_sequence(180, true);
            break;
        case 'J': robot.standing_turn_sequence(-180, false);
            break;
            
        /*  Direct Limb Control:
                    i=hip angle;    o=hip_rotate;   p=hip_side_swing;
                    k=knee angle;   l=lift leg;     ,=ankle anlge;
         
         */
        case 'i': if (last_select_leg)
                    selected_leg->increase_hip_angle            ( +2.);
                  else
                    selected_arm->increase_shoulder_rotate_angle( +2.);
            break;
        case 'I': if (last_select_leg)
                    selected_leg->increase_hip_angle            ( -2.);
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

        case 'l': if (last_select_leg) {
                    lift_height += 1;
                    float max_squat = robot.compute_max_squat_distance();
                    printf("Max Squat delta = %6.2f; now=%6.2f\n", max_squat, lift_height);
                    if (lift_height>max_squat)
                        lift_height = max_squat;
                    robot.squat_distance( lift_height );
                    //bool left = (selected_leg==&(robot.m_left_leg));
                    //robot.lift_leg_from_standing  (lift_height, left);
                  }
                  break;
        case 'L': if (last_select_leg) {
                    lift_height -= 1;       if (lift_height<0) lift_height =0;
                    robot.squat_distance( lift_height );
                    //bool left = (selected_leg==&(robot.m_left_leg));
                    //robot.lift_leg_from_standing  (lift_height, left);
                  }
                  break;
            
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

        case 'R':   if (eye_follows_robots == 0)
                    {
                        assume_robot_view();
                        eye_follows_robots = 1;
                    } else
                        eye_follows_robots = 0;
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
    robot.m_right_leg.make_foot_level();
    robot.m_left_leg.make_foot_level();
    
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

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat lightPos[]   = { -2 * BOX_SIZE, 200,  4 * BOX_SIZE, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos  );
	
	gluLookAt( eyeX,   	  eyeY,      eyeZ,  
			   centerX,   centerY,   centerZ,  
			   0,   	  1.0,   	0   );	

	glShadeModel(GL_SMOOTH);         //GL_FLAT
	draw_objects();
    drawMeshes();       // Imported meshes!

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

    /* CAMERA */
    static int counter = 0;
   /* if (theGlobals.gv.CameraOn)  {
        if ((counter++ % 10)==0)
           ((CameraTexture*)screen.m_texture)->timeslice();
    } else {
       // ((CameraTexture*)screen.m_texture)->stop();
    } */
    
    /*  VIEWPOINT */
    switch (eye_follows_robots) {
        case 0:  /* user moveable viewpoint */  break;
        case 1:  assume_robot_view(); break;
        case 2:  eye_follow_route( *bball_arena.bball_court->m_routes[0] );   break;  // demo sweep over basketball court.
        default: break;
    }
    
    if ((pauser==false) && (pause_count==0))
	{
        // WAVE THE PAPER:
        static float phase = 0.0;
        phase += 2.*M_PI / 180;     // 2 degrees
//        paper.gl_unregister();
//        paper.wave_it(10, 2, phase );
//        paper.gl_register();
        
        theWorld.time_slice( time_period*2 );

        robot.time_slice();
//        create_nose_line();
        
        pause_count = 1;
	} else if (pauser==false) pause_count--;

	glutPostRedisplay();
	glutTimerFunc(ms_update_rate, update, 0);
}

/*************************/
/* MOUSE STUFF           */
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

      glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600, 760);
	glutCreateWindow("Putting It All Together - beyond-kinetics.com");
	initRendering();

  //  delete_all_shm();
    int client_memory_is_attached    = connect_shared_client_memory   ( TRUE );
    int sequencer_memory_is_attached = connect_shared_sequencer_memory( TRUE );
    int simulator_memory_is_attached = connect_shared_simulator_memory( TRUE );
    
    sql_load_scene(1);
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



