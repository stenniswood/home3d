//
//  full_wall.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/2/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include "full_wall.h"
#include "all_objects.h"


glFullWall::glFullWall (  )
{
    m_object_type_name = "full wall";
}
glFullWall::glFullWall( glFullWall& mSrc )
{
    m_bare_wall = mSrc.m_bare_wall;
    long size = mSrc.m_doors.size();
    glDoor* dptr = NULL;
    for (int d=0; d<size; d++)
    {
        dptr = new glDoor();
        *dptr = *(mSrc.m_doors[d]);
        m_doors.push_back( dptr );
    }
    
    // How do you copy a virtual object of unknown type?
    
    size = mSrc.m_fixtures.size();
    glFixture* fptr = NULL;
    for (int f=0; f<size; f++)
    {
        fptr = new glFixture();
        *fptr = *(mSrc.m_fixtures[f]);
        m_fixtures.push_back( fptr );
    }
}

glFullWall::~glFullWall(  )
{
    
}

void glFullWall::Initialize	( )
{
    
}
extern float radians( float degrees );

/* Extracts and puts into the abWall object */
void glFullWall::extract_2d_line_info()
{
    //m_bare_wall.extract_2d_line_info();     can't do this b/c m_y_angle is not in bare wall!
    
    // Put into bare wall because the computations are done from there.
    m_bare_wall.m_line.m_origin.dimension(3);
    m_bare_wall.m_line.m_origin[0] = m_x;
    m_bare_wall.m_line.m_origin[1] = 2.0;
    m_bare_wall.m_line.m_origin[2] = m_z;
    
    // Convert the wall angle to a unit vector.  This might better be done when creating the dwelling in the first place.
    float rad = -radians(m_y_angle);
    m_bare_wall.m_line.m_vector.dimension(3);
    m_bare_wall.m_line.m_vector[0] = cos(rad);
    m_bare_wall.m_line.m_vector[1] = 0.0;        // y is height!
    m_bare_wall.m_line.m_vector[2] = sin(rad);
}

void glFullWall::create_components()
{
    // All DOORS HAVE TO BE ALREADY ADDED.
    m_bare_wall.setup();    // do here because all doors have already been added!
    m_bare_wall.relocate(m_x, m_y, m_z);
    m_components.push_back( &m_bare_wall );
    
    for (int i=0; i<m_doors.size(); i++)
        m_components.push_back( m_doors[i] );
    
    for (int i=0; i<m_fixtures.size(); i++)
        m_components.push_back( m_fixtures[i] );
}

void glFullWall::setup( )
{
    create_components();
}

void glFullWall::clear( )
{
    m_bare_wall.clear();
    for(int i=0; i<m_doors.size(); i++)
        delete m_doors[i];
    m_doors.clear();
    
    for(int i=0; i<m_fixtures.size(); i++)
        delete m_fixtures[i];
    m_fixtures.clear();
}


void glFullWall::add_door( float mPositionLengthwise, glDoor* mDoor )
{
    m_bare_wall.extract_2d_line_info();
    m_bare_wall.add_door( mPositionLengthwise, mDoor->m_width, mDoor->m_height );

    int index  = (int)(m_bare_wall.m_doorways.size()&0xFFFF);
    MathVector mv;
    if (mDoor->m_hinge_side_near)
        mv = m_bare_wall.get_door_coord( index-1 );
    else
        mv = m_bare_wall.get_door_far_coord( index-1, -m_bare_wall.m_wall_thickness/2. );
    mDoor->relocate( mv[0], 0.0, mv[2] );
    m_doors.push_back( mDoor );
}

void glFullWall::add_window( float mPositionLengthwise, float mSillHeight, glWindow* mWindow )
{
    m_bare_wall.add_window( mPositionLengthwise, mWindow->m_size, mSillHeight );
}

void glFullWall::add_fixture ( glFixture*   mFixture )
{
    extract_2d_line_info();
    MathVector v(3);
    
    // Make Adjustment for which side of the wall it's on.
    if (mFixture->m_wall_side==0)           // different for horiz & vert walls!
        mFixture->m_y_angle = 180;
    
    float perp = (mFixture->m_wall_side) * m_bare_wall.m_wall_thickness;
    mFixture->relocate( mFixture->m_distance_along_wall, mFixture->m_y,  perp );
    mFixture->setup();
    
    m_fixtures.push_back  ( mFixture );
    m_components.push_back( mFixture );
}



