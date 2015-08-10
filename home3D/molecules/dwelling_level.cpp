//
//  dwelling_level.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/7/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "dwelling_level.h"
//#include "all_objects.h"
#include "glComputableWall.h"
#include "physical_world.h"


glDwellingLevel::glDwellingLevel( )
{
    m_object_type_name = "dwelling level";
    m_color = 0xFFFFFFFF;
    //m_is_closed = false;
}

glDwellingLevel::~glDwellingLevel( )
{
    
}

/* Gives the max z for a wall origin, not necessarily the max z vertex! */
float glDwellingLevel::get_max_x()
{
    MathVector tmp(3);
    long size = m_fwalls.size();
    float max_x = 0.0;
    for (int w=0; w<size; w++)
    {
        if (m_fwalls[w]->m_x > max_x)
            max_x = m_fwalls[w]->m_x;
        tmp = m_fwalls[w]->m_bare_wall.get_far_end();
        if (tmp[0] > max_x)
            max_x = tmp[0];
    }
    if (m_floor)
    {
        m_floor->compute_min();
        m_floor->compute_max();
        float tmpx;
        tmpx = m_x + m_floor->m_max.position[0];
        if (tmpx > max_x)
            max_x = tmpx;
    }
    return max_x;
}


void glDwellingLevel::generate_exterior_walls()
{
  //  glComputeableWall* ewall = new glComputeableWall();
  //  *ewall = (m_fwalls[0]->m_bare_wall);
  //  m_ext_walls.push_back(ewall); 
}


/* Gives the max z for a wall origin, not necessarily the max z vertex! */
float glDwellingLevel::get_max_z()
{
    MathVector tmp(3);
    long size = m_fwalls.size();
    float max_z = 0.0;
    for (int w=0; w<size; w++)
    {
        if (m_fwalls[w]->m_z > max_z)
            max_z = m_fwalls[w]->m_z;

        tmp = m_fwalls[w]->m_bare_wall.get_far_end();
        if (tmp[2] > max_z)
            max_z = tmp[2];
    }
    if (m_floor)
    {
        m_floor->compute_min();
        m_floor->compute_max();
        float tmpz;
        tmpz = m_z + m_floor->m_max.position[2];
        if (tmpz > max_z)
           max_z = tmpz;
    }
    return max_z;
}

void glDwellingLevel::mirror_image_x          ( )
{
    long  size  = m_fwalls.size();
    float max_z = get_max_z();
    
    // for all walls, set the origin at -m_z
    for (int w=0; w<size; w++)
    {
        m_fwalls[w]->extract_2d_line_info();
        m_fwalls[w]->m_z       = -m_fwalls[w]->m_z + max_z;
        // subtract component length in Z direction.
        MathVector len(3);
        len = m_fwalls[w]->m_bare_wall.m_line.m_vector * m_fwalls[w]->m_bare_wall.m_wall_length;
        m_fwalls[w]->m_z -= len[2];
        
        // Now mirrow image the door/window/fixture locations:        
    }
}

void glDwellingLevel::mirror_image_z          ( )
{
    // for all walls, set the origin at -m_z
    long size = m_fwalls.size();
    float max_x = get_max_x();
    
    for (int w=0; w<size; w++)
    {
        m_fwalls[w]->extract_2d_line_info();
        m_fwalls[w]->m_x       = -m_fwalls[w]->m_x + max_x;
        //m_fwalls[w]->m_y_angle = m_fwalls[w]->m_y_angle + 180;

        MathVector len(3);
        len = m_fwalls[w]->m_bare_wall.m_line.m_vector * m_fwalls[w]->m_bare_wall.m_wall_length;
        m_fwalls[w]->m_x -= len[0];
    }
}

// Class is a glMolecule :
/*void glDwellingLevel::add_walls()
{
//    for (int i=0; i<m_fwalls.size(); i++)
     //   m_components.push_back( m_fwalls[i] );
}*/

void glDwellingLevel::create_room ( float width, float length )
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
void glDwellingLevel::update_line_info( )
{
    for (int w=0; w<m_fwalls.size(); w++)
    {
        // Extracts the wall info plus copies the Doors and Windows info :
        m_fwalls[w]->m_bare_wall.extract_2d_line_info( );
    }
}

void glDwellingLevel::create_map( glMap2D& mMap )
{
    for (int i=0; i<m_fwalls.size(); i++)
    {
        m_fwalls[i]->extract_2d_line_info();
        mMap.m_fwalls.push_back( m_fwalls[i] );
    }
}

void glDwellingLevel::create_floor()        // to cover all.
{
    struct room  dimen;
    dimen.sx = 0;
    dimen.sz = 0;
    dimen.ex = get_max_x();
    dimen.ez = get_max_z();
    m_floor->cover_all( dimen );
}

/* For 1 wall.  */
void glDwellingLevel::create_linesegs( float mLinearDistance, float mWallAngleRadians, float mHeight )
{
    // break at each door opening.
    
    //
    
}
/*
 mWallAngle is the horizontal angle away from the wall.
 */
void glDwellingLevel::find_all_possibilies( float mLinearDistance, float mWallAngleRadians, float mHeight )
{
    float perp_distance     = mLinearDistance * sin( mWallAngleRadians );
    float parallel_distance = mLinearDistance * cos( mWallAngleRadians );
    
    // now go around all the wall and create a line segment set so far away from the wall.
    // if another wall passes within that distance, then cut the line there.
    long wsize = m_fwalls.size();
    LineSegment lseg;
    
    for (int w=0; w<wsize; w++)
    {
        long num_doorways   = m_fwalls[w]->m_bare_wall.m_doorways.size();
        MathVector start_pt = m_fwalls[w]->m_bare_wall.m_line.m_origin;
        MathVector end_pt   = m_fwalls[w]->m_bare_wall.get_far_end();
        MathVector offset = m_fwalls[w]->m_bare_wall.m_line.m_vector.get_perp_xz();
        offset.unitize();
        offset *= perp_distance;
        start_pt += offset;
        end_pt   += offset;
        
        // now take parallel_distance off of 1 end.
        MathVector end_retraction = m_fwalls[w]->m_bare_wall.m_line.m_vector * parallel_distance;
        if (mWallAngleRadians>M_PI/2)
            end_pt -= end_retraction;
        else
            start_pt += end_retraction;
        
        LineSegment lseg( start_pt, end_pt );
        m_line_segs.push_back(lseg);
    }
}

void glDwellingLevel::accept_heading_measurement( float mAngleToNorth )
{
    
}


glDwellingLevel* glDwellingLevel::create_copy()
{
    glDwellingLevel* dl = new glDwellingLevel();
 /*   long fsize = m_fwalls.size();
    glFullWall* wptr = NULL;
    for (int w=0; w<fsize; w++)
    {
        wptr = new glFullWall();
        *wptr = *m_fwalls[w];       // copy contents
        // this should create duplicate fixtures, doors, windows, etc.
        dl->m_fwalls.push_back(wptr);
    } */
    
    // DONT COPY THE FURNITURE:
    /*fsize = m_furniture.size();
     for (int f=0; f<fsize; f++)
     {
     dl->m_furniture.push_back(wptr);
     }
     
     // COPY ALL THINGS:
     fsize = m_things.size();
     for (int f=0; f<fsize; f++)
     {
     dl->m_things.push_back(wptr);
     }*/
    return dl;
}

glDwellingLevel* glDwellingLevel::create_mirror_image( )
{
    glDwellingLevel* dl = new glDwellingLevel();
 /*   long fsize = m_fwalls.size();
    glFullWall* wptr = NULL;
    for (int w=0; w<fsize; w++)
    {
        wptr = new glFullWall();
        *wptr = *m_fwalls[w];       // copy contents
                // this should create duplicate fixtures, doors, windows, etc.
        dl->m_fwalls.push_back(wptr);
    } */
    
    // All of the angles will stay the same.  just the locations swapped (ie. m_x,m_y,m_z)
    //   swap with which though.
    //
    
    // DONT COPY THE FURNITURE:
    /*fsize = m_furniture.size();
    for (int f=0; f<fsize; f++)
    {
        dl->m_furniture.push_back(wptr);
    }

    // COPY ALL THINGS:
    fsize = m_things.size();
    for (int f=0; f<fsize; f++)
    {
        dl->m_things.push_back(wptr);
    }*/
    
    return dl;
}

bool glDwellingLevel::evaluate_collision( glSphere* mOther )
{
    long size = m_fwalls.size();
    bool result = false;
    for (int w=0; w<size; w++)
    {
        result = m_fwalls[w]->m_bare_wall.evaluate_collision(mOther);
        if (result) {
            MathVector normal(3);
            normal = m_fwalls[w]->m_bare_wall.m_line.m_vector.get_perp_xz();
            theWorld.snells_law( mOther, normal );
            mOther->m_position = mOther->m_prev_position;            
            return true;
        }
    }
    return false;
}




