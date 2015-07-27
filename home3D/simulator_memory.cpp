/*********************************************************************
Product of Beyond Kinetics, Inc
------------------------------------------
|			Atmel						 |
------------------------------------------
			|SPI|
------------------------------------------
|   atiltcam   |<==>|  bkInstant (wifi)  |
------------------------------------------

This code handles IPC Shared memory for graphical display:  visual
Intended for the PiCamScan board on RaspberryPi.

WRITES TO SHARED MEMORY:
	The "abkInstant" establishes the memory segment.
	and the avisual may connect to it when it's run.
READS:
	
DATE 	:  8/8/2013
AUTHOR	:  Stephen Tenniswood
********************************************************************/
#include <stdio.h> 
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

//#include "bk_system_defs.h"
//#include "interrupt.h"
#include "simulator_memory.h"


char* 	simulator_shared_memory;
int 	simulator_segment_id;
struct  simulator_ipc_memory_map* ipc_memory_sim=NULL;

bool sim_new_command_available( )
{
    bool retval = (ipc_memory_sim->CommandCounter > ipc_memory_sim->AcknowledgeCounter);
    /*    printf("%lu / %lu\n",  ipc_memory_sim->CommandCounter,
                               ipc_memory_sim->AcknowledgeCounter ); */
    return retval;
}

void sim_acknowledge_command( )
{
    ipc_memory_sim->AcknowledgeCounter++;
}

void sim_wait_for_acknowledgement( )
{
    bool still_available=sim_new_command_available();
    while( still_available )
    {
        still_available = sim_new_command_available();
    }
}

void sim_dump_ipc()
{
	int length = sizeof(struct simulator_ipc_memory_map);
	for (int i=0; i<length; i++)
	{
		if ((i%32)==0)
			printf("\n");
		printf("%x ", simulator_shared_memory[i] );
	}	
}

void sim_save_segment_id(char* mFilename)
{
	FILE* fd = fopen(mFilename, "w");
	//FILE* fd = fopen("simulator_shared_memseg_id.cfg", "w");
	printf("Segment_id=%d\n", simulator_segment_id );
	fprintf( fd, "%d", simulator_segment_id );
	fclose( fd );
}
int sim_read_segment_id(char* mFilename)
{
	FILE* fd = fopen( mFilename, "r" );
	fscanf( fd, "%d", &simulator_segment_id );
	fclose( fd );
	return simulator_segment_id;
}

void delete_all_shm()
{
    for (int id=0; id < INT_MAX; id++)
        shmctl(id, IPC_RMID, NULL);
}
int sim_allocate_memory( )
{
	const int 	shared_segment_size = sizeof(struct simulator_ipc_memory_map);
    printf("sim shared_seg_size=%d\n", shared_segment_size);

	/* Allocate a shared memory segment. */
	simulator_segment_id = shmget( IPC_KEY_SIM, shared_segment_size, IPC_CREAT | 0666 );
    int errsv = errno;
    if (errno>0)
        //perror(sys_errlist[errsv]);
        printf("%s\n", sys_errlist[errsv]);
    
	// IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	printf ("Sim3D shm segment_id=%d\n", simulator_segment_id );
	return simulator_segment_id;	
}

long sim_attach_memory()
{
	/* Attach the shared memory segment. */
	simulator_shared_memory = (char*) shmat (simulator_segment_id, 0, 0);
	ipc_memory_sim			= (struct simulator_ipc_memory_map*)simulator_shared_memory;
	printf ("Sim3D shm attached at address %p\n\n", simulator_shared_memory);
    return (long)simulator_shared_memory;
}

void sim_reattach_memory()
{
	/* Reattach the shared memory segment, at a different address. */ 
	simulator_shared_memory = (char*) shmat (simulator_segment_id, (void*) 0x5000000, 0); 
	ipc_memory_sim			= (struct simulator_ipc_memory_map*)simulator_shared_memory;
	printf ("Sim3D shm reattached at address %p\n", simulator_shared_memory);
}

void sim_detach_memory()
{
	/* Detach the shared memory segment. */
	shmdt (simulator_shared_memory);
}

unsigned long sim_get_segment_size()
{
	struct 		shmid_ds	shmbuffer;
	/* Determine the segment’s size. */
	shmctl (simulator_segment_id, IPC_STAT, &shmbuffer);
	unsigned long segment_size = shmbuffer.shm_segsz;
	printf ("Sim3D segment size: %lu\n", segment_size);
	return segment_size;
}

void sim_fill_memory()
{
    long size = sim_get_segment_size();
	memset(simulator_shared_memory, 0, size);

//    for (int i=0; i<size; i++)
//        simulator_shared_memory[i] = i%255;    
}

void sim_deallocate_memory(int msegment_id)
{
	/* Deallocate the shared memory segment. */ 
	shmctl (msegment_id, IPC_RMID, 0);
}

/* WRITE IMPLIES TO SHARED MEMORY.  And since we are the abkInstant task,
	the writes will be transfer to avisual
FORMAT:
	INT 		ID
	char[255] 	Buffer
*/
void ipc_write_response( char* mSentence )
{
	long length = strlen(mSentence);
	int MaxAllowedLength = sizeof(ipc_memory_sim->Response);
	if (length>MaxAllowedLength)
		length = MaxAllowedLength;
		
	ipc_memory_sim->ResponseCounter++;

	//printf("%d:Copying %d bytes to shared mem.\n", SentenceCounter, length);
	strcpy(ipc_memory_sim->Response, mSentence);
	printf("|%s|\n", ipc_memory_sim->Response );
}

void sim_ipc_write_connection_status( char* mStatus )
{
	long length = strlen(mStatus);
	int MaxAllowedLength = sizeof(ipc_memory_sim->ConnectionStatus);
	if (length > MaxAllowedLength)
		length = MaxAllowedLength;	// do not write into memory of next variable.

	ipc_memory_sim->StatusCounter++;

	//printf("%d:Copying %d bytes to shared mem.\n", StatusCounter, length );
	strcpy( ipc_memory_sim->ConnectionStatus, mStatus);
	printf( "|%s|\n", ipc_memory_sim->ConnectionStatus );
}

void sim_ipc_write_active_page( short NewActivePage )
{
//	ipc_memory_sim->ScreenNumber = NewActivePage;
}
/* See udp_transponder for update_client_list()		*/


/**************************************/
/* WRITE TO SHARED MEMORY - COMMANDS  */
/**************************************/
void sim_move_object( long mObject_id, MathVector mNewLocation, MathVector mNewOrientation )
{
    ipc_memory_sim->Command = COMMAND_MOVE_OBJECT;

    ipc_memory_sim->object_id   = mObject_id;
    ipc_memory_sim->Location1.x = mNewLocation[0];
    ipc_memory_sim->Location1.y = mNewLocation[1];
    ipc_memory_sim->Location1.z = mNewLocation[2];

    ipc_memory_sim->Location2.x = mNewOrientation[0];
    ipc_memory_sim->Location2.y = mNewOrientation[1];
    ipc_memory_sim->Location2.z = mNewOrientation[2];
    ipc_memory_sim->CommandCounter++;
}
void sim_new_object( long mObject_type, int mQuantity )
{
    ipc_memory_sim->Command = COMMAND_NEW_OBJECT;

    ipc_memory_sim->object_type   = mObject_type;
    ipc_memory_sim->object_datum1 = mQuantity;      //
    ipc_memory_sim->CommandCounter++;
}
void sim_delete_object( long mObject_id )
{
    ipc_memory_sim->Command = COMMAND_DELETE_OBJECT;
    ipc_memory_sim->object_id = mObject_id;
    ipc_memory_sim->CommandCounter++;
}

void sim_robot_move_object( long mRobot_id, long mObject_id, MathVector mNewLocation, MathVector mNewOrientation )
{
    ipc_memory_sim->Command = COMMAND_ROBOT_MOVE_OBJECT;
    ipc_memory_sim->object_id     = mObject_id;
    ipc_memory_sim->robot_id      = mRobot_id;
    ipc_memory_sim->Location1.x   = mNewLocation[0];
    ipc_memory_sim->Location1.y   = mNewLocation[1];
    ipc_memory_sim->Location1.z   = mNewLocation[2];

    ipc_memory_sim->Location2.x   = mNewOrientation[0];
    ipc_memory_sim->Location2.y   = mNewOrientation[1];
    ipc_memory_sim->Location2.z   = mNewOrientation[2];
    
    ipc_memory_sim->CommandCounter++;
}

void sim_move_robot     ( long mRobot_id, MathVector mDestination, MathVector mSource, bool mUseSource )
{
    ipc_memory_sim->Command  = COMMAND_ROBOT_MOVE;
    ipc_memory_sim->robot_id = mRobot_id;
    ipc_memory_sim->object_datum1 = 1;
    if (mUseSource)
    {
        ipc_memory_sim->object_datum1 = 2;
        ipc_memory_sim->Location1.x   = mSource[0];
        ipc_memory_sim->Location1.y   = mSource[1];
        ipc_memory_sim->Location1.z   = mSource[2];
    }
    ipc_memory_sim->Location2.x   = mDestination[0];
    ipc_memory_sim->Location2.y   = mDestination[1];
    ipc_memory_sim->Location2.z   = mDestination[2];
    ipc_memory_sim->CommandCounter++;
}

void sim_robot_angles( long mRobot_id, MathVector mNewAngles )
{
    ipc_memory_sim->Command = COMMAND_ROBOT_ANGLES;
    long size = mNewAngles.m_elements.size();
    for (int i=0; i<size; i++)
        ipc_memory_sim->servo_angles[i] = mNewAngles[i];
    ipc_memory_sim->CommandCounter++;
}

void sim_robot_predefined( long mRobot_id, int mMoveSequenceIndex, int mRepetitions )
{
    ipc_memory_sim->Command = COMMAND_ROBOT_PREDEFINED_SEQ;
    ipc_memory_sim->object_datum1 = mRobot_id;
    ipc_memory_sim->object_datum2 = mMoveSequenceIndex;
    ipc_memory_sim->object_datum3 = mRepetitions;
    ipc_memory_sim->CommandCounter++;
}

void sim_write_object_ids( vector<long> mIDs )
{
    long size = mIDs.size();
    if (size>MAX_IDS) size = MAX_IDS;
    
    ipc_memory_sim->num_valid_ids = (int)(size&0xFF);
    for (int i=0; i<size; i++)
        ipc_memory_sim->new_object_ids[i] = mIDs[i];
}

/***********************************************************/
/* READ FROM SHARED MEMORY - COMMANDS                      */
/***********************************************************/
void sim_read_command( long& mObject_id, long& mObject_type, MathVector& mNewLocation, MathVector& mNewOrientation )
{
    long Acknowledges = ipc_memory_sim->AcknowledgeCounter;
    int  Quantity        =0;
    long Robot_id        =0;
    int MoveSequenceIndex=0;
    int Repetitions      =0;
    MathVector NewAngles;
    std::vector<int>  servo_indices;
    Acknowledges++;
    switch (ipc_memory_sim->Command)
    {
        case COMMAND_MOVE_OBJECT :
            sim_read_move_object    ( mObject_id, mNewLocation, mNewOrientation );
            break;
        case COMMAND_NEW_OBJECT :
            sim_read_new_object     ( mObject_type, Quantity );
            break;
        case COMMAND_ROBOT_MOVE_OBJECT :
            sim_read_robot_move_object(  Robot_id, mObject_id, mNewLocation );            
            break;
        case COMMAND_ROBOT_MOVE :
            //sim_read_move_robot   (  Robot_id, mNewLocation, mSource, true );
            break;
        case COMMAND_ROBOT_ANGLES :
            sim_read_robot_angles   ( Robot_id, NewAngles );
            break;
        case COMMAND_ROBOT_PREDEFINED_SEQ :
            sim_read_robot_predefined_move(  Robot_id, MoveSequenceIndex, Repetitions );
            break;
        default: Acknowledges--;        // No acknowledge!
            break;
    }
    ipc_memory_sim->AcknowledgeCounter = Acknowledges;
}
void sim_read_move_object( long& mObject_id, MathVector& mNewLocation, MathVector& mNewOrientation )
{
    if (ipc_memory_sim->Command == COMMAND_MOVE_OBJECT)
    {
        mObject_id      =   ipc_memory_sim->object_id;
        mNewLocation[0] =   ipc_memory_sim->Location1.x;
        mNewLocation[1] =   ipc_memory_sim->Location1.y;
        mNewLocation[2] =   ipc_memory_sim->Location1.z;
        mNewOrientation[0] = ipc_memory_sim->Location2.x;
        mNewOrientation[1] = ipc_memory_sim->Location2.y;
        mNewOrientation[2] = ipc_memory_sim->Location2.z;
    }
}
void sim_read_delete_object( long& mObject_id )
{
    if (ipc_memory_sim->Command == COMMAND_DELETE_OBJECT)
    {
        mObject_id = ipc_memory_sim->object_id;
    }
}
void sim_read_new_object     ( long& mObject_type, int& mQuantity )
{
    if (ipc_memory_sim->Command == COMMAND_NEW_OBJECT)
    {
        mObject_type = ipc_memory_sim->object_type;
        mQuantity    = (int)(ipc_memory_sim->object_datum1&0xFFFF);
    }
}

void sim_read_robot_move_object( long& mRobot_id, long& mObject_id, MathVector& mNewLocation )
{
    if (ipc_memory_sim->Command == COMMAND_ROBOT_MOVE_OBJECT)
    {
        mRobot_id       =   ipc_memory_sim->robot_id;
        mObject_id      =   ipc_memory_sim->object_id;
        mNewLocation[0] =   ipc_memory_sim->Location1.x;
        mNewLocation[1] =   ipc_memory_sim->Location1.y;
        mNewLocation[2] =   ipc_memory_sim->Location1.z;
    }
}

bool sim_read_move_robot( long& mRobot_id, MathVector&  mSource, MathVector&  mDestination )
{
    if (ipc_memory_sim->Command == COMMAND_ROBOT_MOVE)
    {
        mRobot_id  =   ipc_memory_sim->robot_id;
        mDestination[0] =   ipc_memory_sim->Location2.x;
        mDestination[1] =   ipc_memory_sim->Location2.y;
        mDestination[2] =   ipc_memory_sim->Location2.z;
        if (ipc_memory_sim->object_datum1==2)
        {
            mSource[0] =   ipc_memory_sim->Location1.x;
            mSource[1] =   ipc_memory_sim->Location1.y;
            mSource[2] =   ipc_memory_sim->Location1.z;
            return true;
        }
    }
    return false;
}

void sim_read_robot_angles    ( long& mRobot_id, MathVector& mNewAngles )
{
    if (ipc_memory_sim->Command == COMMAND_ROBOT_ANGLES)
    {
        mRobot_id     =   ipc_memory_sim->robot_id;
        mNewAngles.dimension(3);
        int dimension =   (ipc_memory_sim->object_datum1 & 0xFF);
        mNewAngles.dimension(dimension);
        mNewAngles[0] =   ipc_memory_sim->Location1.x;
        mNewAngles[1] =   ipc_memory_sim->Location1.y;
        mNewAngles[2] =   ipc_memory_sim->Location1.z;
    }
}

void sim_read_robot_predefined_move( long& mRobot_id, int& mMoveSequenceIndex, int& repetitions )
{
    if (ipc_memory_sim->Command == COMMAND_ROBOT_PREDEFINED_SEQ)
    {
        mRobot_id       =   ipc_memory_sim->robot_id;
        mMoveSequenceIndex= (int)(ipc_memory_sim->object_datum1&0xFFFF);
        repetitions     =   (int)(ipc_memory_sim->object_datum2&0xFFFF);
    }
}

void sim_read_object_ids( vector<long>& mIDs )
{
    int size = ipc_memory_sim->num_valid_ids;
    if (size > MAX_IDS) size = MAX_IDS;
    for (int i=0; i<size; i++)
        mIDs.push_back(ipc_memory_sim->new_object_ids[i]);
}

void sim_change_visibility      ( long mObject_id,   int& mParameter1, int& mParameter2 )
{
    ipc_memory_sim->Command       = COMMAND_CHANGE_VISIBILITY;
    ipc_memory_sim->object_id     = mObject_id;
    ipc_memory_sim->object_datum1 = mParameter1;
    ipc_memory_sim->object_datum2 = mParameter2;
    ipc_memory_sim->CommandCounter++;
}
void sim_set_object_velocity    ( long mObject_id,   MathVector& mNewVelocity )
{
    ipc_memory_sim->Command     = COMMAND_SET_VELOCITY;
    ipc_memory_sim->object_id   = mObject_id;
    ipc_memory_sim->Location1.x = mNewVelocity[0];
    ipc_memory_sim->Location1.y = mNewVelocity[1];
    ipc_memory_sim->Location1.z = mNewVelocity[2];
    ipc_memory_sim->CommandCounter++;
}

void sim_read_visibility        ( long& mObject_id,   int& mParameter1, int& mParameter2 )
{
    if (ipc_memory_sim->Command == COMMAND_CHANGE_VISIBILITY)
    {
        mObject_id      = ipc_memory_sim->object_id;
        mParameter1     = (int)(ipc_memory_sim->object_datum1&0xFFFF);
        mParameter2     = (int)(ipc_memory_sim->object_datum2&0xFFFF);
    }
}
void sim_read_object_velocity   ( long& mObject_id,   MathVector&   mNewVelocity )
{
    if (ipc_memory_sim->Command == COMMAND_SET_VELOCITY)
    {
        mObject_id      = ipc_memory_sim->object_id;
        mNewVelocity[0] = ipc_memory_sim->Location1.x;
        mNewVelocity[1] = ipc_memory_sim->Location1.y;
        mNewVelocity[2] = ipc_memory_sim->Location1.z;
    }
}
