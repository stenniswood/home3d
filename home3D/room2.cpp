#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include "all_objects.h"


#define TOP_AND_BOTTOM 2
#define CLOSE_OUT 1


glRoom::glRoom( )
{
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_color = 0xFFFFFFFF;
	//m_is_closed = false;
}



void glRoom::create_apartment()
{
    glDoor* tdoor ;
    
    glBareWall* bw;
	static glFullWall* w;
	struct stSize s;		// for windows
    //w->set_texture(1);

	// Outside wall (sliding door)
    w = new glFullWall();
    w->set_length(144.+36.+116.+120.);
	s.width = 46.25;    s.height = 46.25;
	w->m_bare_wall.add_window( 66.5+144.+36.   , s, 33.5 );
	w->m_bare_wall.add_window( 144.+36.+116.+3., s, 33.5 );
	w->create(  );
    //m_components.push_back( w );
	w->m_y_angle = 0.0;
	w->m_z = 0.;
	w->m_x = 0.;
	m_fwalls.push_back( w );

    // Hall way 1 :
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    tdoor = new glDoor();                   // Door #1
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 122.     , tdoor );
    tdoor = new glDoor();                   // Door #2
    tdoor->set_params(30.);
    tdoor->set_hinge     (true );
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 122.+40.5, tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
	w->create    (  );
	w->m_y_angle = 0.0;
	w->m_z = 160.;
	w->m_x = 144.;
	m_fwalls.push_back( w );

	// Hall way 2 :
    w = new glFullWall();
    w->clear();
    w->set_length(36.+116.+120.-77.5);
    tdoor = new glDoor();                       // Door #3
    tdoor->set_params( 28.);
    tdoor->set_hinge     (true);
    tdoor->set_swing_side(false);
    tdoor->setup         (     );
    tdoor->open          ( 0.1 );
    w->add_door( 164.5, tdoor );
	//w->add_door( 164.5+52.,28. );		 // really need short inside walls
	w->create(  );                       // length = 194.5
	w->m_y_angle = 0.0;
	w->m_z = 160.+36.;                   // 196
	w->m_x = 144.;                       //
	m_fwalls.push_back( w );

	// Final (kitchen to neighbors)
    w = new glFullWall();
    w->clear();
    w->set_length(144.+36.+116.+120.);
	w->create(  );                       // 416
	w->m_y_angle = 0.0;
	w->m_z = 160.+36.+95.;               // 291 
	w->m_x = 0.;
	m_fwalls.push_back( w );

	////////////////////////// horizontals //////////////////////////
	// End of Hallway
    w = new glFullWall();
    w->clear();
    w->set_length( 70. );
    w->create    (     );
	w->m_y_angle = -90.0;	
	w->m_z = 160.; 
	w->m_x = 144.+122.+40.5+30.+2.;			// 338.5
	m_fwalls.push_back( w );

	// Bathroom
    w = new glFullWall();
    w->clear();
    w->set_length( 416. - 338.5 );
    tdoor = new glDoor();                       // Door #3
    tdoor->set_params( 28.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(false);
    tdoor->setup         (     );
    tdoor->open          ( 0.1 );
    w->add_door( 19.5, tdoor    );
	w->create  (  );
	w->m_y_angle = 0.0;	
	w->m_z = 160.+70.;                       // 230
	w->m_x = 144.+122.+40.5+30.+2.;          // 338
	m_fwalls.push_back( w );

	// Divide (living & bed1)
    w = new glFullWall();
    w->clear();
    w->set_length( 160. );
	w->create();
	w->m_y_angle = -90.0;	
	w->m_z = 0.;
	w->m_x = 144.;	
	m_fwalls.push_back( w );
	
	// Divide (bed1 & bed2)
    w = new glFullWall();
    w->clear();
    w->set_length( 160. );
	w->create( );
	w->m_y_angle = -90.0;	
	w->m_z = 0.;
	w->m_x = 144.+36.+116.;             // 296
	m_fwalls.push_back( w );

	// Outside End
    w = new glFullWall();
    w->clear();
    w->set_length( 160.+36.+95. );
    s.width = 34.0;  s.height = 34.0;
	w->m_bare_wall.add_window( 97.5+6., s, 46.0 );
	w->create    (  );
	w->m_y_angle = -90.0;
	w->m_z = 0.;
	w->m_x = 144.+36.+116.+120.;		// 416
	m_fwalls.push_back( w );

	// Inside End:
    w = new glFullWall();
    w->clear();
    w->set_length( 160.+36.+95. );
    tdoor = new glDoor();                   // Door #2
    tdoor->set_params(30.);
    tdoor->set_hinge     (false );
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.5 );
	w->add_door( 160.+18.+5., tdoor  );		// Main Entrance Door
	w->create(  );
	w->m_y_angle = -90.0;
	w->m_z = 0.;
	w->m_x = 0.;
	m_fwalls.push_back( w );
	
	// Divide (kitchen & bath)
    w = new glFullWall();
    w->clear();
    w->set_length(  95.0  );

    tdoor = new glDoor();                   // Door #2
    tdoor->set_params(24.);
    tdoor->set_hinge     (true );
    tdoor->set_swing_side(false);
    tdoor->setup         (     );
    tdoor->open          ( 0.2 );
    w->add_door( 33.0, tdoor ); 	// pantry door.
	w->create( );
	w->m_y_angle = -90.0;	
	w->m_z = 160.+36.;
	w->m_x = 144.+114;
	m_fwalls.push_back( w );

	// Divide (pantry & linen)
    w = new glFullWall();
    w->clear();
    w->set_length( 31.5 );
    w->create(  );
	w->m_y_angle = 0.0;	
	w->m_z = 160.+36.+26;
	w->m_x = 144.+114;
	m_fwalls.push_back( w );
	
	// Divide (kitchen & bath)
    w = new glFullWall();
    w->clear();
    w->set_length( 95.0 );
    tdoor = new glDoor();
    tdoor->set_params(23.5);            // Door #2
    tdoor->set_hinge     (true );
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.2 );
    w->add_door( 2.0, tdoor ); 	// linen closet "door"
	w->create(  );
	w->m_y_angle = -90.0;	
	w->m_z = 160.+36.;
	w->m_x = 144.+114+31.5;
	m_fwalls.push_back( w );
	
	// Stub (entry):
    w = new glFullWall();
    w->clear();
    w->set_length( 70.5-4.25);  //, 43.
	w->create(  );
	w->m_y_angle = 0.0;	
	w->m_z = 160.+18.;
	w->m_x = 0.;	
	m_fwalls.push_back( w );

	// Post :
    w = new glFullWall();
    w->clear();
    w->set_length( 4.25 );
	w->create(  );
	w->m_y_angle = 0.0;
	w->m_z = 160.+18.;
	w->m_x = 70.5-4.25;
	m_fwalls.push_back( w );

	// entry closet :
    w = new glFullWall();
    w->clear();
    w->set_length( 70.5 );      // Critical Note: This wall length has to be set prior to add door!
								// Got lucky above because the length is not normally set until create()
    tdoor = new glDoor();
    tdoor->set_params(28.);
    tdoor->set_hinge     (false );
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.2 );
	w->add_door( 35., tdoor ); 	// linen closet "door"
	w->create(  );
	w->m_y_angle = -90.0;	
	w->m_z       = 160.+36.+95.-70.5;
	w->m_x       = 74.75;
	m_fwalls.push_back( w );

    w = new glFullWall();
    w->clear();
    w->set_length( 74.75 );
    w->create(  );
	w->m_y_angle = 0.0;
	w->m_z = 160.+36.+95.-70.5;
	w->m_x = 0.;
	m_fwalls.push_back( w );

   // add_walls();
}

void glRoom::create_room ( float width, float length )
{
    glFullWall* w = new glFullWall();
    
    // Wall #1
    w->set_length    ( width );
    w->create        (       );
    w->relocate      ( 0,0,0 );
    m_fwalls.push_back( w );

    // Wall #2
    w = new glFullWall();
    w->set_length    ( width );
    w->create        (       );
    w->relocate      ( 0,0,length );
    m_fwalls.push_back( w );

    // Wall #3
    w = new glFullWall();
    w->set_length    ( length );
    w->create        (        );
    w->relocate      ( 0,0,length );
    w->m_y_angle = 90;
    m_fwalls.push_back( w );

    // Wall #4
    w = new glFullWall();
    w->set_length    ( length );
    w->create        (        );
    w->relocate      ( width,0,length );
    w->m_y_angle = 90;
    m_fwalls.push_back( w );
}



/* The wall from 2D point of view consists of:
		<m_x, m_z>			and
		m_y_angle 			(in degrees)
		m_wall_length		(in inches )			

 Creates Vertex  from the Wall 3D data    */
void glRoom::update_line_info( )
{
	for (int w=0; w<m_fwalls.size(); w++)
	{
		// Extracts the wall info plus copies the Doors and Windows info :
		m_fwalls[w]->m_bare_wall.extract_2d_line_info( );
	}
}


