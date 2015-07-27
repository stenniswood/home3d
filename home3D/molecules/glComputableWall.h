//
//  glComputableWall.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/2/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__glComputableWall__
#define __home3D__glComputableWall__

#include <stdio.h>
#include "wall.hpp"



class glComputeableWall : public glBareWall
{
public:
    glComputeableWall ( );
    ~glComputeableWall( );
    
    int     find_closest_door ( MathVector mIntersection );
    float   get_distance_along( MathVector mIntersection );
    float   is_on_which_side  ( MathVector mPoint        );    // returns -1.0 or +1.0
    
    void    find_paths_around ( list<float>& mDistanceAlong );
    void    find_paths_around ( list<MathVector>& mPoints, float mOffsetAwayFromWall );
    int     find_alternate_via( MathVector mIntersection, MathVector& mNewPoint );
    bool	intersects        ( LineSegment mPath, float& t, float& s );
    
    float	get_door_nearside (  int mDoorIndex  );
    float	get_door_center   (  int mDoorIndex  );
    float	get_door_far	  (  int mDoorIndex  );
    
    MathVector  get_point_away_from  (  float mDistanceAlong, float mPerpendicularDistance, bool mPositiveSide=true );
    MathVector	get_door_coord		 (  int mDoorIndex, float mPerpDistance=0.0  );
    MathVector	get_door_center_coord(  int mDoorIndex, float mPerpDistance=0.0  );
    MathVector	get_door_far_coord	 (  int mDoorIndex, float mPerpDistance=0.0  );
    MathVector	get_far_end          (  );
    
};


#endif /* defined(__home3D__glComputableWall__) */
