//
//  ConFloat.hpp
//  home3D
//
//  Created by Stephen Tenniswood on 4/8/16.
//  Copyright © 2016 Stephen Tenniswood. All rights reserved.
//

#ifndef ConFloat_hpp
#define ConFloat_hpp

#include <stdio.h>


class ConFloat
{
public:
    ConFloat();
    ConFloat(float mInitValue);
    ~ConFloat();
    float operator+=(float mRHV);
    float operator-=(float mRHV);
    float operator++();

private:
    float m_Lowest_limit;
    float m_Highest_limit;
    float m_Wrap_around;
};


#endif /* ConFloat_hpp */
