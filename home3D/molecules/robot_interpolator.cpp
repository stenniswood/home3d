//
//  robot_interpolator.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/14/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "robot_interpolator.h"

const int NUMBER_SAMPLES = 10;


glRobotInterpolator::glRobotInterpolator()
{
    
}
glRobotInterpolator::~glRobotInterpolator()
{
    
}

/* simple average */
float glRobotInterpolator::interpolate_one( float mAngle1, float mAngle2,
                                           float mNumberOfSamples,
                                           float mIndex
                                           )
{
    float slice = (mAngle2 - mAngle1)/(mNumberOfSamples-1);
    float result = slice * mIndex + mAngle1;
    return result;
}

struct stBodyPosition*  glRobotInterpolator::interpolate(
                                struct stBodyPosition* bp1,
                                struct stBodyPosition* bp2,
                                int number_samples,
                                int index_within_samples )
{
    static struct stBodyPosition bp;
    float* r  = (float*)&bp;
    float* s1 = (float*)bp1;
    float* s2 = (float*)bp2;
    int number_of_floats = sizeof( struct stBodyPosition ) / sizeof(float);
    
    for (int i=0; i<number_of_floats; i++)
        r[i] = interpolate_one( s1[i], s2[i], number_samples, index_within_samples );
    
    return &bp;
}


/* results stored int m_interpolated_sequence vector */
void  glRobotInterpolator::interpolate_all
                            (struct stBodyPosition* bp1,
                             struct stBodyPosition* bp2,
                             int number_samples  )
{
    struct stBodyPosition* tmp;
    for (int i=0; i<number_samples; i++)
    {
        tmp = interpolate( bp1, bp2, number_samples, i);
        m_interpolated_sequence.push_back( *tmp );
    }
}

bool glRobotInterpolator::play_next()
{
    if (m_interpolated_sequence.size())
        set_robot_position( &m_interpolated_sequence[m_interpolated_index] );
    
    m_interpolated_index++;
    if (m_interpolated_index>m_interpolated_sequence.size())
    {
        m_interpolated_index = 0;
        return true;
    }
    return false;
}

void glRobotInterpolator::morph_demos()
{
    struct stBodyPosition bp1;
    struct stBodyPosition bp2;
    m_interpolated_sequence.clear();
    
    extract_robot_position(&bp1);  // save current positions
    
    // Activate new positions
    glStaticMovesRobot::demo();
    extract_robot_position(&bp2);  // save current positions
    
    interpolate_all( &bp1, &bp2, NUMBER_SAMPLES);
    m_interpolated_index = 0;
}

