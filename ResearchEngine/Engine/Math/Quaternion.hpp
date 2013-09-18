/**
 *  File contains declaration and definition of Quaternion classes.
 *  @file    Quaternion.hpp
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

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Matrix.hpp"

namespace AyumiEngine
{
	namespace AyumiMath
	{
		/**
		 * Class represents unit space TurboQuaternion class witch is used to represent rotation
		 * of object in three dimmension space. Extends Vector4 class.
		 */
		template <class T = GLfloat>
		class TurboQuaternion : public Vector4<>
		{
		public:
			TurboQuaternion();
			TurboQuaternion(const T* q);
			TurboQuaternion(const T x, const T y, const T z, const T w);
			TurboQuaternion(const Matrix4<T>& m);
			TurboQuaternion(const Vector3<T>& axis, T angle);

			operator Vector4<T>();
			TurboQuaternion(const Vector4<T>& v);

			const T norm() const;
			const Matrix4<T> matrix4() const;
			const Matrix3<T> matrix3() const;
			TurboQuaternion<T> conjugate();
			TurboQuaternion<T> inverse();
			void getAxisAngle(Vector3<T>& axis, T& angle) const;

			TurboQuaternion<T>& operator*=( const TurboQuaternion<T>& q);
		};

		template <class T> TurboQuaternion<T> conjugate(const TurboQuaternion<T>&  q);
		template <class T> TurboQuaternion<T> inverse(const TurboQuaternion<T>&  q);
		template <class T> TurboQuaternion<T> slerp(const TurboQuaternion<T>&  p, const TurboQuaternion<T>&  q, T alpha);
		template <class T> TurboQuaternion<T> operator*(const TurboQuaternion<T>&  q1, const TurboQuaternion<T>&  q2);

		/**
		 * Class default constructor.
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(): Vector4<T>()
		{

		}

		/**
		 * Class constructor with vector parameter.
		 * @param	q is pointer array data.
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(const T* q): Vector4<T>(q)
		{

		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	x is quaternion x value.
		 * @param	y is quaternion y value.
		 * @param	z is quaternion z value.
		 * @param	w is quaternion w value.
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(const T x, const T y, const T z, const T w): Vector4<T>(x, y, z, w)
		{

		}

		/**
		 * Class conversion to Vector4 operator.
		 * @return	Vector4 class object of Quaternion data.
		 */
		template <class T> TurboQuaternion<T>::operator Vector4<T>() 
		{ 
			return Vector4D<T>( xyzw );
		}

		/**
		 * Class constructor from Vector4 class.
		 * @param	v is source Vector
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(const Vector4<T>& v): Vector4<T>(v)
		{

		}

		/**
		 * Class constructor from rotation Matrix4.
		 * @param	m is source rotation matrix.
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(const Matrix4<T>&  m)
		{
			T tr = m[0] + m[5] + m[10];

			if ( tr > 0.0 )
			{
				T s = static_cast<T> ( sqrt( tr + m[15] ) );
				xyzw[3] = static_cast<T> ( s*  0.5 );
				s = static_cast<T> ( 0.5 / s );
				xyzw[0] = (m[9] - m[6])*  s;
				xyzw[1] = (m[2] - m[8])*  s;
				xyzw[2] = (m[4] - m[1])*  s;
			}
			else
			{
				int i = 0;
				if ( m[5] > m[0] ) i = 1;
				if ( m[10] > m[i*3+i] ) i = 2;
				const int nxt[3] = { 1, 2, 0 };
				int j = nxt[i];
				int k = nxt[j];
				T s = static_cast<T> ( sqrt( ( m[i*3+j] - ( m[j*3+j] + m[k*3+k] ) ) + 1.0 ) );
				xyzw[i] = static_cast<T> ( s*  0.5 );
				s = static_cast<T> ( 0.5 / s );
				xyzw[3] = ( m[j*3+k] - m[k*3+j] )*  s;
				xyzw[j] = ( m[i*3+j] + m[j*3+i] )*  s;
				xyzw[k] = ( m[i*3+k] + m[k*3+i] )*  s;
			}
		}

		/**
		 * Class constructor from rotaton axis and angle.
		 * @param	axis is rotation axis vector.
		 * @param	angle is rotation angle.
		 */
		template <class T> TurboQuaternion<T>::TurboQuaternion(const Vector3<T>& axis, T angle)
		{
			T sqnorm = dot3( axis, axis );

			if( sqnorm == static_cast<T> ( 0.0 ) )
			{
				xyzw[0] = static_cast<T> ( 0.0 );
				xyzw[1] = static_cast<T> ( 0.0 );
				xyzw[2] = static_cast<T> ( 0.0 );
				xyzw[3] = static_cast<T> ( 1.0 );
			}
			else
			{
				angle *= static_cast<T>( 0.5*  3.141592653589793 / 180.0 );
				T sinAngle = static_cast<T>( sin( angle ) );

				if ( sqnorm != static_cast<T>( 1.0 ) ) 
					sinAngle /= static_cast<T>( sqrt( sqnorm ) );

				xyzw[0] = sinAngle*  axis[0];
				xyzw[1] = sinAngle*  axis[1];
				xyzw[2] = sinAngle*  axis[2];
				xyzw[3] = static_cast<T>( cos( angle ) );
			}
		}

		/**
		 * Method is used to calculate Quaternion norm.
		 * @return	calculated quaternion norm == Vector4 length.
		 */
		template <class T> const T TurboQuaternion<T>::norm() const
		{
			return length();
		}

		/**
		 * Method is used to get rotation matrix from quaternion.
		 * @return	rotation matrix from quaternion.
		 */
		template <class T> const Matrix4<T> TurboQuaternion<T>::matrix4() const
		{
			T norm = dot4(* this,* this );
			T s = ( norm == static_cast<T> ( 0.0 ) ) ? static_cast<T> ( 0.0 ) : static_cast<T> ( 2.0 / norm );

			T xs = xyzw[0]*  s;
			T ys = xyzw[1]*  s;
			T zs = xyzw[2]*  s;
			T wx = xyzw[3]*  xs;
			T wy = xyzw[3]*  ys;
			T wz = xyzw[3]*  zs;
			T xx = xyzw[0]*  xs;
			T xy = xyzw[0]*  ys;
			T xz = xyzw[0]*  zs;
			T yy = xyzw[1]*  ys;
			T yz = xyzw[1]*  zs;
			T zz = xyzw[2]*  zs;

			return Matrix4<T> ( static_cast<T> ( 1.0 - (yy + zz) ),
									xy - wz,
									xz + wy,
									static_cast<T> ( 0.0 ),
									xy + wz,
									static_cast<T> ( 1.0 - (xx + zz) ),
									yz - wx,
									static_cast<T> ( 0.0 ),
									xz - wy,
									yz + wx,
									static_cast<T> ( 1.0 - (xx + yy) ),
									static_cast<T> ( 0.0 ),
									static_cast<T> ( 0.0 ),
									static_cast<T> ( 0.0 ),
									static_cast<T> ( 0.0 ),
									static_cast<T> ( 1.0 ) );
		}

		/**
		 * Method is used to get rotation matrix from quaternion.
		 * @return	rotation matrix from quaternion.
		 */
		template <class T> const Matrix3<T> TurboQuaternion<T>::matrix3() const
		{
			T norm = dot4(* this,* this );
			T s = ( norm == static_cast<T> ( 0.0 ) ) ? static_cast<T> ( 0.0 ) : static_cast<T> ( 2.0 / norm );

			T xs = xyzw[0]*  s;
			T ys = xyzw[1]*  s;
			T zs = xyzw[2]*  s;
			T wx = xyzw[3]*  xs;
			T wy = xyzw[3]*  ys;
			T wz = xyzw[3]*  zs;
			T xx = xyzw[0]*  xs;
			T xy = xyzw[0]*  ys;
			T xz = xyzw[0]*  zs;
			T yy = xyzw[1]*  ys;
			T yz = xyzw[1]*  zs;
			T zz = xyzw[2]*  zs;

			return Matrix3<T> ( static_cast<T> ( 1.0 - (yy + zz) ),
									xy - wz,
									xz + wy,
									xy + wz,
									static_cast<T> ( 1.0 - (xx + zz) ),
									yz - wx,
									xz - wy,
									yz + wx,
									static_cast<T> ( 1.0 - (xx + yy) ) );
		}
	
		/**
		 * Method is used to conjugate quaternion.
		 * @return	conjugated quaternion.
		 */
		template <class T> TurboQuaternion<T> TurboQuaternion<T>::conjugate()
		{
			xyzw[0] = -xyzw[0];
			xyzw[1] = -xyzw[1];
			xyzw[2] = -xyzw[2];
			return* this;
		}

		/**
		 * Method is used to inverse quaternion.
		 * @return	inverted quaternion.
		 */
		template <class T> TurboQuaternion<T> TurboQuaternion<T>::inverse()
		{
			return conjugate();
		}

		/**
		 * Method is used to get rotation axis vector and rotation angle from quaternion.
		 * @param	axis is rotationaxis vector.
		 * @param	angle is rotation agle value.
		 */
		template <class T> void TurboQuaternion<T>::getAxisAngle(Vector3<T>& axis, T& angle) const
		{
			angle = static_cast<T> ( 180.0 /3.141592653589793*  acos( xyzw[3] )*  2.0 );

			if ( angle == 0.0 )
				axis = Vector3<T>( static_cast<T>( 0.0 ), static_cast<T>( 0.0 ), static_cast<T>( 1.0 ) );
			else
			{
				axis[0] = xyzw[0];
				axis[1] = xyzw[1];
				axis[2] = xyzw[2];
				axis.normalize();
			}
		}

		/**
		 * Method is used to interpolate between quaternions about defined angle.
		 * @param	p is first quaternion.
		 * @param	q is second quaternion.
		 * @param	alpha is nterpolation angle.
		 * @return	interpolated quaternion.
		 */
		template <class T> TurboQuaternion<T> slerp(const TurboQuaternion<T>&  p, const TurboQuaternion<T>&  q, T alpha)
		{
			T cosOmega = p[0]*  q[0] + p[1]*  q[1] + p[2]*  q[2] + p[3]*  q[3];
			bool bflip = cosOmega < static_cast<T>( 0.0 );
			if ( bflip ) cosOmega = -cosOmega;

			T beta = static_cast<T>( 1.0 ) - alpha;
			if( cosOmega >= static_cast<T>( 1.0 ) ) return p;
			T omega = static_cast<T>( acos( cosOmega ) );
			T oneOverSinOmega = static_cast<T>( 1.0 / sin( omega ) );
			beta = static_cast<T>( sin( omega*  beta ) *  oneOverSinOmega );
			alpha = static_cast<T>( sin( omega*  alpha )*  oneOverSinOmega );
			if( bflip ) alpha = -alpha;

			TurboQuaternion<T> r;
			r[0] = beta*  p[0]+ alpha*  q[0];
			r[1] = beta*  p[1]+ alpha*  q[1];
			r[2] = beta*  p[2]+ alpha*  q[2];
			r[3] = beta*  p[3]+ alpha*  q[3];
			return r;
		}

		/**
		 * Class muliplication with assign operator.
		 * @param	q is source quaternion.
		 * @return	result quaternion.
		 */
		template <class T> TurboQuaternion<T>& TurboQuaternion<T>::operator*=(const TurboQuaternion<T>&  q)
		{
			*this = TurboQuaternion<T>( xyzw[3]*  q[0] + xyzw[0]*  q[3] + xyzw[1]*  q[2] - xyzw[2]*  q[1],
									xyzw[3]*  q[1] + xyzw[1]*  q[3] + xyzw[2]*  q[0] - xyzw[0]*  q[2],
									xyzw[3]*  q[2] + xyzw[2]*  q[3] + xyzw[0]*  q[1] - xyzw[1]*  q[0],
									xyzw[3]*  q[3] - xyzw[0]*  q[0] - xyzw[1]*  q[1] - xyzw[2]*  q[2] );
			return* this;
		}

		/**
		 * Method is used to conjugate quaternion.
		 * @param	q is source quaternion.
		 * @return	conjugated quaternion.
		 */
		template <class T> TurboQuaternion<T> conjugate(const TurboQuaternion<T>&  q)
		{
			TurboQuaternion<T> quat( q );
			quat.conjugate();
			return quat;
		}

		/**
		 * Method is used to inverse quaternion.
		 * @param	q is source quaternion.
		 * @return	inverted quaternion.
		 */
		template <class T> TurboQuaternion<T> inverse(const TurboQuaternion<T>&  q)
		{
			TurboQuaternion<T> quat( q );
			quat.inverse();
			return quat;
		}

		/**
		 * Class muliplication operator.
		 * @param	q1 is first quaternion.
		 * @param	q2 is second quaternion.
		 * @return	result quaternion.
		 */
		template <class T> TurboQuaternion<T> operator*(const TurboQuaternion<T>&  q1, const TurboQuaternion<T>&  q2)
		{
			TurboQuaternion<T> q( q1 ); 
			q* = q2; 
			return q; 
		}
	}
}
#endif
