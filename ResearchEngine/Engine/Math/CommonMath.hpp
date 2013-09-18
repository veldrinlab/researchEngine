/**
 *  File contains declaration of CommonMath class and Math class includes.
 *  @file    CommonMath.hpp
 *  @author  Szymon "Veldrin" Jab³oñski
 *  @date    2011-06-24
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef COMMONMATH2_HPP
#define COMMONMATH2_HPP

#include <stdlib.h>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

namespace AyumiEngine
{
	namespace AyumiMath
	{
		/**
		 * Template class definition of the most usefull specializations.
		 */
		typedef Vector3<> Vector3D;
		typedef Vector4<> Vector4D;
		typedef Matrix3<> Matrix3D;
		typedef Matrix4<> Matrix4D;
		typedef TurboQuaternion<> Quaternion;

		/**
		 * Class represents set of usefull math functons.
		 */
		class CommonMath
		{
		public:
			static const float PI;
			static const float HALF_PI;
			static const float QUARTER_PI;
			static const float TWO_PI;
			static const float EPSILON;

			/**
			 * Method is used to performs a bilinear interpolation.
			 * P(u,v) = e + v(f - e) 
			 * where
			 * e = a + u(b - a)
			 * f = c + u(d - c)
			 * u in range [0,1]
			 * v in range [0,1]
			 * @param	a is a param.
			 * @param	b is b param.
			 * @param	c is c param.
			 * @param	d is d param.
			 * @param	u is range value.
			 * @paam	v is range value.
			 */
			template <typename T>
			static T bilerp(const T& a, const T& b, const T& c, const T& d,const float u,const float v)
			{
				return a * ((1.0f - u) * (1.0f - v))
					+ b * (u * (1.0f - v))
					+ c * (v * (1.0f - u))
					+ d * (u * v);
			}

			/**
			 * Method is used to translate cartesian coords to spherical.
			 * @param	x is cartesian x value.
			 * @param	y is cartesian y value.
			 * @param	z is cartesian z value.
			 * @param	rho is spherical distance from origin O to point P.
			 * @param	phi is angle between OP and the XZ plane.
			 * @param	theta is angle between X-axis and OP projected onto XZ plane.
			 */
			static void cartesianToSpherical(const float x, const float y, const float z,
											 float& rho,float& phi,float& theta)
			{
				rho = sqrtf((x * x) + (y * y) + (z * z));
				phi = asinf(y / rho);
				theta = atan2f(z, x);
			}

			/**
			 * Method is used to check if two float values are close enough.
			 * @param	f1 is first value.
			 * @param	f2 is second value.
			 * @return	true if closeEnough, false otherwise.
			 */
			static bool closeEnough(const float f1,const float f2)
			{
				return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
			}

			/**
			 * Method is used to convert degress to radians.
			 * @param	degress is degress to convert.
			 * @return	converted degress.
			 */
			static float degreesToRadians(const float degrees)
			{
				return (degrees * PI) / 180.0f;
			}

			/**
			 * Method is used to convert a floating point number into an integer.
			 * Fractional values are truncated as in ANSI C.
			 * About 5 to 6 times faster than a standard typecast to an integer.
			 */
			static long floatToLong(const float f)
			{
				long fpBits = *reinterpret_cast<const long*>(&f);
				long shift = 23 - (((fpBits & 0x7fffffff) >> 23) - 127);
				long result = ((fpBits & 0x7fffff) | (1 << 23)) >> shift;

				result = (result ^ (fpBits >> 31)) - (fpBits >> 31);
				result &= ~((((fpBits & 0x7fffffff) >> 23) - 127) >> 31);

				return result;
			}

			/**
			 * Method is used to chech if value are power of 2.
			 * @param	x is value to chech.
			 * @return	true if is power of 2, false otherwise.
			 */
			static bool isPower2(const int x)
			{
				return ((x > 0) && ((x & (x - 1)) == 0));
			}

			/**
			 * Method is used to perform linear interpolation.
			 * P(t) = (1 - t)a + tb
			 *	= a + t(b - a)
			 * where
			 * t in range [0,1]
			 * @param	a is first value.
			 * @param	b is second value.
			 * @param	t is range.
			 * @return	result of linear interpolation.
			 */
			template <typename T>
			static T lerp(const T& a, const T& b, const float t)
			{
				return a + (b - a) * t;
			}

			/**
			 * Method is used to calculate the closest multiple of value that isn't less than value.
			 * @param	multiple is value to multiple.
			 * @param	value is minimum value.
			 * @return	calculated next multiple of multiple value.
			 */
			static int nextMultipleOf(const int multiple,const int value)
			{
				return multiple * ((value + (multiple - 1)) / multiple);
			}

			/**
			 * Method is used to convert radians to degrees.
			 * @param	radians is radians value to convert.
			 * @return	converted degress.
			 */
			static float radiansToDegrees(const float radians)
			{
				return (radians * 180.0f) / PI;
			}

			/**
			 * Method is used to get random numbr in defined range.
			 * @param	min is range minimum.
			 * @param	max is range maximum.
			 * @return	random number in range [min,max]
			 */
			static float random(const float min, const float max)
			{
				return min + (max - min)
					* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
			}

			/**
			 * Method is used to get smooth step, gradual translation of 'x' from 0 to 1 begining at threshold
			 * 'a' and ending at threshold 'b'.
			 * @see http://www.rendermanacademy.com/docs/smoothstep.htm
			 * @see http://www.brlcad.org/doxygen/d8/d33/noise_8c-source.html
			 * @param	a is threshold a value.
			 * @param	b is threshild b value.
			 * @param	x is gradual translation value.
			 * @reutnr	smoothstep value.
			 */
			static float smoothstep(const float a, const float b, float x)
			{
				if (x < a)
					return 0.0f;
				else if (x >= b)
					return 1.0f;
				else
				{
					x = (x - a) / (b - a);
					return x * x * (3.0f - 2.0f * x);
				}
			}

			/**
			 * Method is used to convert spherical coords to cartesian.
			 * @param	rho is dinatnce from origin O to point P.
			 * @param	phi is angle between OP and XZ plane.
			 * @param	theta is nangle between X-axis and OP projected onto XZ plane.
			 * @param	x is cartesian x coord value.
			 * @param	y is cartesian y coord value.
			 * @param	z is caretsian z coord value.
			 * @return	converted cartesian coords.
			 */
			static void sphericalToCartesian(const float rho, const float phi, const float theta,
											 float& x, float& y, float& z)
			{
				x = rho * cosf(phi) * cosf(theta);
				y = rho * sinf(phi);
				z = rho * cosf(phi) * sinf(theta);
			}
		};	
	}
}
#endif
