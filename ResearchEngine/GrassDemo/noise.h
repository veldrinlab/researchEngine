//
// Simple Perlin noise class
//
// Author: Alex V. Boreskoff
//

#ifndef	__NOISE__
#define	__NOISE__

#include	"Vec3.h"

class	Noise
{
private:
	enum
	{
		n = 256
	};

	int	      * p;
	Vec3  * g;

	int	fold ( int i, int j, int k ) const
	{
		return p [(p [(p[i & (n-1)] + j) & (n-1)] + k) & (n-1)];
	}

	float	drop ( float t ) const
	{
		float	ta = (float) fabs ( t );

		if ( t <= 1 )
			return 1 + ta*ta*(2*ta - 3);

		return 0;
	}

	float	omega ( long i, long j, long k, const Vec3& pt ) const
	{
		return (g [fold ( i, j, k )] & pt) * drop ( pt.x ) * drop ( pt.y ) * drop ( pt.z );
	}

	static	Vec3	large;

public:
	Noise  ( int seed = -1 );
	~Noise ();

	float	noise ( const Vec3& pt ) const
	{
		long	ip  = (long) floor ( pt.x );
		long	jp  = (long) floor ( pt.y );
		long	kp  = (long) floor ( pt.z );
		float	sum = 0;

		for ( register long i = ip; i <= ip + 1; i++ )
			for ( register long j = jp; j <= jp + 1; j++ )
				for ( register long k = kp; k <= kp + 1; k++ )
					sum += omega ( i, j, k, Vec3 ( pt.x - i, pt.y - j, pt.z - k ) );

		return sum;
	}

	Vec3	dNoise ( const Vec3& pt ) const;

	Vec3	noise3 ( const Vec3& pt ) const
	{
		return Vec3 ( noise ( pt - large ), noise ( pt ), noise ( pt + large ) );
	}

	float	turbulence       ( const Vec3& pt, int octaves ) const;
	float	turbulence       ( const Vec3& pt, float minFreq, float maxFreq ) const;
    float   signedTurbulence ( const Vec3& pt, float minFreq, float maxFreq ) const;
	float	fBm              ( const Vec3& pt, float h, float lacunarity, float octaves ) const;
};

#endif
