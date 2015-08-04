//
//  basket_ball_court.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 7/30/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
#include "basket_ball_court.h"
#include "atom_objects.h"


glBackboard::glBackboard()
{
    
}

glBackboard::~glBackboard()
{
    
}

/* Origin:
    bottom is 1 foot below rim   (set at 9*12)
 
 */
void  glBackboard::create_components( )
{
    const float BACKBOARD_WIDTH = 72.;
    const float BACKBOARD_FORWARD_FROM_POLE_DISTANCE = 22.;
    
    // BACK BOARD #1 & 2:
    glBox* backboard = new glBox();
    backboard->set_color( 0xFFFFFFFF);
    backboard->width  = BACKBOARD_WIDTH;
    backboard->height = 42;
    backboard->depth = 1.0;
    backboard->setup();
    backboard->grab_bottom();
    backboard->relocate( 0, 0, 0.5+BACKBOARD_FORWARD_FROM_POLE_DISTANCE );
    m_components.push_back(backboard);

    const float LENGTH_SHORTNESS = 4.;
    // TOP SUPPORT PIPE:
    glCylinder* pipe = new glCylinder(24);
    pipe->set_color(0xFF2f2f2f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_WIDTH - LENGTH_SHORTNESS;
    pipe->m_extrusion_axis   = 0;
    pipe->setup();
    //pipe->grab_left();
    pipe->relocate( -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS/2, 0+pipe->m_radius, -pipe->m_radius+BACKBOARD_FORWARD_FROM_POLE_DISTANCE );
    m_components.push_back(pipe);
    
    // BOTTOM SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_WIDTH - LENGTH_SHORTNESS;
    pipe->m_extrusion_axis   = 0;
    pipe->setup();
    //pipe->grab_left();
    pipe->relocate(  -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS/2, backboard->height-pipe->m_radius, -pipe->m_radius+BACKBOARD_FORWARD_FROM_POLE_DISTANCE );
    m_components.push_back(pipe);
  
// *****************************
// Now pipes extending forward:
    // TOP LEFT FORWARD SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_FORWARD_FROM_POLE_DISTANCE;
    pipe->m_extrusion_axis   = 2;
    pipe->setup();
    pipe->relocate(  -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS, backboard->height-3*pipe->m_radius, 0 );
    m_components.push_back(pipe);

    // TOP RIGHT FORWARD SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_FORWARD_FROM_POLE_DISTANCE;
    pipe->m_extrusion_axis   = 2;
    pipe->setup();
    pipe->relocate(  +BACKBOARD_WIDTH/2-LENGTH_SHORTNESS, backboard->height-3*pipe->m_radius, 0 );
    m_components.push_back(pipe);

    // BOTTOM LEFT FORWARD SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_FORWARD_FROM_POLE_DISTANCE;
    pipe->m_extrusion_axis   = 2;
    pipe->setup();
    pipe->relocate(  -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS, +3*pipe->m_radius, 0 );
    m_components.push_back(pipe);
    
    // BOTTOM RIGHT FORWARD SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_FORWARD_FROM_POLE_DISTANCE;
    pipe->m_extrusion_axis   = 2;
    pipe->setup();
    pipe->relocate(  +BACKBOARD_WIDTH/2-LENGTH_SHORTNESS, +3*pipe->m_radius, 0 );
    m_components.push_back(pipe);
    
    
// NOW 2 more pipes by the pole:
    // TOP SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF2f2f2f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_WIDTH - LENGTH_SHORTNESS;
    pipe->m_extrusion_axis   = 0;
    pipe->setup();
    pipe->relocate( -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS/2, 0+pipe->m_radius, 0 );
    m_components.push_back(pipe);
    
    // BOTTOM SUPPORT PIPE:
    pipe = new glCylinder(24);
    pipe->set_color(0xFF4f2f4f);
    pipe->m_radius = 2.;
    pipe->m_extrusion_length = BACKBOARD_WIDTH - LENGTH_SHORTNESS;
    pipe->m_extrusion_axis   = 0;
    pipe->setup();
    pipe->relocate(  -BACKBOARD_WIDTH/2+LENGTH_SHORTNESS/2, backboard->height-pipe->m_radius, 0 );
    m_components.push_back(pipe);

}



/****************************************************************/

/****************************************************************/

glBasketballCourt::glBasketballCourt ()
{
    
}
glBasketballCourt::~glBasketballCourt()
{
    
}

void  glBasketballCourt::select_floor( int mIndex )
{
        switch( mIndex )
    {
        case 0 :  m_floor->m_cover[0]->load_image("textures/basket_ball_court.jpg");
            break;
        case 1 :  m_floor->m_cover[0]->load_image("textures/pistons.bmp");
            break;
        default:
            break;
    }
}

void  glBasketballCourt::create_floor( )
{
    m_floor = new glFloor();
    m_floor->m_cover[0]->width  = 50*12;
    m_floor->m_cover[0]->height = 1.0  ;
    m_floor->m_cover[0]->depth  = 94*12;
    select_floor( 1 );
    m_floor->m_cover[0]->setup();
    m_components.push_back( (m_floor->m_cover[0]));

    // wall around court (with advertisements)
    glBox* tmpw = new glBox();
    tmpw->set_color( 0xFFFF0000 );
    tmpw->width  = 1.0;
    tmpw->height = 4*12;
    tmpw->depth  = 94*12;
    tmpw->setup();
    tmpw->grab_left();
    tmpw->grab_bottom();
    tmpw->relocate( -25*12, 0, 0*12 );
    m_components.push_back( tmpw );
    
    tmpw = new glBox();
    tmpw->set_color( 0xFFFF0000 );
    tmpw->width  = 1.0;
    tmpw->height = 4*12;
    tmpw->depth  = 94*12;
    tmpw->setup();
    tmpw->grab_left();
    tmpw->grab_bottom();
    tmpw->relocate( 25*12, 0, 0*12 );
    m_components.push_back( tmpw );
    
    tmpw = new glBox();
    tmpw->set_color( 0xFFFF0000 );
    tmpw->width  = 50*12;
    tmpw->height = 4*12;
    tmpw->depth  = 1.0;
    tmpw->setup();
    tmpw->grab_right();
    tmpw->grab_bottom();
    tmpw->relocate( -25*12, 0, -94*12/2 );
    m_components.push_back( tmpw );
    
    tmpw = new glBox();
    tmpw->set_color( 0xFFFF0000 );
    tmpw->width  = 50*12;
    tmpw->height = 4*12;
    tmpw->depth  = 1.0;
    tmpw->setup();
    tmpw->grab_right();
    tmpw->grab_bottom();
    tmpw->relocate( -25*12, 0, 94*12/2 );
    m_components.push_back( tmpw );
}


void    glBasketballCourt::create_baskets ( )
{
    // POLES:
    const float margin = 0.925;
    glCylinder* tmp = new glCylinder(24);
    tmp->m_radius = 1.5;
    tmp->m_extrusion_length = 10*12;        // ?
    tmp->setup();
    m_components.push_back(tmp);
    tmp->relocate( 0, 0, -margin*m_floor->m_cover[0]->depth/2);

    tmp = new glCylinder(24);
    tmp->m_radius = 1.5;
    tmp->m_extrusion_length = 10*12;        // ?
    tmp->setup();
    m_components.push_back(tmp);
    tmp->relocate( 0, 0, margin * m_floor->m_cover[0]->depth/2);
    
    // BACK BOARD #1 & 2:
    glBackboard* backboard = new glBackboard();
    backboard->setup();
    backboard->relocate( 0, 9*12, -margin*m_floor->m_cover[0]->depth/2 );
    m_components.push_back(backboard);

    backboard = new glBackboard();
    backboard->setup();
    backboard->m_y_angle =180.;
    backboard->relocate( 0, 9*12, +margin*m_floor->m_cover[0]->depth/2 );
    m_components.push_back(backboard);
        
    // Rim :
    glRing* rim = new glRing();
    rim->set_color(0xFFFF0000);
    rim->set_finger_params(9, 40);
    rim->set_circle_params(0.25, 10);
    rim->setup();
    m_components.push_back(rim);
    rim->relocate( 0, 10*12, -12*47+63+9 );     // from center court!
    
    rim = new glRing();
    rim->set_color(0xFFFF0000);
    rim->set_finger_params(9, 40);
    rim->set_circle_params(0.25, 10);
    rim->setup();
    m_components.push_back(rim);
    rim->relocate( 0, 10*12,  +12*47-63-9);
    
}

void    glBasketballCourt::add_light_switches      ( )
{
    
}

void    glBasketballCourt::create_components       ( )
{
    create_floor();
    create_baskets();
}

