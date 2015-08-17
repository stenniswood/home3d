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
    m_level_height = 0*12;        // start with a basement.
}

glDwelling::~glDwelling()
{
    
}

void glDwelling::add_level( glDwellingLevel* mLevel )
{
    mLevel->m_y = m_level_height;
    m_components.push_back( mLevel );
    m_levels.push_back(mLevel);

    
    m_level_height += 10*12;
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

