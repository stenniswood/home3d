//
//  sign.cpp
//  home3D
//
//  Created by Stephen Tenniswood on 6/25/15.
//  Copyright (c) 2015 Stephen Tenniswood. All rights reserved.
//

#include "sign.h"
#include "all_objects.h"

glSign::glSign	(  )
{
    m_object_type_name = "sign";
    m_width  = 160;
    m_height = 144;
}

void glSign::generate_vertices( )
{
    m_vertices.clear();
    struct Vertex v;
    v.color[0] = ((m_color & 0x00FF0000)>>16);
    v.color[1] = ((m_color & 0x0000FF00)>>8 );
    v.color[2] = ((m_color & 0x000000FF)    );
    v.color[3] = ((m_color & 0xFF000000)>>24);

    v.position[0] = 0 ;
    v.position[1] = 2.;
    v.position[2] = 0 ;
    m_vertices.push_back(v);
    
    v.position[0] = 0 ;
    v.position[1] = 2.;
    v.position[2] = m_width;
    m_vertices.push_back(v);

    v.position[0] = m_height;
    v.position[1] = 2.;
    v.position[2] = m_width;
    m_vertices.push_back(v);

    v.position[0] = m_height;
    v.position[1] = 2.;
    v.position[2] = 0 ;
    m_vertices.push_back(v);
}

void glSign::setup( )
{
    generate_vertices();
    m_indices.clear();
    m_indices.push_back( 0 );
    m_indices.push_back( 1 );
    m_indices.push_back( 2 );
    m_indices.push_back( 3 );
    
    m_texture = new Texture();    
//    m_texture->load_image( "textures/me_in_car.bmp"   );
//    m_texture->load_image( "textures/light_house.jpg" );
//    m_texture->load_image( "textures/oak_wood.jpg"    );
//    m_texture->load_image( "textures/bambo-fence-texture.jpg" );
//    m_texture->load_image( "textures/cinder-block-texture.jpg" );
    m_texture->load_image( "textures/drywall-texture.jpg" );
    m_texture->m_repetitions_x = 2.0;
    m_texture->generate_texture_coords();
    m_texture->generate_TBO           ();
    m_texture->generate_VBOTexCoords  ();
}

void glSign::draw_primitive( )
{
    glDrawElements(GL_POLYGON, (int)m_vertices.size(), GL_UNSIGNED_INT, 0 );
}




