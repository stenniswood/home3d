//
//  construction_barricade.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/8/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include "all_objects.h"
#include "construction_barricade.h"




glBarricade::glBarricade( )
: m_light1(36), m_light2(36)
{
    Initialize();
}
glBarricade::~glBarricade( )
{
    
}
void glBarricade::Initialize	( )
{
    m_object_type_name = "barricade";
    m_object_class  = 17;
    m_width         = 70;		// Length along 1 side.
    m_board_height  = 10;		// Distance across the table.
    m_board_space   = 10;		// how high off ground.
}

void glBarricade::create_components()
{
    long bcolor = 0xFF8F3030;
    
    //*** Three Boards :
    glBox* board = new glBox();
    board->width  = 70.;
    board->height = 10.;
    board->depth  = 1.5;
    board->m_y    = 10;
    board->grab_right();
    board->change_bottom_color(bcolor);
    board->change_top_color   (bcolor);
    board->create();
    m_components.push_back( board );

    glBox* board2 = new glBox();
    board2->width  = 70.;
    board2->height = 10.;
    board2->depth  = 1.5;
    board2->m_y    = 30;
    board2->grab_right();
    board2->change_bottom_color(bcolor);
    board2->change_top_color   (bcolor);
    board2->create();
    m_components.push_back( board2 );

    board2 = new glBox();
    board2->width  = 70.;
    board2->height = 10.;
    board2->depth  = 1.5;
    board2->m_y    = 50;
    board2->grab_right();
    board2->change_bottom_color(bcolor);
    board2->change_top_color   (bcolor);
    board2->create();
    m_components.push_back( board2 );
    
    //** Two Uprights :
    glBox* bar = new glBox();
    bar->width  =  1.;
    bar->height = 60.;
    bar->depth  =  1.;
    bar->m_x    =   10;
    bar->m_y    =   0;
    bar->m_z    =   0;
    bar->grab_bottom();
    bar->create();
    m_components.push_back( bar );

    glBox* bar2 = new glBox();
    //*bar2 = *bar;
    bar2->width  =  1.;
    bar2->height = 60.;
    bar2->depth  =  1.;
    bar2->m_x    =  60;
    bar2->m_y    =   0;
    bar2->m_z    =   0;
    bar2->grab_bottom();
    bar2->create     ();
    m_components.push_back( bar2 );

    //** Two Feet :
    bar2 = new glBox();
    bar2->depth  = 30;
    bar2->height = 1.;
    bar2->width  = 1.;
    bar2->m_x = 10;
    bar2->m_y =  0.;
    bar2->grab_bottom();
    bar2->create();
    m_components.push_back( bar2 );

    bar2 = new glBox();
    bar2->depth  = 30;
    bar2->height = 1.;
    bar2->width  = 1.;
    bar2->m_x = 60;
    bar2->m_y = 0.;
    bar2->grab_bottom();
    bar2->create();
    m_components.push_back( bar2 );
}

void glBarricade::draw()
{
    int size = (int)m_components.size();
    glExtrusion* ptr;
    for (int c=0; c<size; c++)
    {
        ptr = (glExtrusion*)m_components[c];
        ptr->draw();
    }
}


