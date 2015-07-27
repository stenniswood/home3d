/* This class will generate OpenGL Vertex data
	for a box of given width, height, depth. 
*/
#ifndef _ROOM_H_
#define _ROOM_H_

#include <vector>
#include "wall.hpp"

using namespace std;

/* Make This the glMap class!
   Derive a class from it which adds the walls (create_apartment)
 
 */
class glRoom  : public glMap2D
{
public:
	glRoom	(  );

    void    update_line_info  ( );      // wall origins and vectors.
	void 	create_apartment( );
    void    create_room     ( float width, float length );
    
	float               m_height;
	unsigned long int   m_color ;

	// FOR 2D Drawing (and maze solving):
	glMap2D             create_map( );

	vector<glObject>	m_furniture;
	vector<glObject>	m_things;

};

#endif

