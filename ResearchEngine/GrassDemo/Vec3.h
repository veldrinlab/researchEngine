//
// Basic class for 3 vectors.
// Supports all basic vector operations
//
// Author: Alex V. Boreskoff
//

#ifndef	__Vec3__
#define	__Vec3__

#include	<math.h>

#ifndef	EPS
	#define	EPS	0.0001f
#endif

#ifndef	M_PI								// since not all compilers define it
	#define M_PI        3.14159265358979323846f
#endif

class	Vec3
{
public:
	float	x, y, z;

	Vec3 () {}
	Vec3 ( float v ) : x ( v ), y ( v ), z ( v ) {}
	Vec3 ( float px, float py, float pz ) : x ( px ), y ( py ), z ( pz ) {}
	Vec3 ( const Vec3& v ) : x ( v.x ), y ( v.y ), z ( v.z ) {}

	Vec3& operator = ( const Vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vec3 operator - () const
	{
		return Vec3 ( -x, -y, -z );
	}

	Vec3& operator += ( const Vec3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3& operator -= ( const Vec3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	Vec3& operator *= ( const Vec3& v )
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	Vec3& operator *= ( float f )
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	Vec3& operator /= ( const Vec3& v )
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}

	Vec3& operator /= ( float f )
	{
		x /= f;
		y /= f;
		z /= f;

		return *this;
	}

	float& operator [] ( int index )
	{
		return * ( index + &x );
	}

	float operator [] ( int index ) const
	{
		return * ( index + &x );
	}

	int	operator == ( const Vec3& v ) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	int	operator != ( const Vec3& v ) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	operator float * ()
	{
		return &x;
	}

	operator const float * () const
	{
		return &x;
	}

	float	length () const
	{
		return (float) sqrtf ( x * x + y * y + z * z );
	}

    float   lengthSq () const
    {
        return x * x + y * y + z * z;
    }

	Vec3&	normalize ()
	{
		return (*this) /= length ();
	}

    float   maxLength () const
    {
        return max3 ( (float)fabs (x), (float)fabs (y), (float)fabs (z) );
    }

    float   distanceToSq ( const Vec3& p ) const
    {
        return sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z );
    }

    float   distanceTo ( const Vec3& p ) const
    {
        return (float)sqrt ( sqr ( x - p.x ) + sqr ( y - p.y ) + sqr ( z - p.z ) );
    }

	float	distanceToAlongAxis ( const Vec3& p, int axis ) const
	{
		return (float)fabs ( operator [] ( axis ) - p [axis] );
	}

	int	getMainAxis () const
	{
		int		axis = 0;
		float	val  = (float) fabs ( x );

		for ( register int i = 1; i < 3; i++ )
		{
			float	vNew = (float) fabs ( operator [] ( i ) );

			if ( vNew > val )
			{
				val  = vNew;
				axis = i;
			}
		}

		return axis;
	}

	Vec3&	clamp ( float lower, float upper );

	static	Vec3	getRandomVector ( float len = 1 );

	static	inline	Vec3 vmin ( const Vec3& v1, const Vec3& v2 )
	{
		return Vec3 ( v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y, v1.z < v2.z ? v1.z : v2.z );
	}

	static	inline	Vec3 vmax ( const Vec3& v1, const Vec3& v2 )
	{
		return Vec3 ( v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y, v1.z > v2.z ? v1.z : v2.z );
	}

    static	Vec3	zero;
    static	Vec3	one;

private:
    float   max3 ( float a, float b, float c ) const
    {
        return a > b ? (a > c ? a : c) : (b > c ? b : c);
    }

    float   min3 ( float a, float b, float c ) const
    {
        return a < b ? (a < c ? a : (b < c ? b : c)) :
                       (b < c ? b : (a < c ? a : c));
    }

    float   sqr ( float x ) const
    {
        return x*x;
    }
};

inline Vec3 operator + ( const Vec3& u, const Vec3& v )
{
	return Vec3 ( u.x + v.x, u.y + v.y, u.z + v.z );
}

inline Vec3 operator - ( const Vec3& u, const Vec3& v )
{
	return Vec3 ( u.x - v.x, u.y - v.y, u.z - v.z );
}

inline Vec3 operator * ( const Vec3& u, const Vec3& v )
{
	return Vec3 ( u.x*v.x, u.y*v.y, u.z * v.z );
}

inline Vec3 operator * ( const Vec3& v, float a )
{
	return Vec3 ( v.x*a, v.y*a, v.z*a );
}

inline Vec3 operator * ( float a, const Vec3& v )
{
	return Vec3 ( v.x*a, v.y*a, v.z*a );
}

inline Vec3 operator / ( const Vec3& u, const Vec3& v )
{
	return Vec3 ( u.x/v.x, u.y/v.y, u.z/v.z );
}

inline Vec3 operator / ( const Vec3& v, float a )
{
	return Vec3 ( v.x/a, v.y/a, v.z/a );
}

inline Vec3 operator / ( float a, const Vec3& v )
{
	return Vec3 ( a / v.x, a / v.y, a / v.z );
}

inline float operator & ( const Vec3& u, const Vec3& v )
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline Vec3 operator ^ ( const Vec3& u, const Vec3& v )
{
	return Vec3 (u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

inline float dot ( const Vec3& a, const Vec3& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross ( const Vec3& u, const Vec3& v )
{
	return Vec3 (u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

inline	Vec3 lerp ( const Vec3& a, const Vec3& b, float t )
{
	return a + t * (b - a);
}

inline	float	mixedProduct ( const Vec3& a, const Vec3& b, const Vec3& c )
{
	return ( a & ( b ^ c ) );
}

inline	bool	areCollinear ( const Vec3& a, const Vec3& b, const Vec3& c )
{
	return ((b - a) ^ (c - a)).lengthSq () < EPS * EPS;
}

inline	bool	areComplanar ( const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d )
{
	return fabs ( mixedProduct ( b - a, c - a, d - a ) ) < EPS * EPS * EPS;
}

inline Vec3 reflect ( const Vec3& v, const Vec3& n )	// for unit vectors only
{
	return v - (2.0f * dot (v, n )) * n;
}

#endif
