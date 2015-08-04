//
//  floor.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/30/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "floor.h"
//#include "all_objects.h"
#include "dwelling_level.h"
#include "txt_container.hpp"

glFloor::glFloor()
{
    
    txtContainer* tmp = new txtContainer();
    m_cover.push_back(tmp);
}

glFloor::~glFloor()
{
    
}

void glFloor::cover_all        (struct room mRectangle)
{
    
}
void glFloor::divide_into_rooms()
{
    
}
