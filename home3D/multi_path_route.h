//
//  multi_path_route.h
//  home3D
//
//  Created by Stephen Tenniswood on 6/11/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//
/*
 This class will generate OpenGL Vertex data.
	for an object with a given set of points which establish a route thru the house
	or building.
	
	It shows as a redish/yellow line along the floor.
 It draws the object as GL_LINE_STRIP.
	
	"Follow the yellow brick road!" concept.  Though red is a bolder color!
	
	This will be generated by the "maze solving" software module.
	
 TO USE:
	a) Generating from the Map2D and Routing class.
 
 ALTERNATIVELY:
	a) Create a class of glRoute.
	b) Fill in the vertices directly.
	c) Call : create_sample()
 */

#ifndef __home3D__multi_path_route__
#define __home3D__multi_path_route__

#include <stdio.h>
#include <vector>
#include "vector.hpp"


using namespace std;


/* We may also like to keep a path.  And update the robot foot prints along the way.
 for instance if it stops (stands) to open door.  Then resumes.
 */
class glMultiRoute : public glAtom
{
public:
    glMultiRoute( );

    virtual void 	setup       ( );
    virtual void 	create      ( );    // Better to override setup and

    void            generate_sample ( );

    void 			add_way_point2  ( MathVectorTree& mNode, MathVector mPoint );
    void			start_over_at   ( struct Vertex mStart );
    void			start_over_at2  ( MathVector    mStart );
    
    virtual void	generate_layer_vertices	( );
    virtual void	generate_vertices		( MathVectorTree* mTree );

    virtual void	draw_body			 	( );
    void			print_step			 	( int mIndex, bool mLeft=true );

    MathVectorTree              m_tree;
    MathVectorTree*             m_solved_end_node;
    vector<MathVectorTree*>     m_solution;
    vector<MathVector>          FinalPath;    
};


#endif /* defined(__home3D__multi_path_route__) */
