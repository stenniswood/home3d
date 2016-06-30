/* This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 
*/
#ifndef _ROOM_H_
#define _ROOM_H_

#include <vector>
#include "rectangle.h"
#include "full_wall.h"

#include "super_string.hpp"

using namespace std;

/* Make This the glMap class!
   Derive a class from it which adds the walls (create_apartment)
 
 */
class Room  : public Rectangle
{
public:
	Room  ();
    ~Room ();

    void    place_next_to_x   ( Room& mReference );
    void    place_next_to_y   ( Room& mReference );
    MathVector get_room_center( );
    
    bool    contains          ( MathVector mLocation );
    
    void    create_room       ( float width, float length );      // create the walls bounding the room.
    void    update_line_info  ( );      // wall origins and vectors.
    
    
	SuperString                 m_names;
private:
    float                       m_height;
    
    vector<glFullWall*>         m_bounding_walls;
    vector<glDoor*>             m_doors;
    vector<struct stWindow*>    m_windows;
    
};


int scan_rooms( vector<Room>& mRooms, MathVector mv );


#endif
