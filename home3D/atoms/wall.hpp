/* This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 
	
	This makes a simple Table: a thin box for the table top.
	and 4 legs (cylinders).
*/

#ifndef _GL_BAREWALL_H_
#define _GL_BAREWALL_H_

//#include "all_objects.h"
//#include "abstract_wall.hpp"
#include <vector>
#include "door.hpp"

using namespace std;


const float DEFAULT_DOOR_WIDTH	    = 30.;
const float DEFAULT_WALL_HEIGHT 	= 10.*12.;	// inches  (was 8.*12.)
const float DEFAULT_WALL_THICKNESS  = 6.;		// inches


struct doorway_info {
    int   FirstVertexIndex;
    float position_lengthwise;
    float width;
    float height;
};
struct stSize {
    float width;
    float height;
};

class glBareWall : public glExtrusion
{
public:
	glBareWall ( );
    ~glBareWall( );
    
    void            Initialize      (   );
    virtual void    relocate        ( float mX, float mY, float mZ );
    
	void    set_texture             ( int mSelection );
	void    generate_texture_coords (   );
	void    clear                   (	);
    float   get_visible_height      (   )
            { if (m_show_half_height) return m_wall_height/2.0; else return m_wall_height; };

	bool    add_door         (  float mPositionLengthwise,
                                float mWidth       =DEFAULT_DOOR_WIDTH,
                                float mHeight      =79.0,
                                bool  mDoorwayOnly =false );

	bool    add_window        ( float mPositionLengthwise, struct stSize mSize, float mHeight=36.0 );
    bool	is_valid_location ( float mPositionLengthwise, float mWidth = 36.0 );
    
	// ADD DOORS & WIDOWS BEFORE CREATE()!
    void            set_length_height    ( float mLength, int  mHeight = DEFAULT_WALL_HEIGHT );
    void            setup                (  );
	void            create               (  );
	
    void            extract_2d_line_info (  );      // Call this if any of the m_xyz_angles have changed!
	struct Vertex	get_door_coord		 (  int mDoorIndex  );
	struct Vertex	get_door_center_coord(  int mDoorIndex  );
    struct Vertex	get_door_far_coord	 (  int mDoorIndex  );

	// VERTICES:
	void			generate_wall_vertices      ( );
	void			generate_door_vertices      ( );
	void			generate_window_vertices    ( );
	virtual void	generate_layer_vertices     ( );

	// INDICES:	
	void 			generate_marks              ( );
	void			generate_wall_indices       ( );
	void			generate_window_side_indices( );
	void 			generate_wall_side_indices  ( );
	virtual size_t 	generate_disc_indices       ( GLuint mStartingVertexIndex );

	virtual void	draw_body                   ( );
	void			print_marks                 ( );
    void            print_info                  ( );
    void            print_door_info             ( );
    void            print_window_info           ( );
    
    //=============== VARIABLES ==========================
	int             m_windows_index_start;		// index into m_indices where the window edges start.
    float           m_wall_thickness;
    float           m_wall_length;          // in inches
    float           m_wall_height;          // in inches
    bool            m_show_half_height;
    
    Line            m_line;                 // 2 vectors origin and direction.

    // These are for punching out the holes only.  don't contain the actual door.  too much info in it.
    // swing, open, hinge side, etc.  The doors, plugs, lightswitches, etc will be stored in FullWall molecule.
    
    vector<struct stWindow>     m_windows ;
    vector<struct doorway_info> m_doorways;
};


#endif
