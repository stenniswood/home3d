//
//  verbal_atom.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/22/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__verbal_atom__
#define __home3D__verbal_atom__

#include <stdio.h>
#include <string>
#include "gl_object.hpp"

using namespace std;

/* Note:  Verbal Object forms a base class for glAtom.
 Whereas Verbal Molecule is a derived class of glMolecule. 
 slight difference.  */

class VerbalObject : public glObject
{
public:
    VerbalObject();
    ~VerbalObject();
    bool            name_matches( string mRequestedName );
    
    virtual bool    is_a_molecule();
    string          m_name;
    string          m_object_type_name;
};

    
#endif /* defined(__home3D__verbal_atom__) */
