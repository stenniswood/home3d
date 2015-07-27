//  proc_client_cmds.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/6/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "simulator_memory.h"
#include "sequencer_memory.h"
#include "proc_client_cmds.h"
#include "all_objects.h"




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

VerbalObject* new_object( long mObject_type )
{
    VerbalObject* obj = NULL;
    
    switch (mObject_type)
    {
        case 1: obj = new VerbalObject();
            break;
        case 2: obj = new glAtom();
            break;
        case 3: obj = new glBox();
            ((glBox*)obj)->height = ipc_memory_sim->Location1.x;
            ((glBox*)obj)->width  = ipc_memory_sim->Location1.y;
            ((glBox*)obj)->depth  = ipc_memory_sim->Location1.z;
            printf("New box: hwd = <%6.2f, %6.2f, %6.2f> \n",
                   ipc_memory_sim->Location1.x, ipc_memory_sim->Location1.y, ipc_memory_sim->Location1.z );
            obj->create();
            break;
        case 4: obj = new glDoor();
            break;
        case 5: obj = new glIbeam( ipc_memory_sim->Location1.x );
            printf("New ibeam: %6.2f \n", ipc_memory_sim->Location1.x );
            obj->create();
            break;
        case 6: obj = new glCylinder((int)(ipc_memory_sim->object_datum1&0xFFFF));
            ((glCylinder*)obj)->m_radius           = ipc_memory_sim->Location1.x;
            ((glCylinder*)obj)->m_extrusion_length = ipc_memory_sim->Location1.y;
            ((glCylinder*)obj)->m_extrusion_axis   = (int)(ipc_memory_sim->object_datum2&0xFF);
            printf("New cylinder: radius,length, axis = %6.2f, %6.2f, %lu \n",
                   ipc_memory_sim->Location1.x, ipc_memory_sim->Location1.y, ipc_memory_sim->object_datum2);
            obj->create();
            break;
        case 7: obj = new glExtrusion();
            break;
        case 8: obj = new glPaper( 10 );
            break;
        case 9: obj = new glStairway();
            obj->create();
            break;
        case 10: obj = new txtContainer();
            obj->create();
            break;
        case 11: obj = new glRoute();
            break;
        case 12: obj = new glFullWall();
            obj->create();
            break;
        case 13: obj = new glChair();
            obj->create();
            break;
        case 14: obj = new glTableCenterPole();
            obj->create();
            break;
//        case 15: obj = new glTable();
  //          obj->create();
    //        break;
        case 16: obj = new glDrawer();
            obj->create();
            break;
        case 17: obj = new Palette();
            obj->create();
            break;
        case 18: obj = new glBarricade();
            obj->create();
            break;
//        case 21: obj = new glLightSwitch();
//            break;
        case 23: obj = new glBookcase();            
            obj->create();
            break;

/*      case 18: obj = new glCounter();
            break;
        case 19: obj = new glCabinet();
            break;
        case 20: obj = new glBrickWall();
            break;
        case 24: obj = new glArm();
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
    glObject*  obj = NULL;
    string searchname    = "master bedroom";
    string searchtypename = "door";
    VerbalObject* ptr=NULL;
    

    printf( "Command=%lx \n", ipc_memory_sim->Command );
    
    switch (ipc_memory_sim->Command)
    {
        case COMMAND_MOVE_OBJECT :
            sim_read_move_object( Object_id, NewLocation, NewOrientation );
            obj = theWorld.find_object_id( Object_id );
            printf(" MOVE OBJECT %lu - %x\n", Object_id, obj );
            obj->relocate( NewLocation );
            obj->m_x_angle = NewOrientation[0];
            obj->m_y_angle = NewOrientation[1];
            obj->m_z_angle = NewOrientation[2];
            NewLocation.print();
            NewOrientation.print();
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
            if (obj==NULL)
                printf(" Object #%lu not found.\n", Object_id );
            else {
                delete obj;
                //m_scene.remove((glAtom*)obj);  put back in!
            }
            break;
        case COMMAND_ROBOT_MOVE_OBJECT:
            sim_read_robot_move_object( Robot_id, Object_id, NewLocation );
            printf(" ROBOT MOVE OBJECT: robot_id=%lu; object_id=%lu\t", Robot_id, Object_id);
            NewLocation.print();
            break;
        case COMMAND_ROBOT_MOVE :
            uses_source = sim_read_move_robot( Robot_id, Source,  NewLocation );
            printf(" ROBOT MOVE:  ");     NewLocation.print();
            if (uses_source)
            {
                printf("Reset source - ");  Source.print();
                change_route( Source, NewLocation );
            } else {
                MathVector curr(3);
                curr[0] = robot.m_x;
                curr[1] = robot.m_y;
                curr[2] = robot.m_z;
                change_route( curr, NewLocation );
            }
            break;
        case COMMAND_ROBOT_ANGLES :
            sim_read_robot_angles( Robot_id, RobotAngles );
            printf(" ROBOT ANGLES: (%lu) \t", Robot_id );
            RobotAngles.print();
            break;
        case COMMAND_ROBOT_PREDEFINED_SEQ :
            sim_read_robot_predefined_move(  Robot_id, MoveSequenceIndex, Repetitions );
            printf(" ROBOT PREDEFINED SEQ:  (%lu) Seq #%d  Repetition=%d \n", Robot_id, MoveSequenceIndex, Repetitions );            
            break;
            
        case QUERY_COORDINATE : /* what object is located at XYZ? */
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.

            printf(" Coordinate Query: \n" );
            break;
        case QUERY_GET_OBJECT_ID :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            ptr = theWorld.find_component( searchname, searchtypename );
            if (ptr)
            {
                vector<long> ids;
                ids.push_back(ptr->m_object_id);
                sim_write_object_ids( ids );
                printf(" Find Object ID: %lu \n", ptr->m_object_id );
            }
            break;
        case QUERY_GET_OBJECT_COORD :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory
            ptr = theWorld.find_component( searchname, searchtypename );
            if (ptr)
                printf(" Object Coordinate: <%6.1f, %6.1f, %6.1f> \n", ptr->m_x, ptr->m_y, ptr->m_z );
            break;
            

        case ACTION_OPEN_DOOR :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Open Door: \n" );
            break;

        case ACTION_LIFT_OBJECT :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Lift Object: \n" );
            break;
        case ACTION_PUSH_OBJECT :
            // User may request coordinate of bedroom door, or kitchen sink, or living room center.
            // The response will be a variable number of coordinates in shared memory.
            
            printf(" Action Push Object: \n" );
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




