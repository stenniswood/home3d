//
//  painting.h
//  home3D
//
//  Created by Stephen Tenniswood on 7/24/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#ifndef __home3D__painting__
#define __home3D__painting__

#include <stdio.h>
#include "paper.hpp"
#include "gl_container.hpp"
#include "glMolecule.h"
#include "frame.h"

/* A painting has a frame around it. 
    Ordinary rectangular frame.
 */
class glPainting : public glMolecule
{
public:
    glPainting();
    ~glPainting();
    
    virtual void    create_components();
    void            select_work_of_art( int mIndex );
    
    glPaper* m_painting;
    glFrame* m_frame;
    
    /*glBox*   m_left_side;
    glBox*   m_right_side;
    glBox*   m_top_side;
    glBox*   m_bottom_side; */
};


#endif /* defined(__home3D__painting__) */

