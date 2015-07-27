//  sequencer_memory.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/17/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
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
	
 DATE 	:  6/17/2015
 AUTHOR	:  Stephen Tenniswood
 ********************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include "sequencer_memory.h"
#include "Sim3D_defines.h"


char* 	sequencer_shared_memory;
int 	sequencer_segment_id;
struct  sequencer_ipc_memory_map* ipc_memory_seq = NULL;

bool seq_new_command_available( )
{
    bool retval = (ipc_memory_seq->UpdateCounter > ipc_memory_seq->AcknowledgeCounter);
//    printf("%lu / %lu\n",  ipc_memory_seq->UpdateCounter,
//                           ipc_memory_seq->AcknowledgeCounter );
    return retval;
}
void seq_acknowledge_command( )
{
    ipc_memory_seq->AcknowledgeCounter++;
}

void seq_dump_ipc()
{
    int length = sizeof(struct sequencer_ipc_memory_map);
    for (int i=0; i<length; i++)
    {
        if ((i%32)==0)
            printf("\n");
        printf("%2x ", sequencer_shared_memory[i] );
    }
}

void seq_save_segment_id(char* mFilename)
{
    FILE* fd = fopen(mFilename, "w");
    //FILE* fd = fopen("sequencer_shared_memseg_id.cfg", "w");
    printf("Segment_id=%d\n", sequencer_segment_id );
    fprintf( fd, "%d", sequencer_segment_id );
    fclose( fd );
}
int seq_read_segment_id(char* mFilename)
{
    FILE* fd = fopen( mFilename, "r" );
    fscanf( fd, "%d", &sequencer_segment_id );
    fclose( fd );
    return sequencer_segment_id;
}

int seq_allocate_memory( )
{
    const int 	shared_segment_size = sizeof(struct sequencer_ipc_memory_map);
    printf("seq shared_seg_size=%d\n", shared_segment_size);
    
    /* Allocate a shared memory segment. */
    sequencer_segment_id = shmget( IPC_KEY_SEQ, shared_segment_size, IPC_CREAT | 0666 );
    int errsv = errno;
    if (errno)
        printf("%s\n", sys_errlist[errsv]);
    
    // IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    printf ("Sequencer shm segment_id=%d\n", sequencer_segment_id );
    return sequencer_segment_id;
}

long seq_attach_memory()
{
    /* Attach the shared memory segment. */
    sequencer_shared_memory = (char*) shmat (sequencer_segment_id, 0, 0);
    ipc_memory_seq			 = (struct sequencer_ipc_memory_map*)sequencer_shared_memory;
    printf ("Sequencer shm attached at address %p\n\n", sequencer_shared_memory);
    return (long)sequencer_shared_memory;
}

void seq_reattach_memory()
{
    /* Reattach the shared memory segment, at a different address. */
    sequencer_shared_memory = (char*) shmat (sequencer_segment_id, (void*) 0x5000000, 0);
    ipc_memory_seq			 = (struct sequencer_ipc_memory_map*)sequencer_shared_memory;
    printf ("Sequencer shm reattached at address %p\n", sequencer_shared_memory);
}

void seq_detach_memory()
{
    /* Detach the shared memory segment. */
    shmdt (sequencer_shared_memory);
}

unsigned long seq_get_segment_size()
{
    struct 		shmid_ds	shmbuffer;
    /* Determine the segment’s size. */
    shmctl (sequencer_segment_id, IPC_STAT, &shmbuffer);
    unsigned long segment_size = shmbuffer.shm_segsz;
    printf ("Sequencer segment size: %lu\n", segment_size);
    return segment_size;
}

void seq_fill_memory()
{
    memset(sequencer_shared_memory, 0, seq_get_segment_size());
}

void seq_deallocate_memory(int msegment_id)
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
void seq_ipc_write_response( char* mSentence )
{
    long length = strlen(mSentence);
    int MaxAllowedLength = sizeof(ipc_memory_seq->Response);
    if (length>MaxAllowedLength)
        length = MaxAllowedLength;
    
    ipc_memory_seq->ResponseCounter++;
    
    //printf("%d:Copying %d bytes to shared mem.\n", SentenceCounter, length);
    strcpy(ipc_memory_seq->Response, mSentence);
    printf("|%s|\n", ipc_memory_seq->Response );
    //dump_ipc();
}

void ipc_write_connection_status( char* mStatus )
{
    long length = strlen(mStatus);
    int MaxAllowedLength = sizeof(ipc_memory_seq->ConnectionStatus);
    if (length > MaxAllowedLength)
        length = MaxAllowedLength;	// do not write into memory of next variable.
    
    ipc_memory_seq->StatusCounter++;
    
    //printf("%d:Copying %d bytes to shared mem.\n", StatusCounter, length );
    strcpy( ipc_memory_seq->ConnectionStatus, mStatus);
    printf( "|%s|\n", ipc_memory_seq->ConnectionStatus );
}

void ipc_write_active_page( short NewActivePage )
{
    //	ipc_memory_sim->ScreenNumber = NewActivePage;
}
/* See udp_transponder for update_client_list()		*/


// Return true if added.
bool ipc_add_sequence           ( struct one_vector  mVector )
{
    if (ipc_memory_seq == NULL)  return false;
    
    int index = ipc_memory_seq->number_of_vectors++;
    if (ipc_memory_seq->number_of_vectors >= MAX_SEQUENCE_LENGTH)
        ipc_memory_seq->number_of_vectors = 0;
    
    return ipc_write_sequence( index, mVector);
}

bool ipc_write_sequence     ( int mIndex, struct one_vector  mVector )
{
    if ((mIndex >= MAX_SEQUENCE_LENGTH) || (ipc_memory_seq == NULL))
        return false;

    ipc_memory_seq->sequence[mIndex] = mVector;
    return true;
}

bool ipc_write_instance_index( int mIndex, unsigned char mInstance )
{
    if (mIndex >= MAX_SERVOS)
        return false;
    
    ipc_memory_seq->instance_mapping[mIndex] = mInstance;
    return true;
}

