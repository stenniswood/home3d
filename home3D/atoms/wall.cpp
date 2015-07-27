/* Strategy:
		We'll keep a list of vertices in 2 dimensions; starting with 
		4 defining a rectangle length and height of wall.
		
		if a door is added.  the vertices are added.
		
		To generate the indices:
			scan the vertices starting at the left.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include "all_objects.h"
#include "door.hpp"

#define Debug 0

vector<int>  floor_marks;		// store the indices (not the value) for coordinating the opengl rectangles.
vector<int>  ceiling_marks;		// store the indices (not the value)
double radians(double deg)
{
    return deg * M_PI / 180.0;
}

glBareWall::glBareWall( )
{
    m_object_class = 10;
    Initialize();
}
glBareWall::~glBareWall( )
{
    
}

void glBareWall::Initialize()
{
    m_name = "bare wall";
    m_wall_thickness = DEFAULT_WALL_THICKNESS;
    m_wall_height       = DEFAULT_WALL_HEIGHT;
    m_wall_length       = 10.*12.;
    m_show_half_height  = false;    
    m_texture           = NULL;
    m_windows_index_start = 0;
    m_color               = 0xFFFF8F8F;
    clear();
}

void glBareWall::clear()
{
	m_vertices.clear();
	m_indices.clear();
    m_doorways.clear();
    
	floor_marks.clear();
	ceiling_marks.clear();
    
    m_layer_one_vertices  = 0;
	m_layer_one_indices   = 0;
	m_number_side_indices = 0;
}

void	glBareWall::generate_layer_vertices( )
{
    generate_wall_vertices	( );
    generate_door_vertices	( );
    generate_window_vertices( );
    m_layer_one_vertices = m_vertices.size();
}


/* VERTICE PATTERN:
	0	floor start
	1	floor end
	2	ceiling end
	3	ceiling start
	d1 + 0  door floor
	d1 + 1  door height
	d1 + 2  door ceiling
	d1 + 3	door far floor
	d1 + 4	door far height
	d1 + 5	door far ceiling
	d2...
	w1 + 0	window floor
	w1 + 1	window sill
	w1 + 2	window top
	w1 + 3	window ceiling
	w1 + 4	window far floor
	w1 + 5	window far sill
	w1 + 6	window far top
	w1 + 7	window far ceiling 
*/

/* Generates the 4 corners of the wall. On 1 side only. */
void	glBareWall::generate_wall_vertices( )
{
	// GENERATE WALL:  (four points)    a Wall is an extrusion!
	struct Vertex v;
	v.color[0] = 0xBF; v.color[1] = 0xBF;  v.color[2] = 0xBF; v.color[3] = 0xBF;
	v.position[0] = 0.0;
	v.position[1] = 0.0;
    v.position[2] = 0.0;
	m_vertices.push_back( v );
	v.position[0] = m_wall_length;
	v.position[1] = 0.0;
	v.position[2] = 0.0;	
	m_vertices.push_back( v );
	v.position[0] = m_wall_length;
	v.position[1] = get_visible_height();
	v.position[2] = 0.0;	
	m_vertices.push_back( v );
	v.position[0] = 0.0;
	v.position[1] = get_visible_height();
	v.position[2] = 0.0;	
	m_vertices.push_back( v );
}

/* Generates the 4 corners of the door + 2 extra headers. On 1 side only. */
void glBareWall::generate_door_vertices( )
{
	struct Vertex v;
	v.color[0] = 0xFF; v.color[1] = 0x7F;  v.color[2] = 0xFF; v.color[3] = 0xFF;
    
	for (int d=0; d<m_doorways.size(); d++)
	{		
		// We'll add 6 vertices ( 4 of the doorway itself and 
		//   					  2 for the header meets the ceiling)	
		m_doorways[d].FirstVertexIndex = (int)m_vertices.size();

		v.position[0] = m_doorways[d].position_lengthwise;			// Corner 0
		v.position[1] = 0.0;
		v.position[2] = 0.0;
		m_vertices.push_back( v );
		v.position[0] = m_doorways[d].position_lengthwise;			// Corner 1
		v.position[1] = m_doorways[d].height;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );	
		v.position[0] = m_doorways[d].position_lengthwise;			// Also add first top of header
        v.position[1] = m_wall_height;//  get_visible_height();
		v.position[2] = 0.0;	
		m_vertices.push_back( v );									

		v.position[0] = m_doorways[d].position_lengthwise + m_doorways[d].width;	// Corner 2
		v.position[1] = 0.0;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_doorways[d].position_lengthwise + m_doorways[d].width;	// Corner 3
		v.position[1] = m_doorways[d].height;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_doorways[d].position_lengthwise + m_doorways[d].width;	// Also add 2nd top of header
		v.position[1] = m_wall_height;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
	}
}

/* Generates the 4 corners of the window + 2 headers + 2 footers = 8 vertices.  */
void glBareWall::generate_window_vertices( )
{
	struct Vertex v;
	v.color[0] = 0xFF; v.color[1] = 0xFF;  v.color[2] = 0xFF; v.color[3] = 0xFF;  	
	for (int w=0; w<m_windows.size(); w++)
	{
		m_windows[w].FirstVertexIndex = (int)m_vertices.size();

		// We'll add 8 vertices ( 4 of the window itself,
		//   					  2 for the header meets the ceiling,
		//						  2 where the window meets the floor )
		v.position[0] = m_windows[w].PositionLengthwise;			// front edge: Floor
		v.position[1] = 0.0;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise;			// front edge: Sill
		v.position[1] = m_windows[w].SillHeight;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise;			// front edge: Top
		v.position[1] = m_windows[w].SillHeight + m_windows[w].Height;
        v.position[2] = 0.0;
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise;			// front edge: Ceiling
		v.position[1] = get_visible_height();
		v.position[2] = 0.0;
		m_vertices.push_back( v );	

		v.position[0] = m_windows[w].PositionLengthwise + m_windows[w].Width;	// Far edge: Floor
		v.position[1] = 0.0;
		v.position[2] = 0.0;
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise + m_windows[w].Width;	// Far edge: Sill
		v.position[1] = m_windows[w].SillHeight;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise + m_windows[w].Width;	// Far edge: Window Top
		v.position[1] = m_windows[w].SillHeight + m_windows[w].Height;
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
		v.position[0] = m_windows[w].PositionLengthwise + m_windows[w].Width;	// Far edge: Ceiling
		v.position[1] = get_visible_height();
		v.position[2] = 0.0;	
		m_vertices.push_back( v );
	}
	//if (Debug) printf("generate_window_vertices()  %lu vertices\n", m_vertices.size() );
}

/*   For a window or door  */
bool glBareWall::is_valid_location( float mPositionLengthwise, float mWidth )
{
    float start = 0;
    float stop  = 0;
    float NewDoorStop = 0;
    NewDoorStop = mPositionLengthwise + mWidth;

    // NO OVERLAPPING ANOTHER DOOR :
    for (int d=0; d<m_doorways.size(); d++)
    {
        start = m_doorways[d].position_lengthwise;
        stop  = m_doorways[d].position_lengthwise + m_doorways[d].width;
        
        if ((mPositionLengthwise>start) &&
            (mPositionLengthwise<stop) )
        {
            printf("Requested position is in middle of door #%d\n", d);
            return false;
        }
        if ((NewDoorStop > start) && (NewDoorStop < stop) )
        {
            printf("Requested End Position is in middle of door #%d\n", d);
            return false;
        }
    }
    // OVERLAPPING A WINDOW?
    
    // WITHIN WALL LENGTH?
    if ((mPositionLengthwise + mWidth) > m_wall_length)
    {
        printf("Requested position is after wall end!\n");
        return false;
    }
    if ((mPositionLengthwise) < 0.0)
    {
        printf("Requested position is before wall start!\n");
        return false;
    }
    return true;	
}

/* Extracts and puts into the abWall object */
void glBareWall::extract_2d_line_info()
{
    m_line.m_origin.dimension(3);
    m_line.m_origin[0] = m_x;
    m_line.m_origin[1] = 2.0;
    m_line.m_origin[2] = m_z;
    
    // Convert the wall angle to a unit vector.  This might better be done when creating the dwelling in the first place.
    float rad = -radians(m_y_angle);
    m_line.m_vector.dimension(3);
    m_line.m_vector[0] = cos(rad);
    m_line.m_vector[1] = 0.0;        // y is height!
    m_line.m_vector[2] = sin(rad);
}

void glBareWall::relocate( float mX, float mY, float mZ )
{
    glAtom::relocate(mX,mY,mZ);
    extract_2d_line_info();
}

/*
INPUT:
    mPositionLengthwise - from starting edge of the wall and along to front edge of door.
Return:	true 	if added.
		false	not added - another in the way.
*/
bool  glBareWall::add_door( float mPositionLengthwise,
                        float mWidth,
                        float mHeight,
                        bool  mDoorwayOnly  )
{
    struct doorway_info    di;
    di.position_lengthwise = mPositionLengthwise;
    di.width  = mWidth;
    di.height = mHeight;

    bool okay = is_valid_location( di.position_lengthwise, di.width );
    if (okay)
    {
        m_doorways.push_back( di );
        if (mDoorwayOnly==false)
        {
        }
    }
	return true;
}


bool glBareWall::add_window( float mPositionLengthwise, struct stSize mSize, float mHeight )
{
    if ( is_valid_location( mPositionLengthwise, mSize.width )==false )
    {
        printf("Cannot add window at %6.1f of width=%6.1f, height=%6.1f.  Wall length is %6.1f\n",
               mPositionLengthwise, mSize.width, mHeight, m_wall_length );
        return false;
    }
    struct stWindow w;
    w.PositionLengthwise = mPositionLengthwise;
    w.Width              = mSize.width;
    w.Height             = mSize.height;
    w.SillHeight         = mHeight;
    m_windows.push_back(w);
    return true;
}

void glBareWall::set_length_height( float mLength, int  mHeight )
{
    m_wall_length = mLength;
    m_wall_height = mHeight;
}

void glBareWall::setup( )
{
    // VERTICES :
    generate_layer_vertices ( );
    extrude_vertices        ( m_wall_thickness, 2 );		// generates new vertices
    
    // INDICES :
    generate_disc_indices   ( 0 );    
}

/* Call this :
	After constructing, AFTER adding any doors.

NOTE: If you want to change the vertices color, it must be done inside this function.
	  (before generate_VBO() and after all vertices are created!)
*/
void  glBareWall::create( )
{
    setup();
    gl_register();
}

size_t 	glBareWall::generate_disc_indices( GLuint mStartingVertexIndex )
{
	generate_marks			( );
	generate_wall_indices	( );		// GL_QUADS - m_layer_one_indices

	/*  Because there are multiple sets of sides, this is slightly different than
    	what glExtrusion will do.   */    
    // We need indices for drawing the other side.	
	generate_otherside_indices();			//   - m_layer_one_indices
	generate_wall_side_indices();			//  The edges of the wall (outside perimeter)
											//  (includes edges of the doorways)
	generate_window_side_indices();			//  The edges of each window.  
	return (GLuint)m_indices.size();
}

/* 
	Generates the side edges all around the wall and around the doorways.
	Does not do the sides edges of the windows (discontinuous).
*/
void glBareWall::generate_wall_side_indices()
{
	// assume the floors & marks are still active.
	// These indices :  Should go bottom, to next door.  trace around the doorway,
	//					then continue on bottom to next door, trace around doorway, etc.
	//					until end of wall.  then trace up the end, across the top and back
	//					down to the bottom starting point.
	size_t i_count = m_indices.size();
	m_indices.push_back( 0 );           m_indices.push_back( 0+(GLuint)m_layer_one_vertices );

	GLubyte fi;
	for (int d=0; d<m_doorways.size(); d++)
	{
        fi = m_doorways[d].FirstVertexIndex;                                                        // door floor
		m_indices.push_back( fi   );	m_indices.push_back( fi+(GLuint)  m_layer_one_vertices );       // door height
        
        // Over the door polygon :
		m_indices.push_back( fi+1 );	m_indices.push_back( fi+1+(GLuint)m_layer_one_vertices );		// door far height
		m_indices.push_back( fi+4 );	m_indices.push_back( fi+4+(GLuint)m_layer_one_vertices );		// door far floor

        // Start the next run of the wall :
		m_indices.push_back( fi+3 );	m_indices.push_back( fi+3+(GLuint)m_layer_one_vertices );
	}

	// We now have the bottom (up to the last door) and all doorway frames :
	// Go across bottom to end of wall:
	m_indices.push_back( 1 );		m_indices.push_back( 1+(GLuint)m_layer_one_vertices );	// To ceiling at end of wall:
	m_indices.push_back( 2 );		m_indices.push_back( 2+(GLuint)m_layer_one_vertices );	// To ceiling at start of wall:
	m_indices.push_back( 3 );		m_indices.push_back( 3+(GLuint)m_layer_one_vertices );	// To floor at start of wall:
	m_indices.push_back( 0 );		m_indices.push_back( 0+(GLuint)m_layer_one_vertices );
	m_number_side_indices = m_indices.size()-i_count;
}

// GENERATES THE INSIDE OF THE WINDOWS :
void glBareWall::generate_window_side_indices()
{
	m_windows_index_start = (GLuint)m_indices.size();
	GLuint fi;
	for (int w=0; w<m_windows.size(); w++)
	{
		fi = m_windows[w].FirstVertexIndex;
		// Do the four corners:
		m_indices.push_back( fi+1 );		m_indices.push_back( fi+1+(GLuint)m_layer_one_vertices );
		m_indices.push_back( fi+2 );		m_indices.push_back( fi+2+(GLuint)m_layer_one_vertices );
		m_indices.push_back( fi+6 );		m_indices.push_back( fi+6+(GLuint)m_layer_one_vertices );
		m_indices.push_back( fi+5 );		m_indices.push_back( fi+5+(GLuint)m_layer_one_vertices );
		// close out:
		m_indices.push_back( fi+1 );		m_indices.push_back( fi+1+(GLuint)m_layer_one_vertices );
	}
}

/* VERTICE PATTERN:
	0	floor start
	1	floor end
	2	ceiling end
	3	ceiling start
	d1 + 0  door floor
	d1 + 1  door height
	d1 + 2  door ceiling
	d1 + 3	door far floor
	d1 + 4	door far height
	d1 + 5	door far ceiling
	d2...
	w1 + 0	window floor
	w1 + 1	window sill
	w1 + 2	window top
	w1 + 3	window ceiling
	w1 + 4	window far floor
	w1 + 5	window far sill
	w1 + 6	window far top
	w1 + 7	window far ceiling
*/

/* 
	Mark the wall all along the way with both doors and Windows
	for simplicity of generating the indices.	
*/
void glBareWall::generate_marks( )
{
	int d = 0;
	int w = 0;
	float d_value = 0.0;
	float w_value = 0.0;
    floor_marks.clear();
    ceiling_marks.clear();
	floor_marks.push_back(0);		// start of wall.
	ceiling_marks.push_back(3);		// start of wall.	
	bool done       = false;
	int v_index     = 0;
    int num_doors   = (int)m_doorways.size();
    int num_windows = (int)m_windows.size();
    while (!done)
	{
		if (d < num_doors)
            d_value = m_doorways[d].position_lengthwise;
		else 
			d_value = m_wall_length;
        
		if (w<num_windows)
			w_value = m_windows[w].PositionLengthwise;
		else 
			w_value = m_wall_length;
        //printf("generate_marks() d=%d; FirstIndex=%d  d_value=%6.1f\n", d, m_doors[d].FirstVertexIndex, d_value);
        //printf("wall end=%6.1f\n", m_wall_length);

        if ((d_value < w_value))
        {
            // in case window off the end of the wall.
            //if (d >= num_doors)    done = true;
            //else {
            //printf("Adding door to indices marks\n");
            // ADD INDICES for drawing WALL PANELS (not headers)
            // So allow wall to come right up to the door:			
            v_index =  m_doorways[d].FirstVertexIndex + 0;	// door floor
            floor_marks.push_back  ( v_index );
            v_index =  m_doorways[d].FirstVertexIndex + 2;	// door ceiling index
            ceiling_marks.push_back( v_index );
            
            // Then allow wall start again right after the door:			
            v_index =  m_doorways[d].FirstVertexIndex + 3;	// door far floor
            floor_marks.push_back  ( v_index );
            v_index =  m_doorways[d].FirstVertexIndex + 5;	// door far ceiling index
            ceiling_marks.push_back( v_index );			
            // Then it comes back to ____.
            // 		index 3 for the first door.
            // for 2nd and later doors, comes back to prev door ceiling, then floor.				
            d++;
            //}
		}
        else if (w<num_windows)
        {
			// ADD INDICES for drawing WALL PANELS (not headers)
			// So allow wall to come right up to the door:			
			v_index =  m_windows[w].FirstVertexIndex + 0;	// window floor
			floor_marks.push_back  ( v_index );
			v_index =  m_windows[w].FirstVertexIndex + 3;	// window ceiling index.
			ceiling_marks.push_back( v_index );
			
			// Then allow wall start again right after the door:		
			v_index =  m_windows[w].FirstVertexIndex + 4;	// window far floor
			floor_marks.push_back  ( v_index );
			v_index =  m_windows[w].FirstVertexIndex + 7;	// window far ceiling index.
			ceiling_marks.push_back( v_index );			
			w++;
		}
		if ((d>=m_doorways.size())	&& (w>=m_windows.size()))
			done = true;
	}
	// Last add the far wall ends:
	floor_marks.push_back  ( 1 );
	ceiling_marks.push_back( 2 );
}

void glBareWall::print_info( )
{
    printf("Wall Info:\n");
    printf("length=%6.1f;  height=%6.1f;\n", m_wall_length, m_wall_height );
    
    printf("Number Doors  :%lu \n", m_doorways.size() );
    printf("Number Windows:%lu \n", m_windows.size()  );
    
    printf("Located at: \n\t");    m_line.m_origin.print();
    printf("\t");    m_line.m_vector.print();
}

void glBareWall::print_door_info( )
{
    printf("Number Doors  :%lu \n", m_doorways.size() );
    for (int d=0; d<m_doorways.size(); d++)
    {
        printf("\tDoor   #%d : Distance: %6.1f\t", d, m_doorways[d].position_lengthwise );
        printf("w=%6.1f; h=%6.1f;\n", m_doorways[d].width, m_doorways[d].height );
        if (d<m_doorways.size()-1) printf("\n");
    }
}

void glBareWall::print_window_info( )
{
    for (int w=0; w<m_windows.size(); w++)
    {
        printf("\tWindow #%d : Distance: %6.1f;\tw=%6.1f; h=%6.1f\t", w, m_windows[w].PositionLengthwise,

                              m_windows[w].Width, m_windows[w].Height );
        printf("Sill Height=%6.1f;  \n", m_windows[w].SillHeight );
        if (w<(m_windows.size()-1)) printf("\t");
    }
}

void glBareWall::print_marks( )
{
	printf("ceil:\t");
	for (int i=0; i<ceiling_marks.size(); i++)
	{
		printf("%d, ", ceiling_marks[i]);		
	}
	printf("\nfloor:\t");
	for (int i=0; i<floor_marks.size(); i++)
	{
		printf("%d, ", floor_marks[i] );
	}
	printf("\n");
}

/*  
	This makes:  the wall for 1 side.  Setup for GL_QUADS	
*/
void glBareWall::generate_wall_indices( )
{
	// We will be drawing as GL_QUADS.	They go counter clockwise.

	// FIRST DO ALL OF THE NON-DOOR/WINDOW PLACES:
	for (int i=0; i<floor_marks.size()-1; i+=2)
	{
		m_indices.push_back( floor_marks[i]     );		// 0 is the floor start of wall.
		m_indices.push_back( floor_marks[i+1]   );
		m_indices.push_back( ceiling_marks[i+1] );
		m_indices.push_back( ceiling_marks[i]   );
	}
	// NEXT DO THE DOOR HEADERS:
	for (int d=0; d<m_doorways.size(); d++)
	{
		m_indices.push_back( m_doorways[d].FirstVertexIndex + 1 );
		m_indices.push_back( m_doorways[d].FirstVertexIndex + 4 );
		m_indices.push_back( m_doorways[d].FirstVertexIndex + 5 );
		m_indices.push_back( m_doorways[d].FirstVertexIndex + 2 );
	}

	// NEXT DO THE WINDOW HEADERS:
	for (int w=0; w<m_windows.size(); w++)
	{
		m_indices.push_back( m_windows[w].FirstVertexIndex + 2 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 6 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 7 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 3 );
	}
	// NEXT DO THE WINDOW FOOTERS (the part below the window):
	for (int w=0; w<m_windows.size(); w++)
	{
		m_indices.push_back( m_windows[w].FirstVertexIndex + 0 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 4 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 5 );
		m_indices.push_back( m_windows[w].FirstVertexIndex + 1 );
	}

	m_layer_one_indices = m_indices.size();
	//printf("wall indices = %d\n", m_indices.size() );
}

/****** Door Vertices ********/
struct Vertex glBareWall::get_door_coord( int mDoorIndex )
{
	int v_index = m_doorways[mDoorIndex].FirstVertexIndex;
	return m_vertices[v_index];
}
struct Vertex glBareWall::get_door_center_coord( int mDoorIndex )
{
    int v_index   = m_doorways[mDoorIndex].FirstVertexIndex;
	struct Vertex v1 =  m_vertices[v_index  ];
	struct Vertex v2 =  m_vertices[v_index+1];
	struct Vertex vc;
	vc.color[0] = v1.color[0];
	vc.color[1] = v1.color[1];
	vc.color[2] = v1.color[2];
	vc.color[3] = v1.color[3]; 
	vc.position[1] = v1.position[1];                                // Floor Height.
	vc.position[0] = (v1.position[0] + v2.position[0]) / 2.;        // Average close & far sides.
	vc.position[2] = (v1.position[2] + v2.position[2]) / 2.;        //
	return vc;
}
struct Vertex glBareWall::get_door_far_coord(  int mDoorIndex  )
{
    int v_index = m_doorways[mDoorIndex].FirstVertexIndex;
	return m_vertices[v_index+1];
}
/****** Door Vertices ********/


void	glBareWall::draw_body()
{
	//Make the new VBO active. Repeat here incase changed since initialisation
	glBindBuffer        (GL_ARRAY_BUFFER, 		  m_VBO	);
	glVertexPointer     (3, GL_FLOAT, 		  sizeof(struct Vertex), NULL);
	glColorPointer      (4, GL_UNSIGNED_BYTE, sizeof(struct Vertex), (GLvoid*)(offsetof(struct Vertex,color)));

	glBindBuffer        (GL_ELEMENT_ARRAY_BUFFER, m_IBO );
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);

	// FIRST IS DRAW WALL : 
	if (m_texture)
        m_texture->draw();
	glDrawElements(GL_QUADS, (GLuint)(2*m_layer_one_indices), GL_UNSIGNED_INT, (GLvoid*)((char*)NULL) );
	if (m_texture)
        m_texture->after_draw();

	// DRAW WALL EDGE:
	// These indices :  Should go bottom, to next door.  trace around the doorway,
	//					then continue on bottom to next door, trace around doorway, etc.
	//					until end of wall.  then trace up the end, across the top and back
	//					down to the bottom starting point.	
	glDrawElements(GL_QUAD_STRIP, (GLuint)m_number_side_indices, GL_UNSIGNED_INT,
					(GLvoid*)BUFFER_OFFSET(m_layer_one_indices*2)  );

	// 	DRAW WINDOWS SIDES : (separate draw functions)
	// Note:  number of indices to draw is (4+1)*2=10  four vertices plus closeout *2
	int number_indices_to_draw = (4+1)*2;
	for (int w=0; w<m_windows.size(); w++)
		glDrawElements(GL_QUAD_STRIP, number_indices_to_draw, GL_UNSIGNED_INT, 
					  (GLvoid*)BUFFER_OFFSET(m_windows_index_start + w*number_indices_to_draw) );
}

void glBareWall::set_texture( int mSelection )
{
     m_texture = new Texture( );
     switch (mSelection)
     {
     case 0:	m_texture->load_image("textures/light-stucco-texture.jpg" 	);  break;
     case 1:	m_texture->load_image("textures/stucco-texture.jpg" 		);  break;
     case 2:	m_texture->load_image("textures/stucco-2-texture.jpg" 		);  break;
     case 3:	m_texture->load_image("textures/wall-texture.jpg" 			);  break;
     case 4:	m_texture->load_image("textures/drywall-texture.jpg" 		);	break;
     case 5:	m_texture->load_image("textures/concrete-texture.jpg" 		);  break;
     case 6:	m_texture->load_image("textures/bump-wall-texture.jpg" 		);  break;
     case 7:	m_texture->load_image("textures/bamboo-fence-texture.jpg"	);  break;
     default:  break;
     }
     generate_texture_coords 		( );
     m_texture->generate_TBO		( );
     m_texture->generate_VBOTexCoords( );
}

void glBareWall::generate_texture_coords( )
{
     // We match vertices here, not indices.
    // First 4 are beginning and ending of the wall.
     const float fs = 1.0;
     struct stTextCoord t;
     t.u = 0.;              t.v = 0.;			// Vertex 0,0
     m_texture->m_TexCoords.push_back( t );
     t.u = fs;			t.v = 0.;			// Vertex 0,0   (wall length, floor)
     m_texture->m_TexCoords.push_back( t );
     t.u = fs;			t.v = fs;			// Vertex 0,0   (wall length, ceiling)
     m_texture->m_TexCoords.push_back( t );
     t.u = 0.;              t.v = fs;			// Vertex 0,0   (start, ceiling)
     m_texture->m_TexCoords.push_back( t );

    t.u = 0.;              t.v = 0.;			// Vertex 0,0
    m_texture->m_TexCoords.push_back( t );
    t.u = fs;			t.v = 0.;			// Vertex 0,0   (wall length, floor)
    m_texture->m_TexCoords.push_back( t );
    t.u = fs;			t.v = fs;			// Vertex 0,0   (wall length, ceiling)
    m_texture->m_TexCoords.push_back( t );
    t.u = 0.;              t.v = fs;			// Vertex 0,0   (start, ceiling)
    m_texture->m_TexCoords.push_back( t );
    
    /*
     for (int d=0; d<m_doors.size(); d++)
     {
     // We'll add 6 vertices
     // ( 4 of the doorway itself and 2 for the header meets the ceiling)
     t.u = 1.0;			// because end of prev wall segment.
     t.v = 0.0;
     m_texture->m_TexCoords.push_back( t );
     
     t.u = 1.0;			// because end of prev wall segment.
     t.v = m_doors[d].Height / get_visible_height();
     m_texture->m_TexCoords.push_back( t );
     
     t.u = 1.0;			// because end of prev wall segment.
     t.v = 1.0;
     m_texture->m_TexCoords.push_back( t );
     
     t.u = 0.0;			// Corner 2
     t.v = 0.0;
     m_texture->m_TexCoords.push_back( t );
     
     t.u = 0.0;			// Corner 3
     t.v = m_doors[d].Height / get_visible_height();
     m_texture->m_TexCoords.push_back( t );
     
     t.u = 0.0;			// 
     t.v = 1.0;
     m_texture->m_TexCoords.push_back( t );
     }
     */
}

