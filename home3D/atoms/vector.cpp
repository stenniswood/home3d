#include <vector>
#include <string>
#include <math.h>
#include "vector.hpp"



MathVector::MathVector ( )
{
    m_name = "";
}

MathVector::MathVector ( int mDimension )
{
	m_name = "";
	dimension( mDimension );
}

MathVector::MathVector( string mName, int mDimension )
{
	m_name = mName;
	//printf("%s\n", mName.c_str() );	
	dimension( mDimension );
}
		
MathVector::~MathVector( )
{
}

void MathVector::Initialize				(	)
{
}
void MathVector::dimension( int mDimension )
{
	m_elements.clear();
	for (int i=0; i<mDimension; i++)
		m_elements.push_back( 0.0 );
}
void MathVector::set_xyz( double x, double y, double z )
{
	dimension(3);
	m_elements[0] = x;
	m_elements[1] = y;
	m_elements[2] = z;
}	

double MathVector::get_distance( MathVector& mPt2 )
{
    MathVector sub = (*this - mPt2);
    double distance = sub.magnitude();
    return distance;
}

void MathVector::print( bool mLF	)
{
    if (m_name.length())
        printf("%s\t", m_name.c_str() );
	size_t size = m_elements.size();
	for (int i=0; i<size; i++)
		printf(" %6.3f, ", m_elements[i] );
    if (mLF)
        printf("\n");
}

bool MathVector::operator==(const MathVector &rhs)
{
	size_t size = rhs.m_elements.size();
    if (m_elements.size() != size)  return false;
    
	float diff=0.0;
	float sum_diff=0.0;
	for (int i=0; i<size; i++)  {
		diff = (m_elements[i] - rhs.m_elements[i]);
		sum_diff += diff;
	}		
    // float mag = magnitude();
    // float fractional_uncertainty = fabs( sum_diff / mag );
    //printf("sdiff=%6.3f / mag=%6.3f = fractional_uncertainty=%6.3f\n", sum_diff, mag, fractional_uncertainty );
    //if (fractional_uncertainty < 0.01 )
    
    if (sum_diff==0.0)
        return true;
    else
        return false;
}
MathVector& MathVector::operator=(const MathVector &rhs)
{
	// Note, we do not copy the name!
	m_elements = rhs.m_elements;
	return *this;
}
	
MathVector MathVector::operator+(const MathVector &rhs)
{
	int size = (int)m_elements.size();
	MathVector v(size);
	for (int i=0; i<size; i++)
		v[i] = m_elements[i] + rhs.m_elements[i];
	return v;
}

MathVector MathVector::operator-(const MathVector &rhs)
{
	int size = (int)m_elements.size();
	MathVector v(size);
	for (int i=0; i<size; i++)
		v[i] = m_elements[i] - rhs.m_elements[i];
	return v;
}

void MathVector::operator/=( double mScaler )
{
    size_t size = m_elements.size();
    for (int i=0; i<size; i++)
        m_elements[i] /= mScaler;
}

void MathVector::operator*=( double mScaler )
{
	size_t size = m_elements.size();
	for (int i=0; i<size; i++)
		m_elements[i] *= mScaler;
}
	
void MathVector::operator+=(const MathVector &rhs)
{
	size_t size = m_elements.size();
	for (int i=0; i<size; i++)
		m_elements[i] += rhs.m_elements[i];
}
void MathVector::operator-=(const MathVector &rhs)
{
    size_t size = m_elements.size();
    for (int i=0; i<size; i++)
        m_elements[i] -= rhs.m_elements[i];    
}

MathVector MathVector::operator/( double mScaler )
{
    int size = (int)m_elements.size();
    MathVector v(size);
    for (int i=0; i<size; i++)
        v[i] = m_elements[i] / mScaler;
    return v;
}

MathVector MathVector::operator*( double mScaler )
{
	int size = (int)m_elements.size();
	MathVector v(size);
	for (int i=0; i<size; i++)
		v[i] = m_elements[i] * mScaler;
	return v;
}

double MathVector::dot(const MathVector &rhs)
{
	int  size = (int)m_elements.size();
	int rsize = (int)m_elements.size();
	double result=0.0;
	int msize  = min(size,rsize);	
	for (int i=0; i<msize; i++)
	{
		result += ( m_elements[i] * rhs.m_elements[i] );
	}
	return result;
}

MathVector MathVector::cross( MathVector lhs,  MathVector rhs)
{
    int  size = (int)m_elements.size();
    int rsize = (int)m_elements.size();
    if ((size!=3) || (rsize!=3)) return -1;
    
    MathVector r(3);
    
    r[0] = lhs[1]*rhs[2] - rhs[1] * lhs[2];
    r[1] = lhs[2]*rhs[0] - rhs[2] * lhs[0];
    r[2] = lhs[0]*rhs[1] - rhs[0] * lhs[1];

    return r;
}

MathVector	MathVector::get_perp_yz()
{
    MathVector perp( (int)m_elements.size() );
    perp[0] = m_elements[2];
    perp[2] = - m_elements[0];
    return perp;
}

MathVector	MathVector::get_perp_2d()
{
	MathVector perp( (int)m_elements.size() );
	perp[0] = m_elements[1];
	perp[1] = - m_elements[0];
	return perp;
}
	
double MathVector::magnitude( )
{
	double sum_sq = 0.;
	int size = (int)m_elements.size();
	for (int i=0; i<size; i++)
		sum_sq += (m_elements[i]*m_elements[i]);
	return sqrt( sum_sq );
}
			
double& MathVector::operator[](int index)
{
	return m_elements[index];	
}

Line::Line()
{
}

/*Line::Line( Line const &mline )
{
    m_origin = mline.m_origin;
    m_vector = mline.m_vector;
    t = mline.t;
}*/

Line::Line(int dimension)
{
    m_origin.dimension(3);
    m_vector.dimension(3);
}
Line::~Line()
{
}
void Line::unitize()
{
    float mag = m_vector.magnitude();
    m_vector /= mag;
}

void Line::print()
{
    printf("Line:  ");
    m_origin.print();
    m_vector.print();
}

// intersect2D_2Segments(): find the 2D intersection of 2 finite segments
//    Input:  two finite segments S1 and S2
//    Output: *I0 = intersect point (when it exists)
//            *I1 =  endpoint of intersect segment [I0,I1] (when it exists)
//    Return: 0=disjoint (no intersect)
//            1=intersect  in unique point I0
//            2=overlap  in segment from I0 to I1
#define perp(u,v) ((u[0] * v[1]) - (u[1] * v[0]))

/*int intersect2D_2Segments( LineSegment S1, LineSegment S2, Point* I0, Point* I1 )
{
    MathVector    u = S1.m_vector ;
    MathVector    v = S2.m_vector ;
    MathVector    w = S1.m_origin - S2.m_origin;
    float     D = perp(u,v);
    
    // test if  they are parallel (includes either being a point)
    if (fabs(D) < SMALL_NUM) {           // S1 and S2 are parallel
        if (perp(u,w) != 0 || perp(v,w) != 0)  {
            return 0;                    // they are NOT collinear
        }
        // they are collinear or degenerate
        // check if they are degenerate  points
        float du = dot(u,u);
        float dv = dot(v,v);
        if (du==0 && dv==0) {            // both segments are points
            if (S1.P0 !=  S2.P0)         // they are distinct  points
                return 0;
            *I0 = S1.P0;                 // they are the same point
            return 1;
        }
        if (du==0) {                     // S1 is a single point
            if  (inSegment(S1.P0, S2) == 0)  // but is not in S2
                return 0;
            *I0 = S1.P0;
            return 1;
        }
        if (dv==0) {                     // S2 a single point
            if  (inSegment(S2.P0, S1) == 0)  // but is not in S1
                return 0;
            *I0 = S2.P0;
            return 1;
        }
        // they are collinear segments - get  overlap (or not)
        float t0, t1;                    // endpoints of S1 in eqn for S2
        Vector w2 = S1.P1 - S2.P0;
        if (v.x != 0) {
            t0 = w.x / v.x;
            t1 = w2.x / v.x;
        }
        else {
            t0 = w.y / v.y;
            t1 = w2.y / v.y;
        }
        if (t0 > t1) {                   // must have t0 smaller than t1
            float t=t0; t0=t1; t1=t;    // swap if not
        }
        if (t0 > 1 || t1 < 0) {
            return 0;      // NO overlap
        }
        t0 = t0<0? 0 : t0;               // clip to min 0
        t1 = t1>1? 1 : t1;               // clip to max 1
        if (t0 == t1) {                  // intersect is a point
            *I0 = S2.P0 +  t0 * v;
            return 1;
        }
        
        // they overlap in a valid subsegment
        *I0 = S2.P0 + t0 * v;
        *I1 = S2.P0 + t1 * v;
        return 2;
    }
    
    // the segments are skew and may intersect in a point
    // get the intersect parameter for S1
    float     sI = perp(v,w) / D;
    if (sI < 0 || sI > 1)                // no intersect with S1
        return 0;
    
    // get the intersect parameter for S2
    float     tI = perp(u,w) / D;
    if (tI < 0 || tI > 1)                // no intersect with S2
        return 0;
    
    *I0 = S1.P0 + sI * u;                // compute S1 intersect point
    return 1;
}

int	Line::line_intersection_2D( Line mLine2, float& mt, float& ms )
{
    
}*/

//    Return: 0=disjoint (no intersect)
//            1=intersect  in unique point I0
//            2=overlap  in segment from I0 to I1

/* Works for 3D lines.   */
int	Line::line_intersection( Line mLine2, float& mt, float& ms )
{
    int num_var_solved = 0;
    float t=0.0;
    float s=0.0;

    // 3 Equations(x,y,z) - 2 Unknowns (s,t)
	MathVector mv1 = ( m_origin - mLine2.m_origin );
    
    // Colinear:
    if ((m_vector[0] == mLine2.m_vector[0])  && (m_vector[1] == mLine2.m_vector[1])  && (m_vector[2] == mLine2.m_vector[2]))
    {
        //  m_origin2 + m_vector2 * t = m_origin
        //  t = mv1[0] / m_vector[0]
        if (mLine2.m_vector[0] != 0)
        {    t = mv1[0] / mLine2.m_vector[0];    num_var_solved++;      }
        else if (mLine2.m_vector[1] != 0)
        {    t = mv1[1] / mLine2.m_vector[1];    num_var_solved++;      }
        else if (mLine2.m_vector[2] != 0)
        {    t = mv1[2] / mLine2.m_vector[2];    num_var_solved++;      }
        else
            return 0;       // no intersect!
    }

	// mv1 + t * m_vector = s * mLine2.m_vector	 [0]
	// mv1 + t * m_vector = s * mLine2.m_vector  [1]
    // mv1 + t * m_vector = s * mLine2.m_vector  [2]

    // if any of m_vector[0..2] or mLine2.m_vector[0..2] are 0.0.  Means 1 equation with 1 unkown.
    // Can solve for t or s right away.

    //if (num_var_solved==0)
    {
        // Difference of 2 Equations:
        // Eq:  [0]-[1]
        MathVector iv(3);
        float mult_2_by=1.;
        int eq = 1;             // subtract eq 1
        if (mLine2.m_vector[eq]==0.)
            eq = 2;
        
        if (mLine2.m_vector[eq]==0)
        {
            // we already solved t
        } else {
            mult_2_by = mLine2.m_vector[0] / mLine2.m_vector[eq];
        
            iv[0] = mv1[0]      		- mult_2_by * mv1[eq];
            iv[1] = m_vector[0] 		- mult_2_by * m_vector[eq];
            iv[2] = mLine2.m_vector[0] 	- mult_2_by * mLine2.m_vector[eq];  // goes to zero
            
            // iv[0] = -iv[1] * t;
            // t = iv[0] / -iv[1]
            mt = t = -(iv[0]/iv[1]);
        }
        MathVector resultant = mv1 + m_vector * t;
        float s1 = (resultant[0]) / mLine2.m_vector[0];
        float s2 = (resultant[1]) / mLine2.m_vector[1];
        float s3 = (resultant[2]) / mLine2.m_vector[2];
        if ((!isnan(s1)) && (s1 != 0.0))
            s = s1;
        else if ((!isnan(s2)) && (s2 != 0.0))
            s = s2;
        else if ((!isnan(s3)) && (s3 != 0.0))
            s = s3;
    }
    
    // Substitute Parameters to find the actual intersection point :
	MathVector intersection = m_origin + m_vector * t;
	intersection.m_name = "Solution1";
	//intersection.print( );

	//printf("t=%6.3f;  s=%6.3f\n", t, s);
	MathVector intersection2 = mLine2.m_origin + mLine2.m_vector * s;
	intersection2.m_name = "Solution2";
	//intersection2.print( );

    
	if (intersection == intersection2)
		return 1;
    else
        return 0;
}

LineSegment::LineSegment ()
{
    m_origin.dimension(3);
    m_vector.dimension(3);
}

LineSegment::LineSegment (MathVector msource, MathVector mdest )
{
    m_origin = msource;
    m_vector = mdest - msource;
    m_length = m_vector.magnitude();
    m_vector /= m_length;    
}

/* Assuming the point is in the line!  */
bool LineSegment::is_in_segment( MathVector mPoint)
{
    // if    m_origin + t * m_vector = mPoint
    MathVector delta = (mPoint - m_origin);
    float t1 = (delta[0] / m_vector[0]);
    //float t2 = (delta[1] / m_vector[1]);
    //float t3 = (delta[2] / m_vector[2]);
    if ((t1 > 0) && (t1 < m_length))
        return true;
    return false;
}

bool lineSegmentIntersection(double Ax, double Ay,
                             double Bx, double By,
                             double Cx, double Cy,
                             double Dx, double Dy,
                             double *X, double *Y  )
{
    
    double  distAB, theCos, theSin, newX, ABpos ;
    
    //  Fail if either line segment is zero-length.
    if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy))
        return false;
    
    //  Fail if the segments share an end-point.
    if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy) || (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy))
    {   return false;  }
    
    //  (1) Translate the system so that point A is on the origin.
    Bx-=Ax; By-=Ay;
    Cx-=Ax; Cy-=Ay;
    Dx-=Ax; Dy-=Ay;
    
    //  Discover the length of segment A-B.
    distAB=sqrt(Bx*Bx+By*By);
    
    //  (2) Rotate the system so that point B is on the positive X axis.
    theCos=Bx/distAB;
    theSin=By/distAB;
    newX=Cx*theCos+Cy*theSin;
    Cy  =Cy*theCos-Cx*theSin; Cx=newX;
    newX=Dx*theCos+Dy*theSin;
    Dy  =Dy*theCos-Dx*theSin; Dx=newX;
    
    //  Fail if segment C-D doesn't cross line A-B.
    if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.))
        return false;
    
    //  (3) Discover the position of the intersection point along line A-B.
    ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
    
    //  Fail if segment C-D crosses line A-B outside of segment A-B.
    if (ABpos<0. || ABpos>distAB) return false;
    
    //  (4) Apply the discovered position to line A-B in the original coordinate system.
    *X=Ax+ABpos*theCos;
    *Y=Ay+ABpos*theSin;
    
    //  Success.
    return true;
}


/* 
 This checks if the lines intersect at all.  And if they do, then is it within the length of this
 line segment.  mLine2 is assumed to be an infinite line.  To check it for a max length, compare 
   if (ss < Line2.m_length)
 */
int LineSegment::line_seg_intersection( LineSegment mLine2, float& ss, double* mX, double* mY )
{
    MathVector pA = m_origin        + m_vector * m_length;
    MathVector pB = mLine2.m_origin + mLine2.m_vector * mLine2.m_length;
    
    bool result = lineSegmentIntersection(
                             m_origin[0],           m_origin[2],
                             pA[0],                 pA[2],
                             mLine2.m_origin[0],    mLine2.m_origin[2],
                             pB[0],                 pB[2],
                             mX,                    mY  );
    if (result)
    {
        return 1;
    }
    return 0;
/*    float tt;
    bool within_seg1=true;
    bool within_seg2=true;
    int result = Line::line_intersection( mLine2, tt, ss );
    if (result)
    {
        within_seg1 = ((tt>0) && (tt<m_length));
        //within_seg2 = ((ss>0) && (ss<mLine2.m_length));
        if (within_seg1 && within_seg2)    // if is within the length of the line segment!
            return 1;
        else
            return 0;
    } */
    return result;
}

/*************************** SAMPLED LINE SEGMENT *************************************************/
SampledLineSegment::SampledLineSegment ()
{
    
}

SampledLineSegment::SampledLineSegment (MathVector msource, MathVector mdest, float mIncrement )
:LineSegment( msource, mdest )
{
    m_increment = mIncrement;
    
}

/* 
 Return :  Number of Samples generated
*/
int	SampledLineSegment::generate_samples( float mIncrement )
{
    m_increment = mIncrement;
    float position = 0.0;
    MathVector mv = m_origin;
    MathVector delta = m_vector * mIncrement;
    while (position<m_length)
    {
        mv += delta;
        m_samples.push_back( mv );
    }
    // Add one more exactly on the end point.
    mv = m_origin + m_vector * m_length;
    m_samples.push_back(mv);
    return (int)m_samples.size();
}

/*************************** MATH VECTOR TREE *******************************************/

/***********************  TREE MATHVECTOR  *********************************
    This class is used to create a path thru the maze.
    In other words when multiple ways to go, you can branch each option.
    Prune the branches if not valid, and get the final result.
 ***************************************************************************/
MathVectorTree::MathVectorTree()
{
    parent = NULL;
}


void MathVectorTree::clear( )
{
    list<MathVectorTree>::iterator iter = children.begin();
    while( iter != children.end() )
    {
        iter->clear();
        iter++;
    }
    children.clear();
}

void MathVectorTree::add_node( MathVector& mPoint )
{
    MathVectorTree tmp;
    tmp.parent = this;
    tmp.item = mPoint;
    children.push_back(tmp);
}

/* Puts the MathVector into a Tree node and adds to the child list */
void MathVectorTree::add_nodes( list<MathVector>& mPoints )
{
    MathVectorTree tmp;
    list<MathVector>::iterator iter = mPoints.begin();
    while( iter != mPoints.end() )
    {
        tmp.item = *iter;
        tmp.parent = this;
        children.push_back(tmp);
        iter++;
    }
}

void MathVectorTree::extract_path( vector<MathVector>& mVector )
{
    MathVectorTree* ptr = this;
    while (ptr != NULL)
    {
        mVector.push_back(ptr->item);
        ptr = ptr->parent;
    }
    std::reverse( mVector.begin(), mVector.end() );
}

// Walk up the parents to the root. recording the item at each.
void MathVectorTree::extract_path_pointers( vector<MathVectorTree*>& mVector )
{
    MathVectorTree* ptr = this;
    while (ptr != NULL)
    {
        mVector.push_back(ptr);
        ptr = ptr->parent;
    }
    std::reverse( mVector.begin(), mVector.end() );
}

