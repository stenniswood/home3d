//
//  quarter_629.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/12/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include "quarter_629.h"
#include "all_objects.h"


glUpstairs::glUpstairs()
{
    
}
glUpstairs::~glUpstairs()
{
    
}
void glUpstairs::create_vertical_walls   ( )
{
    glDoor* tdoor;
    static glFullWall* w;
    //struct stSize s;		// for windows

    // Side 1 :
    w = new glFullWall();
    w->clear( );
    w->set_length( 50*12.);
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 0.;
    w->m_x = 0.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
    // Side 2 :
    w = new glFullWall();
    w->clear( );
    w->set_length( 50*12.);
    tdoor = new glDoor();                   // Door #1
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 110.     , tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 275.;
    w->m_x = 0.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
    // Hall #1
    w = new glFullWall  ( );
    w->clear            ( );
    w->set_length( 36.+116.+120.-3.);
    tdoor = new glDoor();                   // Door #1 Boys bedroom
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door          ( 110., tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    tdoor = new glDoor();                   // Door #2  Mom and Dads
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 110.     , tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    w->setup            (  );
    w->m_y_angle = 0.0;
    w->m_z = 120.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back    ( w );
    
    // Kitchen / Living Room (similar to hall #2):
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 160.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
    // Hall #2
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    tdoor = new glDoor();                   // Bathroom Door
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 110.     , tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 160.;
    w->m_x = 144.+12*12;
    m_components.push_back( w );
    m_fwalls.push_back( w );
}

void glUpstairs::create_horizontal_walls ( )
{
    glDoor* tdoor;
    static glFullWall* w;
    //struct stSize s;		// for windows
    
    // Front :
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    tdoor = new glDoor();                   // Bathroom Door
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true );
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    w->add_door  ( 110.     , tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 0.;
    w->m_x = 0.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
    // Living Room/Boys Bedroom :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 0.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );

    // Boys Bedroom/Mom & Dads :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 0.;
    w->m_x = 144.+12*12;
    m_components.push_back( w );
    m_fwalls.push_back( w );

    // Back End :
    w = new glFullWall();
    w->clear( );
    w->set_length( 20*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 0.;
    w->m_x = 400;
    m_components.push_back( w );
    m_fwalls.push_back( w );


    // Bathroom/Lynn's :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 160.;
    w->m_x = 400-144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );

    // Dining Room/Bathroom :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 160.;
    w->m_x = 400-144.-6*12;
    m_components.push_back( w );
    m_fwalls.push_back( w );

    // Kitchen/Dining Room :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    tdoor = new glDoor();                   // Bathroom Door
    tdoor->set_params(30.);
    tdoor->set_hinge     (false);
    tdoor->set_swing_side(true);
    tdoor->setup         (     );
    tdoor->open          ( 0.4 );
    //w->add_door  ( 110.     , tdoor );
    w->m_bare_wall.add_door( 122.+40.5+64.5, DEFAULT_DOOR_WIDTH, 79.0, false );
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 160.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );

    // Entryway / Kitchen :
    w = new glFullWall();
    w->clear( );
    w->set_length( 10*12.);
    w->setup    (  );
    w->m_y_angle = -90.0;
    w->m_z = 160.;
    w->m_x = 4*12.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
}

void glUpstairs::add_light_switches      ( )
{
    glLightSwitch*      ls;
    glDualLightSwitch* dls;
    // 44 inches to bottom of cover

    // Entrance
    dls = new glDualLightSwitch();              dls->m_y = 44;
    dls->m_distance_along_wall = 8;             dls->m_wall_side = 1;
    m_fwalls[1]->add_fixture( dls );
    
    
    // Living Room1
    // Living room2
    // dining room
    // dining room2 side door
    // kitchen
    // boys bedroom
    // mom & dads bedroom
    // Lynn's bedroom
    // bathroom
    
}
void glUpstairs::create_components       ( )
{
    create_vertical_walls();
    create_horizontal_walls();
    add_light_switches();
}

/********************************************/
glBasement::glBasement ()
{
    
}
glBasement::~glBasement()
{
    
}
void glBasement::create_vertical_walls   ( )
{
    glDoor* tdoor;
    static glFullWall* w;
    
    // Side 1 :
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 160.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
    // Side 2 :
    w = new glFullWall();
    w->clear( );
    w->set_length( 36.+116.+120.-3.);
    w->setup    (  );
    w->m_y_angle = 0.0;
    w->m_z = 160.;
    w->m_x = 144.;
    m_components.push_back( w );
    m_fwalls.push_back( w );
    
}
void glBasement::create_horizontal_walls ( )
{
    
}
void glBasement::add_light_switches      ( )
{
    
}
void glBasement::create_components       ( )
{
    create_vertical_walls  ();
    create_horizontal_walls();
    add_light_switches     ();
}


/********************************************/
glOurHouse::glOurHouse ( )
{
    
}

glOurHouse::~glOurHouse( )
{
    
}

void glOurHouse::create_components( )
{
    //bm = new glBasement();
    //sw = new glStairway();

    up = new glUpstairs();
    up->setup();
    up->relocate(0,0,0);
    m_components.push_back(up);
    
    //bm->setup();
    //sw->setup();
    //sw->relocate( 200, 0, 0);
    

//    m_components.push_back(bm);
//    m_components.push_back(sw);
}
