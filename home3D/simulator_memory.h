#ifdef  __cplusplus
//extern "C" {
#endif

#include <vector>
#include "vector.hpp"
#include "Sim3D_defines.h"


#define IPC_KEY_SIM           1234
#define MAX_CLIENT_ARRAY_SIZE 2048

extern char* 	simulator_shared_memory;
extern int 		simulator_segment_id;
extern struct   simulator_ipc_memory_map*  ipc_memory_sim;

struct Coordinate
{
    float x;
    float y;
    float z;
};
    
#define COMMAND_MOVE_OBJECT          0x101       // Move an item in the 3D space
#define COMMAND_NEW_OBJECT           0x102       // Create a new item in 3D space
#define COMMAND_ROBOT_MOVE_OBJECT    0x103       // Pick up specified object
#define COMMAND_ROBOT_MOVE           0x104       // Relocate (walk) to an XYZ
#define COMMAND_ROBOT_ANGLES         0x105       // Move the robot to specified angles
#define COMMAND_ROBOT_PREDEFINED_SEQ 0x106       // Move robot thru a predefined sequence.
#define COMMAND_ROBOT_SEQ            0x107       //
#define COMMAND_DELETE_OBJECT        0x108       // Create a new item in 3D space
#define COMMAND_SET_VELOCITY         0x109       // Create a new item in 3D space
#define COMMAND_CHANGE_VISIBILITY    0x10A       // ie make walls invisible. (half transparency, etc)
//#define COMMAND_CHANGE_VISIBILITY    0x10A       // ie make walls invisible. (half transparency, etc)

/*const int QUERY_GET_OBJECT_ID       = 0x201;
const int QUERY_GET_OBJECT_COORD    = 0x202;
const int QUERY_COORDINATE          = 0x203; */

#define QUERY_GET_OBJECT_ID                 0x201       // string object name, returns MathVector position.
#define QUERY_GET_OBJECT_POSITION           0x202       // string object name, returns MathVector position.
#define QUERY_OBJECT_POSITION_RELATIVE      0x203       // object name, returns verbal description compared to another object.
#define QUERY_OBJECT_NAME                   0x204       // object id, return string object name.

#define ACTION_OBJECT_OPEN                  0x301       // string object name, returns MathVector position.
#define ACTION_OBJECT_LIFT	                0x302       // string object name, returns MathVector position.
#define ACTION_OBJECT_PUSH	                0x303       // string object name, returns MathVector position.
#define ACTION_OBJECT_GRAB	                0x304       // string object name, returns MathVector position.
#define ACTION_OBJECT_RELEASE               0x305       // string object name, returns MathVector position.


#define MAX_IDS 50
/******************** CLIENT CONNECTION       *****************
  This holds memory structure for the client connections (user 
  interface and control) to and from this service.
 
 This allows NLP commands to create, place, query and view
 new scenarios. 
 
 ******************** 3D SIMULATOR CLIENT MEMORY MAP *****************/

struct simulator_ipc_memory_map
{
	long int StatusCounter;
	char	 ConnectionStatus[64];

    long int AcknowledgeCounter;    // move object
    long int CommandCounter;        // move object

    long     Command;               // move object
    long     object_id;             //
    long     robot_id;              //
    long     object_type;           //
    long     object_datum1;         // depends on the command
    long     object_datum2;         //
    long     object_datum3;         //
    char	 object_name[255];		// for finding an object by name.
    
    struct Coordinate Location1;
    struct Coordinate Location2;
    float    servo_angles[MAX_SERVOS];  // Degrees.

    int      num_valid_ids;
    long     new_object_ids[MAX_IDS];
    
	long int ResponseCounter;
	char	 Response        [128];
};
//short    RobotCommand;          // move object

/******************** CLIENT MEMORY MAP *****************/
/*********************************************************/

void delete_all_shm();

bool sim_new_command_available( );
void sim_acknowledge_command  ( );
void sim_wait_for_acknowledgement( );


void sim_dump_ipc               ( );
void sim_save_segment_id        (char* mFilename);
int  sim_read_segment_id        (char* mFilename);

int  sim_allocate_memory        ( );
void sim_deallocate_memory      (int msegment_id);

long  sim_attach_memory         ( );
void  sim_reattach_memory       ( );
void  sim_detach_memory         ( );

unsigned long sim_get_segment_size( );
void sim_fill_memory            ( );
    
void sim_ipc_write_connection_status( char* mStatus   );
void sim_ipc_write_response         ( char* mSentence );

// WRITE COMMANDS TO IPC MEM:
void sim_new_object             ( long mObject_type, int mQuantity                                  );
void sim_delete_object          ( long mObject_id                                                   );
void sim_move_object            ( long mObject_id, MathVector mNewLocation, MathVector mNewOrientation );
void sim_robot_move_object      ( long mRobot_id,  long       mObject_id,   MathVector mNewLocation, MathVector mNewOrientation );
void sim_move_robot             ( long mRobot_id,  MathVector mDestination, MathVector mSource, bool mUseSource=false );
void sim_robot_angles           ( long mRobot_id,  vector<int> mservo_indices, MathVector mNewAngles );
void sim_robot_predefined       ( long mRobot_id,  int mMoveSequenceIndex, int repetitions           );
void sim_write_object_ids       ( vector<long> mIDs );
void sim_change_visibility      ( long mObject_id,   int& mParameter1, int& mParameter2 );
void sim_set_object_velocity    ( long mObject_id,   MathVector& mNewVelocity );

// READ COMMAND FROM IPC MEM:
void sim_read_command           ( long& mObject_id,   MathVector& mNewLocation, MathVector& mNewOrientation );
void sim_read_move_object       ( long& mObject_id,   MathVector& mNewLocation, MathVector& mNewOrientation );
void sim_read_new_object        ( long& mObject_type, int&         mQuantity                                );
void sim_read_delete_object     ( long& mObject_id );
void sim_read_robot_move_object ( long& mRobot_id,    long&        mObject_id,  MathVector& mNewLocation    );
bool sim_read_move_robot        ( long& mRobot_id,    MathVector&  mSource,     MathVector&  mDestination   );
void sim_read_robot_angles      ( long& mRobot_id,    MathVector& mNewAngles );
void sim_read_robot_predefined_move( long& mRobot_id,    int&         mMoveSequenceIndex,  int& repetitions    );
void sim_read_object_ids        ( vector<long>& mIDs );
void sim_read_visibility        ( long& mObject_id,   int& mParameter1, int& mParameter2 );
void sim_read_object_velocity   ( long& mObject_id,   MathVector&   mNewVelocity );

    
#ifdef  __cplusplus
//}
#endif
