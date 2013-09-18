//
// Basic class for 3 vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#include	<stdlib.h>
#include	"Vec3.h"

Vec3	Vec3 :: zero ( 0, 0, 0 );
Vec3	Vec3 :: one  ( 1, 1, 1 );


static	inline	float	rnd ()		// return float random number in [-1,1]
{
	return 2.0f * float ( rand () ) / float ( RAND_MAX ) - 1.0f;
}

Vec3	Vec3 :: getRandomVector ( float len )
{
	Vec3	v;

	for ( ; ; )
	{
		v.x = rnd ();
		v.y = rnd ();
		v.z = rnd ();

		if ( v.lengthSq () < EPS )
			continue;

		v *= len / v.length ();

		return v;
	}
}

Vec3&	Vec3 :: clamp ( float lower, float upper )
{
	if ( x < lower )
		x = lower;
	else
	if ( x > upper )
		x = upper;

	if ( y < lower )
		y = lower;
	else
	if ( y > upper )
		y = upper;
			
	if ( z < lower )
		z = lower;
	else
	if ( z > upper )
		z = upper;

	return *this;
}

