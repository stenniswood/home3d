//
//  verbal_atom.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/22/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "verbal_object.h"
#include "all_objects.h"



VerbalObject::VerbalObject()
{
    
}

VerbalObject::~VerbalObject()
{
    
}

bool VerbalObject::is_a_molecule()
{
    return false;
}

bool  VerbalObject::name_matches( string mRequestedName )
{
    return m_name.compare( mRequestedName )==0;
}


