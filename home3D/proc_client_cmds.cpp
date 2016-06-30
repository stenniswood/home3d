//  proc_client_cmds.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/6/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include <stdlib.h>

#include "simulator_memory.h"
#include "sequencer_memory.h"
#include "proc_client_cmds.h"
#include "all_objects.h"

#include "truss.h"
#include "rafter.h"
#include "hinge.h"
#include "road.h"
#include "sign.h"
#include "basketball_arena.h"


#include "robot.hpp"
#include "walking_robot.h"
#include "init_samples.hpp"
#include "room2.hpp"

extern glWalkingRobot robot;



VerbalObject* new_object( long mObject_type )
{
    VerbalObject* obj = NULL;
    enum eObjectTypes  obj_type = (enum eObjectTypes) mObject_type;
    Texture* one_brick_texture=NULL;
    txtContainer*  mb=NULL;
    
    switch (obj_type)
    {
        case VERBAL_OBJECT: obj = new VerbalObject();
            break;
        case ATOM       : obj = new glAtom();
            break;
        case BOX        : obj = new glBox();
            ((glBox*)obj)->height = ipc_memory_sim->Location1.x;
            ((glBox*)obj)->width  = ipc_memory_sim->Location1.y;
            ((glBox*)obj)->depth  = ipc_memory_sim->Location1.z;
            printf("New box: hwd = <%6.2f, %6.2f, %6.2f> \n",
                   ipc_memory_sim->Location1.x, ipc_memory_sim->Location1.y, ipc_memory_sim->Location1.z );
            obj->create();
            break;
        case DOOR       : obj = new glDoor();
            break;
        case IBEAM      : obj = new glIbeam( 12*10 );
            //ipc_memory_sim->Location1.x
            //obj->m_ends_color  = 0xFFFF0000;
            obj->m_color       = 0xFFFFFFFF;
            ((glIbeam*)obj)->m_is_closed   = true;
            obj->create();
            break;
        case CYLINDER   : obj = new glCylinder((int)(ipc_memory_sim->object_datum1&0xFFFF));
            ((glCylinder*)obj)->m_radius           = ipc_memory_sim->Location1.x;
            ((glCylinder*)obj)->m_extrusion_length = ipc_memory_sim->Location1.y;
            ((glCylinder*)obj)->m_extrusion_axis   = (int)(ipc_memory_sim->object_datum2&0xFF);
            printf("New cylinder: radius,length, axis = %6.2f, %6.2f, %lu \n",
                   ipc_memory_sim->Location1.x, ipc_memory_sim->Location1.y, ipc_memory_sim->object_datum2);
            obj->create();
            break;
        case EXTRUSION  : obj = new glExtrusion();
            break;
        case PAPER      : obj = new glPaper( 10 );
            break;
        case STAIRWAY   : obj = new glStairway();
            obj->create();
            break;
        case TEXTURE_CONTAINER: obj = new txtContainer();
            obj->create();
            break;
        case ROUTE      : obj = new glRoute();
            break;
        case FULL_WALL  : obj = new glFullWall();
            ((glFullWall*)obj)->set_length( ipc_memory_sim->object_datum2 );
            obj->create();
            break;
        case CHAIR      : obj = new glChair();
            obj->create();
            break;
        case TABLE_4LEGS: obj = new glTable();
            obj->create();
            break;
        case TABLE_CENTER_POLE: obj = new glTableCenterPole();
            obj->create();
            break;
        case DRAWER      : obj = new glDrawer();
            obj->create();
            break;
        case PALLETTE    : obj = new Palette();
            obj->create();
            break;
        case BARRICADE  : obj = new glBarricade();
            obj->create();
            break;
        case BOOKCASE   : obj = new glBookcase();
            obj->create();
            break;
            
        case SPHERE : obj = new glSphere( 25, 25, 36 );
            obj->set_color(0x5f5f5f5f);
            obj->create();
            break;
            
        case PICNIC_TABLE :
            obj = new glPicnicTable();
            obj->create();
            break;
            
        case BRICK :
            one_brick_texture = new Texture();
            one_brick_texture->load_image("textures/cinder-block-texture.jpg");
            mb   = new txtContainer();
            mb->width  = 7.625;
            mb->height = 3.625;
            mb->depth  = 2.125;
            mb->set_color( 0xFF7FFF7F );
            mb->setup();
            mb->m_texture = one_brick_texture;
            mb->grab_bottom();
            mb->grab_left();
            break;
        case CABINET : obj = new glCabinet();
            obj->create();
            break;

        case TRUSS2D : obj = new glTruss2D();
            obj->create();
            break;
        case TRUSS3D : obj = new glTruss();
            obj->create();
            break;
        case TRUSSHIGHWAY : obj = new glHighwayOverhead();
            obj->create();
            break;            
        case RAFTER : obj = new glRafter();
            obj->create();
            break;
        case LIGHTSWITCH : obj = new glLightSwitch();
            obj->create();
            break;
        case HINGE : obj = new glHinge();
            obj->create();
            break;
        case ROAD : obj = new glRoad();
            obj->create();
            break;
        case TERRAIN : obj = new glTerrain();
            obj->create();
            break;
        case PAINTING : obj = new glPainting();
            obj->create();
            break;

        case BASKETBALL : obj = new glSphere(NBA_BASKET_BALL_RADIUS, 10, 64);
            obj->is_participating = true;
            obj->coef_of_restitution = COEFF_RESTITUTION_BASKETBALL;
            obj->set_color( 0xFF7F7F00);
            ((glSphere*)obj)->load_texture("textures/basketball_texture.jpg" );
            obj->create();
            break;
        case SOCCERBALL_5 : obj = new glSphere(FIFA_SOCCER_BALL_RADIUS_SIZE5, 10, 64);
            obj->is_participating = true;
            obj->coef_of_restitution = COEFF_RESTITUTION_SOCCERBALL;
            obj->set_color( 0xFFFF0000);
            ((glSphere*)obj)->load_texture("textures/soccer_ball_3.jpg" );
            obj->create();
            break;
        case SOCCERBALL_4 : obj = new glSphere(FIFA_SOCCER_BALL_RADIUS_SIZE4, 10, 64);
            obj->is_participating = true;
            obj->coef_of_restitution = COEFF_RESTITUTION_SOCCERBALL;
            obj->set_color( 0xFF5F7F2F);
            ((glSphere*)obj)->load_texture("textures/soccer_ball_world_cup.jpg" );            
            obj->create();
            break;
        case BASEBALL : obj = new glSphere(BASEBALL_RADIUS, 10, 64);
            obj->is_participating = true;
            obj->coef_of_restitution = COEFF_RESTITUTION_BASEBALL;
            obj->set_color( 0xFF0F0000);
            ((glSphere*)obj)->load_texture("textures/baseball_texture.jpg" );
            obj->create();
            break;

            
            //        case 18: obj = new glCounter();
            //            break;
            //        case 20: obj = new glBrickWall();
            break;
        case ROBOT_LEG :  obj = new glLightSwitch();
            obj->create();
            break;
        case ROBOT_ARM :  obj = new glLightSwitch();
            obj->create();
            break;
        case ROBOT :    obj = new glLightSwitch();
            obj->create();
            break;
        case STREET_SIGN : obj = new StreetSign();
            ((StreetSign*)obj)->m_sign_name = "stop";
            obj->create();
            break;
        case BASKETBALL_ARENA:  obj = new glArena();
            obj->create();
            obj->relocate( -40*12, 2, -94*12 );
            ((glArena*)obj)->create_cam_routes();
            break;
        case SKETCHUP_IMPORT :
            
            break;
/*        case 24: obj = new glArm();
            break; */
        default:
            break;
    }
    return obj;
}

vector<long> mIDs;
void print_ids()
{
    printf("ids List %lu ", mIDs.size());
    for (int i=0; i<mIDs.size(); i++)
        printf("%lu, ", mIDs[i]);
    printf("\n");
}



void perform_predefined_seq( int mSequenceNumber, int mRepetitions )
{
    //float fraction=0.;
    enum PredefinedPoses pose_num = (enum PredefinedPoses) mSequenceNumber;
    switch (pose_num)
    {
        case STAND      :    robot.stand();                  break;
        case SIT        :    robot.sit();                    break;
        case FLOOR_SIT  :    robot.floor_sit();              break;
        case ONE_KNEE_KNEEL :robot.one_knee_kneel(true);     break;
        case KNEEL      :    robot.kneel();                  break;
        case SQUAT      :    //fraction = 0.5;
                             robot.squat_fraction(mRepetitions/100.);      break;

        case HANDS_ON_HIP               :    robot.hands_on_hip();           break;
        case FOLDED_ARMS                :    robot.folded_arms();            break;
        case ZOMBIE_ARMS                :    robot.zombie_arms();            break;
        case ARMS_DOWN_BY_SIDE          :    robot.arms_down_by_side();      break;

        case ARM_STRAIGHT_UP            :    robot.arm_straight_up(true);           break;
        case ARM_STRETCHED_SIDE_LEFT    :    robot.arm_stretched_to_side_left();    break;
        case ARM_STRETCHED_SIDE_RIGHT   :    robot.arm_stretched_to_side_right();   break;

        case ATTENTION_1    :    robot.attention_1();      break;
        case ATTENTION_2    :    robot.attention_2();      break;
        default : break;
    }
    
}

extern union uGlobalVars theGlobals;

/* sim_read_move_object     (  Object_id,    MathVector& mNewLocation, MathVector& mNewOrientation );
 sim_read_new_object        (  Object_type,  int mQuantity                                     );
 sim_read_robot_move_object (  Robot_id,     MathVector mNewLocation                           );
 sim_read_move_robot        (  Robot_id,     MathVector mNewLocation                           );
 sim_read_robot_angles      (  Robot_id,     vector<int> mservo_indices, MathVector mNewAngles );
 sim_read_robot_predefined  (  Robot_id,     int mMoveSequenceIndex, int repetitions           );*/

/* The IPC data is extracted in sim_read_command().  The purpose of this function is to 
   act on that data (ie. update the objects and redisplay).
 */
void proc_sim_command()
{
    long    Robot_id   =0;
    long    Object_id  =0;
    long    Object_type=0;
    int     Quantity   =0;
    int     MoveSequenceIndex=0;
    int     Repetitions=0;
    bool    uses_source=false;
    MathVector NewLocation(3);
    MathVector NewOrientation(3);
    MathVector RobotAngles(3);
    MathVector Source(3);
    MathVector xyz(3);
    glObject*  obj = NULL;
    string searchname    = "master bedroom";
    string searchtypename = "door";
//    vector<VerbalObject*>* ptr=NULL;
    char* last_space;
    long  size;
    vector<VerbalObject*> object_hier;
    bool result=false;
    int room_index;

    struct stBodyPosition bp;
    float* bp_ptr = (float*)&bp;
    int num_floats=0;
    int num_servos=0;
    int servo_index=0;
    float ServoAngle=0;
    int global_index=0;
    
    printf( "Command=%lx \n", ipc_memory_sim->Command );    
    switch (ipc_memory_sim->Command)
    {
        case COMMAND_MOVE_OBJECT :  /* Instantaneous, no robot */
            sim_read_move_object( Object_id, NewLocation, NewOrientation );
            if (Object_id==-1)
                break;
            obj = theWorld.find_object_id( Object_id );
            if (obj==NULL)  {
                printf( "Unable to find object %lu\n", Object_id );
                break;
            }
            if ((ipc_memory_sim->object_datum1 & 0x0001)) {
                printf(" MOVE OBJECT %lu - %x\n", Object_id, obj );
                obj->relocate( NewLocation );
                NewLocation.print();
            }

            if (ipc_memory_sim->object_datum1 & 0x0002)
            {
                obj->m_x_angle = NewOrientation[0];
                obj->m_y_angle = NewOrientation[1];
                obj->m_z_angle = NewOrientation[2];
                NewOrientation.print();
            }
            break;

        case COMMAND_NEW_OBJECT :
            printf(" NEW OBJECT ");
            sim_read_new_object( Object_type, Quantity );
            mIDs.clear();
            for (int i=0; i<Quantity; i++)
            {
                obj = new_object( Object_type );
                if (obj)
                {
                    mIDs.push_back( obj->m_object_id );
                }
            }
            // Return a list of the new object's ids :
            sim_write_object_ids( mIDs );
            print_ids       ();
            theWorld.list_object_ids ();
            break;

        case COMMAND_DELETE_OBJECT :
            printf(" DELETE OBJECT ");
            sim_read_delete_object ( Object_id );
            obj = theWorld.find_object_id   ( Object_id );
            if (obj==NULL) {
                printf(" Object #%lu not found.\n", Object_id );
                strcpy (ipc_memory_sim->Response, "Object_id not found.");
                ipc_memory_sim->ResponseCounter++;
            }
            else {
                delete obj;
                //m_scene.remove((glAtom*)obj);  put back in!
                strcpy (ipc_memory_sim->Response, "Object deleted.");
                ipc_memory_sim->ResponseCounter++;
            }
            break;
        case COMMAND_WHERE_IS :            
            if (ipc_memory_sim->object_type==0)
            {
                glObject* obj = theWorld.find_object_id(ipc_memory_sim->object_id);
                xyz = obj->m_position;
            } else {
                xyz[0] =ipc_memory_sim->Location1.x;
                xyz[1] =ipc_memory_sim->Location1.y;
                xyz[2] =ipc_memory_sim->Location1.z;
            }
            room_index = scan_rooms(Rooms, xyz );
            if (room_index == -1)
                strcpy (ipc_memory_sim->Response, "Object is outside.");
            else {
                strcpy (ipc_memory_sim->Response, "Object is in the ");
                strcat (ipc_memory_sim->Response, Rooms[room_index].m_names.c_str() );
            }
            
            ipc_memory_sim->ResponseCounter++;
            break;
            
        case COMMAND_WHAT_IS_IN :
            
            break;
            
        case COMMAND_ROBOT_MOVE_OBJECT:
            // Use the robot to move an object.
            sim_read_robot_move_object( Robot_id, Object_id, NewLocation );
            printf(" ROBOT MOVE OBJECT: robot_id=%lu; object_id=%lu\t", Robot_id, Object_id);
            NewLocation.print();
            break;

        case COMMAND_ROBOT_STOP :
            robot.stop_walking();
            printf(" ROBOT STOPPED:  "); 
            break;
            
        case COMMAND_ROBOT_MOVE :
            // Walk to X Y Z:
            uses_source = sim_read_move_robot( Robot_id, Source,  NewLocation );
            printf(" ROBOT MOVE:  ");     NewLocation.print();
            if (uses_source)
            {
                printf("Reset source - ");  Source.print();
                change_route( Source, NewLocation );
            } else {
                MathVector curr(3);
                curr[0] = robot.m_x;
                curr[1] = 0.0;
                curr[2] = robot.m_z;
                change_route( curr, NewLocation );
            }
            robot.start_walking();
            break;

        case COMMAND_ROBOT_ANGLE :  /* Move a single servo to a specified position */
            sim_read_robot_angle( Robot_id, servo_index, ServoAngle );
            // how to set 1 motor?
            robot.set_servo_angle( servo_index, ServoAngle);
            printf(" ROBOT ANGLE: (%lu) servo[%d]=%6.2ff\t", Robot_id, servo_index, ServoAngle );
            break;
            
        case COMMAND_ROBOT_ANGLES : /* An Entire pose - ie all servos */
            sim_read_robot_angles( Robot_id, RobotAngles );
            num_floats = sizeof(struct stBodyPosition) / sizeof(float);
            num_servos = MIN( RobotAngles.m_elements.size(), num_floats );
            for (int i=0; i<num_servos; i++)
                bp_ptr[i] = RobotAngles[i];
            robot.set_body_pose( &bp );
            
            printf(" ROBOT ANGLES: (%lu) \t", Robot_id );
            RobotAngles.print();
            break;
            
        case COMMAND_ROBOT_PREDEFINED_SEQ :
            sim_read_robot_predefined(  Robot_id, MoveSequenceIndex, Repetitions );
            perform_predefined_seq( MoveSequenceIndex, Repetitions );
            printf(" ROBOT PREDEFINED SEQ:  (%lu) Seq #%d  Repetition=%d \n", Robot_id, MoveSequenceIndex, Repetitions );            
            break;
            
//        case QUERY_GET_OBJECT_POSITION : /* what object is located at XYZ? */
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
//            printf(" Coordinate Query: \n" );
  //          break;
        case QUERY_OBJECT_NAME:
            
            break;
            
        case QUERY_GET_OBJECT_ID :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            last_space  = strrchr(ipc_memory_sim->object_name, ' ' );
            *last_space = 0;
            searchname  = ipc_memory_sim->object_name;
            searchtypename = (last_space+1);
            result = theWorld.find_component( searchname, searchtypename, &object_hier, true );
            if (result)
            {
                /* The hierarchy will be:  [0] is the found object.
                 [1] is the parent of the found object.
                 [2] parent of parent, etc. */
                vector<long> ids;
                // write whole owner hierarchy.
                size = object_hier.size();
                for (int i=0; i<size; i++)
                    ids.push_back( object_hier[i]->m_object_id );
                sim_write_object_ids( ids );
                printf(" Found Object ID: %s %s %lu \n", searchname.c_str(),
                                            searchtypename.c_str(),
                                            object_hier[0]->m_object_id );
                // Need to map this to World Coordinates!  How?  we need the wall that it belongs to.
                // well the find_component search goes based on the hierarchy.  so it should be able to
                // compile the complete hierarchy.  ie before each recursion, store the ptr.

                // The objects coordinates (ie. the door m_x,y,z are the relative to the wall.
                MathVector coord(3);
                coord[0] = object_hier[0]->m_x;
                coord[1] = object_hier[0]->m_y;
                coord[2] = object_hier[0]->m_z;
                for (long i=1; i<size; i++)
                {
                    coord = object_hier[i]->map_coords(coord);
                    // last is the object.
                    // 2nd to last is the parent of the object.
                    // 3rd to last is the parent of the parent.
                }
                ipc_memory_sim->Location1.x = coord[0];
                ipc_memory_sim->Location1.y = coord[1];
                ipc_memory_sim->Location1.z = coord[2];
                ipc_memory_sim->ResponseCounter++;
            }
            else {
                ipc_memory_sim->num_valid_ids = 0;
                strcpy (ipc_memory_sim->Response, "Object not found!" );
                ipc_memory_sim->ResponseCounter++;
            }
            break;
        case QUERY_GET_OBJECT_POSITION :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory
            result    = theWorld.find_component_by_id( ipc_memory_sim->object_id, &object_hier );
            if (result)
            {
                /* The hierarchy will be:  [0] is the found object.
                 [1] is the parent of the found object.
                 [2] parent of parent, etc. */
                vector<long> ids;
                // write whole owner hierarchy.
                size = object_hier.size();
                for (int i=0; i<size; i++)
                    ids.push_back( object_hier[i]->m_object_id );
                sim_write_object_ids( ids );
                printf(" Found Object ID: %s %s %lu \n", searchname.c_str(),
                       searchtypename.c_str(),
                       object_hier[0]->m_object_id );
                // Need to map this to World Coordinates!  How?  we need the wall that it belongs to.
                // well the find_component search goes based on the hierarchy.  so it should be able to
                // compile the complete hierarchy.  ie before each recursion, store the ptr.
                
                // The objects coordinates (ie. the door m_x,y,z are the relative to the wall.
                MathVector coord(3);
                coord[0] = object_hier[0]->m_x;
                coord[1] = object_hier[0]->m_y;
                coord[2] = object_hier[0]->m_z;
                for (long i=1; i<size; i++)
                {
                    coord = object_hier[i]->map_coords(coord);
                    // last is the object.
                    // 2nd to last is the parent of the object.
                    // 3rd to last is the parent of the parent.
                }
                ipc_memory_sim->Location1.x = coord[0];
                ipc_memory_sim->Location1.y = coord[1];
                ipc_memory_sim->Location1.z = coord[2];
                ipc_memory_sim->ResponseCounter++;
            }
            else {
                ipc_memory_sim->num_valid_ids = 0;
                strcpy (ipc_memory_sim->Response, "Object not found!" );
                ipc_memory_sim->ResponseCounter++;
            }
            break;

        case ACTION_OBJECT_OPEN :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            result    = theWorld.find_component_by_id( ipc_memory_sim->object_id, &object_hier );
            if (result)
            {
                ((glDoor*)object_hier[0])->open( ipc_memory_sim->servo_angles[0] );
            }
            printf(" Action Open Door: \n" );
            break;

        case ACTION_OBJECT_LIFT :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Lift Object: \n" );
            break;
        case ACTION_OBJECT_PUSH :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Push Object: \n" );
            break;
        case ACTION_OBJECT_GRAB :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Grab Object: \n" );
            break;
        case ACTION_OBJECT_RELEASE :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            printf(" Action Release Object: \n" );
            break;

        case COMMAND_CHANGE_GLOBAL_VAR :
            global_index = (ipc_memory_sim->object_id & 0xFF);          // variable id (ie index into array)
            theGlobals.raw_array[global_index] = (ipc_memory_sim->object_datum1 & 0xFF);
            break;
            
        default:
            break;
    }
}

void respond_to_client_timeslice()
{
    // Check Command Counter
    bool sim = sim_new_command_available();
    bool seq = seq_new_command_available();
    if (sim) {
        printf("New Simulator Command available!\t");
        proc_sim_command();
        sim_acknowledge_command();
    }
    if (seq) {
        printf("New Sequencer Command available!\t");
        seq_acknowledge_command();
    }
}




/* Direct Robot Movement command */
void process_robot_command(  )
{
    int cmd = 0;    // ipc_memory_sim->RobotCommand
    switch (cmd)
    {
        case ROBOT_CMD_LIFT_LEG :   // 0,1,2 - left right, both
            
            break;
        case ROBOT_CMD_LIFT_ARM :
            
            break;
        case ROBOT_CMD_BEND_LEG :  // FURNITURE, MOVEABLES, etc
            
            break;
        case ROBOT_CMD_BEND_ARM :
            
            break;
        case ROBOT_CMD_MOVE_OBJ :
            
            break;
        default : break;
    }
}
void process_command(  )
{
    int cmd = 0;    // ipc_memory_sim->Command
    switch (cmd)
    {
        case CMD_MOVE_OBJ :
            break;
        case CMD_CLEAN_SLATE :
            break;
        case CMD_ONLY_SHOW :  // FURNITURE, MOVEABLES, etc.
            break;
        case CMD_ROBOT_CMD : process_robot_command();
            break;
        case CMD_CREATE_SHAPE :
            break;
        case CMD_CREATE_FILE_OBJ :  // downloaded object from sketch up or such.
            break;
            
        default:
            break;
    }
}
