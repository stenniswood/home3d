//
//  dwelling.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/7/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "dwelling.h"
#include "all_objects.h"


glDwelling::glDwelling()
{
    m_object_type_name = "dwelling";
}

glDwelling::~glDwelling()
{
    
}


/*glDoor* glDwelling::get_door( string mName   )
{
    glDoor* ptr= NULL;
    for (int w=0; w<m_levels.size(); w++)
    {
        ptr = m_levels[w].get_door( mName );
        if (ptr)
            return ptr;
    }
    return NULL;
} */

