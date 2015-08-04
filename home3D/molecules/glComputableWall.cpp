//
//  glComputableWall.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/2/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "glComputableWall.h"


const float WALL_END_OFFSET = 12.;      // for routing the robot around a corner


glComputeableWall::glComputeableWall( )
{
    
}

glComputeableWall::~glComputeableWall( )
{
    
}

/*
 Assumption is that mIntersection is a 3D point on the base of the wall
 */
int glComputeableWall::find_closest_door( MathVector mIntersection )
{
    int   min_door_index = -1;
    float min_distance   = m_wall_length;
    int   num_doors      = (int)m_doorways.size();
    
    // Find closest Door to the Intersection.
    for (int d=0; d<num_doors; d++)
    {
        MathVector dc = get_door_center_coord( d );
        float distance = mIntersection.get_distance( dc );
        if (distance < min_distance)
        {
            min_door_index = d;
            min_distance = distance;
        }
    }
    return min_door_index;
}

/* Assumption is that mIntersection is a 3D point on the base of the wall */
float glComputeableWall::get_distance_along( MathVector mIntersection )
{
    // m_origin + m_vector * mIntersection
    MathVector delta = (mIntersection - m_line.m_origin);
    float  distance = delta.magnitude();
    return distance;
}

// returns -1.0 or +1.0
float glComputeableWall::is_on_which_side( MathVector mPoint )
{
    // Find which side of the wall we are on (with respect to the unit vector):
    MathVector dv = mPoint - m_line.m_origin;
    float side_projection = dv.dot( m_line.m_vector.get_perp_xz() );
    if (side_projection>=0.0)
    {
        side_projection = 1.0;      printf("is on + side\n");
    } else {
        side_projection = -1.0;     printf("is on - side\n");
    }
    return side_projection;
}

void glComputeableWall::find_paths_around ( list<float>& mDistanceAlong )
{
    // LIST ALL DOORS :
    long size = m_doorways.size();
    for (int i=0; i<size; i++)
        mDistanceAlong.push_back( get_door_center(i) );
    
    // LIST END POINTS :
    float tmp = -WALL_END_OFFSET ;
    mDistanceAlong.push_back(tmp);
    
    tmp = (m_wall_length+WALL_END_OFFSET);
    mDistanceAlong.push_back(tmp);
}

void glComputeableWall::find_paths_around( list<MathVector>& mPoints, float mOffsetAwayFromWall )
{
    // LIST ALL DOORS :
    long size = m_doorways.size();
    for (int i=0; i<size; i++)
        mPoints.push_back( get_door_center_coord( i, mOffsetAwayFromWall ) );
    
    // LIST END POINTS :
    MathVector tmp = m_line.m_origin + m_line.m_vector * -WALL_END_OFFSET + m_line.m_vector.get_perp_xz()*mOffsetAwayFromWall;
    mPoints.push_back(tmp);
    
    tmp = m_line.m_origin + m_line.m_vector * (m_wall_length + WALL_END_OFFSET);
    tmp += m_line.m_vector.get_perp_xz()*mOffsetAwayFromWall;
    mPoints.push_back(tmp);
}

int glComputeableWall::find_alternate_via( MathVector mIntersection, MathVector& mNewPoint )
{
    //
    float distance1 = mIntersection.get_distance( m_line.m_origin );
    MathVector far_end = m_line.m_origin + (m_line.m_vector * m_wall_length);
    float distance2 = mIntersection.get_distance( far_end );
    
    //
    int index = find_closest_door(mIntersection);
    MathVector dc = get_door_center_coord( index );
    float distance3 = mIntersection.get_distance( dc );
    float min_dist  = distance1;
    int min = 1;
    
    //
    if (distance2 < min_dist)
    {    min = 2;   min_dist = distance2; }
    if (distance3 < min_dist)
    {    min = 3;   min_dist = distance2; }
    switch (min)
    {
        case 1: mNewPoint = m_line.m_origin;
            break;
        case 2: mNewPoint = far_end;
            break;
        case 3: mNewPoint = dc;
            break;
    }
    return min;
}

bool  glComputeableWall::intersects(  LineSegment mPath, float& t, float& s )
{
    int result = m_line.line_intersection(mPath, t, s);
    if (result)
    {
        // Yes they intersect.
        //      adjust endpoint
        return true;
    }
    else
        return false;
}

// Distance along wall
float glComputeableWall::get_door_nearside(  int mDoorIndex  )
{
    return m_doorways[mDoorIndex].position_lengthwise;
}

float glComputeableWall::get_door_center(  int mDoorIndex  )
{
    float distance_to_door_center = m_doorways[mDoorIndex].position_lengthwise + (m_doorways[mDoorIndex].width/2.0);
    return distance_to_door_center;
}

float glComputeableWall::get_door_far	 (  int mDoorIndex  )
{
    float distance_to_far = m_doorways[mDoorIndex].position_lengthwise + m_doorways[mDoorIndex].width;
    return distance_to_far;
}

MathVector glComputeableWall::get_point_away_from( float mDistanceAlong, float mPerpendicularDistance, bool mPositiveSide )
{
    MathVector pt("point away from wall", 3);
    pt = m_line.m_origin + m_line.m_vector * mDistanceAlong;
    // Define Positive Side:
    //          side of increasing perpendicular vector.
    
    //    if (mPositiveSide)
        
    pt += m_line.m_vector.get_perp_xz() * mPerpendicularDistance;
    return pt;
}

MathVector glComputeableWall::get_door_coord( int mDoorIndex, float mPerpDistance )
{
    float  distance = get_door_nearside(mDoorIndex);
    return get_point_away_from( distance, mPerpDistance );
}

MathVector glComputeableWall::get_door_center_coord( int mDoorIndex, float mPerpDistance )
{
    float distance = get_door_center(mDoorIndex);
    return get_point_away_from( distance, mPerpDistance );
}

MathVector glComputeableWall::get_door_far_coord(  int mDoorIndex, float mPerpDistance  )
{
    float distance = get_door_far(mDoorIndex);
    return get_point_away_from( distance, mPerpDistance );
}

MathVector	glComputeableWall::get_origin (  )
{
    return m_line.m_origin;
}

MathVector	glComputeableWall::get_far_end (  )
{
    MathVector end = m_line.m_origin + (m_line.m_vector * m_wall_length);
    return end;
}

//bool glComputeableWall::evaluate_collision( glMovingObject* mOther )
bool glComputeableWall::evaluate_collision( glSphere* mOther )
{
    // for now we assume it's a sphere:

    // IS IT ABOVE/BELOW the WALL?
    //bool candidate = false;
    float extent = mOther->m_position[1] + mOther->m_radius;
    if (extent < m_y)                return false;
    extent = mOther->m_position[1] - mOther->m_radius;
    if (extent > m_y+m_wall_height)  return false;

    // IS IT PAST THE START or END of the WALL?
    MathVector delta = mOther->m_position - m_line.m_origin;
    float distance_along_wall = m_line.m_vector.dot(delta);
    if (distance_along_wall < -mOther->m_radius) return false;
    if (distance_along_wall > (m_wall_length+mOther->m_radius)) return false;

    
    // We are now within the extents of the wall.  Check the actual distance away:
    MathVector perp = m_line.m_vector.get_perp_xz();
    float distance_away_from_wall = fabs(perp.dot( delta ));
    
    if (distance_away_from_wall > mOther->m_radius)
        return false;
    
    // Only chance of no collision now is in a doorway:
    long size = m_doorways.size();
    for(int d=0; d<size; d++)
    {
        // If it's in the doorway, let it pass.
        if ((distance_along_wall > m_doorways[d].position_lengthwise) &&
            (distance_along_wall < (m_doorways[d].position_lengthwise+m_doorways[d].width)) )
            if (mOther->m_y < (m_doorways[d].height-mOther->m_radius))
                return false;
    }
    
    return true;
}

