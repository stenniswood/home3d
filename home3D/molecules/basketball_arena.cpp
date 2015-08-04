//
//  basketball_arena.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 8/3/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "basketball_arena.h"
#include "basket_ball_court.h"
#include "stairway.hpp"
#include "sports_bench.h"
#include "walking_robot.h"



const int NUMBER_STEPS           = 20;
const float AUDIENCE_TIER_LENGTH = 50*24;

glArena::glArena ()
{
    
}
glArena::~glArena()
{
    
}
void      glArena::create_vertical_walls   ( )
{
    
}


void glArena::add_light_switches      ( )
{
    
}

void glArena::create_team1            ( )
{
    glStaticMovesRobot* p;
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team1.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team1.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team1.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team1.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team1.push_back(p);
    m_components.push_back(p);
}

void glArena::create_team2            ( )
{
    glStaticMovesRobot* p;
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team2.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team2.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team2.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team2.push_back(p);
    m_components.push_back(p);
    
    p = new glStaticMovesRobot();
    p->setup();
    p->hands_on_hip();
    m_team2.push_back(p);
    m_components.push_back(p);
}

void glArena::position_lineup_tunnel   ( )
{
    m_team1[0]->relocate(  CENTER_COURT_X+0,   50, -CENTER_COURT_Y-5*12 );
    m_team1[1]->relocate(  CENTER_COURT_X+100, 50, -CENTER_COURT_Y-5*12 );
    m_team1[2]->relocate(  CENTER_COURT_X+200, 50, -CENTER_COURT_Y-5*12 );
    m_team1[3]->relocate(  CENTER_COURT_X-100, 50, -CENTER_COURT_Y-5*12 );
    m_team1[4]->relocate(  CENTER_COURT_X-200, 50, -CENTER_COURT_Y-5*12 );
    
    m_team2[0]->relocate(  CENTER_COURT_X-0,   50, -CENTER_COURT_Y+5*12 );
    m_team2[1]->relocate(  CENTER_COURT_X+100, 50, -CENTER_COURT_Y+5*12 );
    m_team2[2]->relocate(  CENTER_COURT_X+200, 50, -CENTER_COURT_Y+5*12 );
    m_team2[3]->relocate(  CENTER_COURT_X-100, 50, -CENTER_COURT_Y+5*12 );
    m_team2[4]->relocate(  CENTER_COURT_X-200, 50, -CENTER_COURT_Y+5*12 );
    for (int i=0; i<5; i++)
        m_team2[i]->m_y_angle = 180;
}

void glArena::position_team_initial   ( )
{

    m_team1[0]->relocate(  CENTER_COURT_X-0, 50,   -CENTER_COURT_Y-NBA_COURT_LENGTH/12 );
    m_team1[1]->relocate(  CENTER_COURT_X+150, 50, -CENTER_COURT_Y-NBA_COURT_LENGTH/6 );
    m_team1[2]->relocate(  CENTER_COURT_X-100, 50, -CENTER_COURT_Y-NBA_COURT_LENGTH/6 );
    m_team1[3]->relocate(  CENTER_COURT_X+NBA_COURT_WIDTH/5, 50, -CENTER_COURT_Y-2*NBA_COURT_LENGTH/6 );
    m_team1[4]->relocate(  CENTER_COURT_X-NBA_COURT_WIDTH/5, 50, -CENTER_COURT_Y-2*NBA_COURT_LENGTH/8 );
    
    m_team2[0]->relocate(  CENTER_COURT_X-0,  50,  -CENTER_COURT_Y+NBA_COURT_LENGTH/12 );
    m_team2[1]->relocate(  CENTER_COURT_X+150, 50, -CENTER_COURT_Y+NBA_COURT_LENGTH/6 );
    m_team2[2]->relocate(  CENTER_COURT_X-100, 50, -CENTER_COURT_Y+NBA_COURT_LENGTH/6 );
    m_team2[3]->relocate(  CENTER_COURT_X+NBA_COURT_WIDTH/5, 50, -CENTER_COURT_Y+2*NBA_COURT_LENGTH/6 );
    m_team2[4]->relocate(  CENTER_COURT_X-NBA_COURT_WIDTH/5, 50, -CENTER_COURT_Y+2*NBA_COURT_LENGTH/8 );
    
    for (int i=0; i<5; i++)
        m_team2[i]->m_y_angle = 180;
}

void glArena::create_stands           ( )
{
    glStairway* tmp = new glStairway();
    tmp->m_run = 24.;
    tmp->m_extrusion_length = 94*12;
    tmp->set_color( 0xFFFF00FF );
    tmp->m_number_of_steps = 2*NUMBER_STEPS;
    tmp->setup();
    tmp->relocate( 25*12+24, 0, -tmp->m_extrusion_length );
    m_components.push_back( tmp );
    
    // Audience benches:
    glBench* tmpB = NULL;
    for (int i=0; i<NUMBER_STEPS; i++)
    {
        tmpB = new glBench();
        tmpB->m_length = 50*12;
        tmpB->m_height = 2*12;
        tmpB->setup();
        tmpB->m_y_angle = 90;
        tmpB->relocate( tmp->get_front_edge(i)+tmp->m_run*1.5+25*12, tmp->get_height(i), -94*12/4 );
        m_components.push_back( tmpB );
    }

    tmp = new glStairway();
    tmp->m_run = 24.;
    tmp->m_extrusion_length = 94*12;
    tmp->set_color( 0xFFFF00FF );
    tmp->m_number_of_steps = 2*NUMBER_STEPS;
    tmp->setup();
    tmp->m_y_angle = 180;
    tmp->relocate( -25*12+24, 0, -0 );
    m_components.push_back( tmp );
    
    // Audience benches:
    tmpB = NULL;
    for (int i=0; i<NUMBER_STEPS; i++)
    {
        tmpB = new glBench();
        tmpB->m_length = 50*12;
        tmpB->m_height = 2*12;
        tmpB->setup();
        tmpB->m_y_angle = 90;
        tmpB->relocate( -(tmp->get_front_edge(i)+tmp->m_run*1.5+25*12), tmp->get_height(i), -94*12/4 );
        m_components.push_back( tmpB );
    }
}

void glArena::create_stands_ends( )
{
    glStairway* tmp = new glStairway();
    tmp->m_run = 24.;
    tmp->m_extrusion_length = 50*12;
    tmp->set_color( 0xFFFF00FF );
    tmp->m_number_of_steps = 2*NUMBER_STEPS;
    tmp->setup();
    tmp->m_y_angle = -90;
    tmp->relocate( NBA_COURT_WIDTH/2, 0, 0*12 );
    m_components.push_back( tmp );

    // Audience benches:
    glBench* tmpB = NULL;
    for (int i=0; i<NUMBER_STEPS; i++)
    {
        tmpB = new glBench();
        tmpB->m_length = 50*12;
        tmpB->m_height = 2*12;
        tmpB->setup();
        tmpB->m_y_angle = 0;
        tmpB->relocate( -25*12, tmp->get_height(i), tmp->get_front_edge(i)+tmp->m_run*1.5 );
        m_components.push_back( tmpB );
    }
    
    tmp = new glStairway();
    tmp->m_run = 24.;
    tmp->m_extrusion_length = 50*12;
    tmp->set_color( 0xFFFF00FF );
    tmp->m_number_of_steps = 2*NUMBER_STEPS;
    tmp->setup();
    tmp->m_y_angle = 90;
    tmp->relocate( -tmp->m_extrusion_length/2, 0, -94*12 );
    m_components.push_back( tmp );
    
    // Audience benches:
    tmpB = NULL;
    for (int i=0; i<NUMBER_STEPS; i++)
    {
        tmpB = new glBench();
        tmpB->m_length = 50*12;
        tmpB->m_height = 2*12;
        tmpB->setup();
        tmpB->m_y_angle = 0;
        tmpB->relocate( -25*12, tmp->get_height(i), -(tmp->get_front_edge(i)+tmp->m_run*1.5)-94*12 );
        m_components.push_back( tmpB );
    }
    
}

void glArena::create_components       ( )
{
    create_stands();
    create_stands_ends();
    
    // CREATE COURT:
    glBasketballCourt* bball_court = new glBasketballCourt();
    bball_court->setup();
    bball_court->relocate( 0*12, 2, -47*12 );   // centered
    m_components.push_back( bball_court);
    
    // AND ONE BASKET BALL :
    glSphere* ball = new glSphere(NBA_BASKET_BALL_RADIUS, 10, 64);
    ball->is_participating = true;
    ball->coef_of_restitution = COEFF_RESTITUTION_BASKETBALL;
    ball->set_color( 0xFF7F7F00);
    ball->load_texture("textures/basketball_texture.jpg" );
    ball->create  (  );
    ball->relocate( 0, 8*12, -94*12/2 );
    m_balls.push_back(ball);
    m_components.push_back(ball);
}

void glArena::setup( )
{
    glMolecule::setup();

    create_team1();
    create_team2();
//    position_team_initial();
    position_lineup_tunnel();
    
    
}


