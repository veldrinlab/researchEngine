/**
 *  File contains declaration and definition of Vector classes.
 *  @file    Vector.hpp
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

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <GL/glew.h>
#include <math.h>

namespace AyumiEngine
{
	namespace AyumiMath
	{
		/**
		 * Class represents templated three dimmension vector class used to physics calculation and Entity
		 * state representation like 
		 */
		template <class T = GLfloat>
		class Vector3
		{
		protected:
			T xyz[3];
		public:
			Vector3();
			Vector3(const T x, const T y, const T z);
			Vector3(const T* v) ;
			Vector3(const Vector3<T>& v);

			const T* data() const;
			T& operator[] (const unsigned int i);
			const T& operator[] (const unsigned int i) const;
			const T x() const;
			const T y() const;
			const T z() const;
			void set(const T x, const T y, const T z);
			void setX(const T x);
			void setY(const T y);
			void setZ(const T z);

			Vector3<T>& operator+=(const Vector3<T>& v);
			Vector3<T>& operator-=(const Vector3<T>& v);
			Vector3<T> operator-();
			Vector3<T>& operator*=(const T f);
			Vector3<T>& operator/=(const T f);
			Vector3<T> operator*(const T f);

			const T length() const;
			Vector3<T> normalize();
		};

		template <class T> Vector3<T> normalize(const Vector3<T>& v);
		template <class T> const T dot3(const Vector3<T>& v1, const Vector3<T>& v2);
		template <class T> Vector3<T> cross3(const Vector3<T>& v1, const Vector3<T>& v2 );
		template <class T> Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2 );
		template <class T> Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2 );
		template <class T> Vector3<T> operator*(const T f, const Vector3<T>& v );
		template <class T> Vector3<T> operator*(const Vector3<T>& v, const T f );
		template <class T> Vector3<T> operator/(const Vector3<T>& v, const T f );
		template <class T> bool operator==(const Vector3<T>& v1, const Vector3<T>& v2 );
		template <class T> bool operator!=(const Vector3<T>& v1, const Vector3<T>& v2 );

		/**
		 *  Class represents templated four dimmension vector class in unit space.
		 */
		template <class T = GLfloat>
		class Vector4
		{
		protected:
			T xyzw[4];
		public:
			Vector4();
			Vector4(const T x, const T y, const T z, const T w);
			Vector4(const T* v );
			Vector4(const Vector4<T>& v );

			const T* data() const;
			T& operator[] ( const unsigned int i );
			const T& operator[] (const unsigned int i ) const;
			const T x() const;
			const T y() const;
			const T z() const;
			const T w() const;
			void set(const T x, const T y, const T z, const T w);
			void setX(const T x);
			void setY(const T y);
			void setZ(const T z);
			void setW(const T w);

			Vector4<T>& operator+=(const Vector4<T>& v);
			Vector4<T>& operator-=(const Vector4<T>& v);
			Vector4<T> operator-();
			Vector4<T>& operator*=(const T f);
			Vector4<T>& operator/=(const T f);
			Vector4<T> operator*(const T f);

			const T length() const;
			Vector4<T> normalize();
			Vector4<T> normalize3();
		};

		template <class T> Vector4<T> normalize(const Vector4<T>& v );
		template <class T> Vector4<T> normalize3(const Vector4<T>& v );
		template <class T> const T dot3(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> const T dot4(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> Vector4<T> cross3(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> Vector4<T> operator+(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> Vector4<T> operator-(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> Vector4<T> operator*(const T f, const Vector4<T>& v );
		template <class T> Vector4<T> operator*(const Vector4<T>& v, const T f );
		template <class T> Vector4<T> operator/(const Vector4<T>& v, const T f );
		template <class T> bool operator==(const Vector4<T>& v1, const Vector4<T>& v2 );
		template <class T> bool operator!=(const Vector4<T>& v1, const Vector4<T>& v2 );

		/**
		 * Class default constructor.
		 */
		template <class T> Vector3<T>::Vector3()
		{
			xyz[0] = static_cast<T>(0.0);
			xyz[1] = static_cast<T>(0.0);
			xyz[2] = static_cast<T>(0.0);
		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	x is vector x value.
		 * @param	y is vector y value.
		 * @param	z is vector z value.
		 */
		template <class T> Vector3<T>::Vector3(const T x, const T y, const T z )
		{
			xyz[0] = x;
			xyz[1] = y;
			xyz[2] = z;
		}
		
		/**
		 * Class constructor with vector parameter.
		 * @param	v is pointer to array data.
		 */
		template <class T> Vector3<T>::Vector3(const T* v)
		{
			xyz[0] = v[0];
			xyz[1] = v[1];
			xyz[2] = v[2];
		}

		/**
		 * Class copy constructor.
		 * @param	v is const reference of source vector.
		 */
		template <class T> Vector3<T>::Vector3( const Vector3<T>& v )
		{
			xyz[0] = v[0];
			xyz[1] = v[1];
			xyz[2] = v[2];
		}

		/**
		 * Method is used to get pointer to vector data array.
		 * @return	pointer to Vector data array.
		 */
		template <class T> const T* Vector3<T>::data() const
		{
			return xyz;
		}

		/**
		 * Class index operator which is used to get indexed Vector value.
		 * @param	i is index of data.
		 * @return	indexed value.
		 */
		template <class T> T& Vector3<T>::operator[] (const unsigned int i)
		{
			return xyz[i];
		}

		/**
		 * Class index operator which is used to get constatnt indexed Vector value.
		 * @param	i is index of data.
		 * @return	constant indexed value.
		 */
		template <class T> const T& Vector3<T>::operator[] (const unsigned int i) const
		{
			return xyz[i];
		}

		/**
		 * Method is used to get Vector x value.
		 * @return	Vector x value.
		 */
		template <class T> const T Vector3<T>::x() const
		{
			return xyz[0];
		}

		/**
		 * Method is used to get Vector y value.
		 * @return	Vector y value.
		 */
		template <class T> const T Vector3<T>::y() const
		{
			return xyz[1];
		}

		/**
		 * Method is used to get Vector z value.
		 * @return	Vector z value.
		 */
		template <class T> const T Vector3<T>::z() const
		{
			return xyz[2];
		}

		/**
		 * Method is used set Vector values.
		 * @param	x is first value.
		 * @param	y is second value.
		 * @param	z is third value.
		 */
		template <class T> void Vector3<T>::set(const T x, const T y, const T z)
		{
			xyz[0] = x;
			xyz[1] = y;
			xyz[2] = z;
		}

		/**
		 * Method is used set Vector first value
		 * @param	x is first value.
		 */
		template <class T> void Vector3<T>::setX(const T x)
		{
			xyz[0] = x;
		}

		/**
		 * Method is used set Vector second value
		 * @param	y is second value.
		 */
		template <class T> void Vector3<T>::setY(const T y)
		{
			xyz[1] = y;
		}

		/**
		 * Method is used set Vector third value
		 * @param	z is third value.
		 */
		template <class T> void Vector3<T>::setZ(const T z)
		{
			xyz[2] = z;
		}

		/**
		 * Class multiplication operator. Multiply Vector by scalar, inverse parameters order.
		 * @param	f is scalar.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T> operator*(const T f, const Vector3<T>& v)
		{
			return Vector3<T>(v[0]*f, v[1]*f, v[2]*f );
		}

		/**
		 * Class multiplication operator. Multiply Vector by scalar, left to right parameters order.
		 * @param	v is source Vector.
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T> operator*(const Vector3<T>& v, const T f)
		{
			return Vector3<T>(v[0]*f, v[1]*f, v[2]*f);
		}

		/**
		 * Class multiplication with assign operator. Multiply Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T>& Vector3<T>::operator*=(const T f)
		{
			xyz[0] *= f;
			xyz[1] *= f;
			xyz[2] *= f;
			return* this;
		}

		/**
		 * Class divide with assign operator. Divide Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T>& Vector3<T>::operator/=(const T f)
		{
			xyz[0] /= f;
			xyz[1] /= f;
			xyz[2] /= f;
			return* this;
		}

		/**
		 * Class divide operator. Divide Vector by scalar. Left to right parameters order.
		 * @param	v is source Vector.
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T> operator/(const Vector3<T>& v, const T f)
		{
			return Vector3<T> (v[0] / f, v[1] / f, v[2] / f);
		}

		/**
		 * Class addition with assign operator.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v)
		{
			xyz[0] += v[0];
			xyz[1] += v[1];
			xyz[2] += v[2];
			return* this;
		}

		/**
		 * Class substraction with assign operator.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v)
		{
			xyz[0] -= v[0];
			xyz[1] -= v[1];
			xyz[2] -= v[2];
			return* this;
		}

		/**
		 * Class negation operator. 
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T> Vector3<T>::operator-()
		{
			xyz[0] = -xyz[0];
			xyz[1] = -xyz[1];
			xyz[2] = -xyz[2];
			return* this;
		}

		/**
		 * Class multiplication. Multiply Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector3<T> Vector3<T>::operator*(const T f)
		{
			return Vector3<T>(xyz[0]*= f, xyz[1]*= f, xyz[2]*= f);
		}

		/**
		 * Class addition operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	result vector.
		 */
		template <class T> Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return Vector3<T>(v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]);
		}

		/**
		 * Class substraction operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	result vector.
		 */
		template <class T> Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return Vector3<T>(v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]);
		}

		/**
		 * Class equal operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	true if vectors are equal, false otherwise.
		 */
		template <class T> bool operator==(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]);
		}

		/**
		 * Class not equal operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	true if vectors are not equal, false otherwise.
		 */
		template <class T> bool operator!=(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return (v1[0] != v2[0]) || (v1[1] != v2[1]) || (v1[2] != v2[2]);
		}

		/**
		 * Method is used to calculate Vector length.
		 * @return	calculated length.
		 */
		template <class T> const T Vector3<T>::length() const
		{
			return static_cast<T> (sqrt( xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]));
		}

		/**
		 * Method is used to normalize Vector.
		 * @return	normalized Vector.
		 */
		template <class T> Vector3<T> Vector3<T>::normalize()
		{
			T d = length();
			if(d)
			{
				xyz[0] /= d;
				xyz[1] /= d;
				xyz[2] /= d;
			}
			return* this;
		}

		/**
		 * Method is used to normalize Vector.
		 * @param	v is source Vector.
		 * @return	normalized Vector.
		 */
		template <class T> Vector3<T> normalize(const Vector3<T>& v)
		{
			Vector3<T> vector(v);
			return vector.normalize();
		}

		/**
		 * Method is used to calculate vectors dot produkt.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	calculated dot product.
		 */
		template <class T> const T dot3(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
		}

		/**
		 * Method is used to calculate vectors cross produkt.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	calculated dot product.
		 */
		template <class T> Vector3<T> cross3(const Vector3<T>& v1, const Vector3<T>& v2)
		{
			return Vector3<T>( v1[1]*v2[2] - v2[1]*v1[2],
								v2[0]*v1[2] - v1[0]*v2[2],
								v1[0]*v2[1] - v2[0]*v1[1] );
		}

		/**
		 * Class default constructor.
		 */
		template <class T> Vector4<T>::Vector4()
		{
			xyzw[0] = static_cast<T> ( 0.0 );
			xyzw[1] = static_cast<T> ( 0.0 );
			xyzw[2] = static_cast<T> ( 0.0 );
			xyzw[3] = static_cast<T> ( 0.0 );
		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	x is vector x value.
		 * @param	y is vector y value.
		 * @param	z is vector z value.
		 * @param	w is vector w value.
		 */
		template <class T> Vector4<T>::Vector4(const T x, const T y, const T z, const T w)
		{
			xyzw[0] = x;
			xyzw[1] = y;
			xyzw[2] = z;
			xyzw[3] = w;
		}

		/**
		 * Class constructor with vector parameter.
		 * @param	v is pointer to array data.
		 */
		template <class T> Vector4<T>::Vector4(const T* v)
		{
			xyzw[0] = v[0];
			xyzw[1] = v[1];
			xyzw[2] = v[2];
			xyzw[3] = v[3];
		}

		/**
		 * Class copy constructor.
		 * @param	v is const reference of source vector.
		 */
		template <class T> Vector4<T>::Vector4(const Vector4<T>& v)
		{
			xyzw[0] = v[0];
			xyzw[1] = v[1];
			xyzw[2] = v[2];
			xyzw[3] = v[3];
		}

		/**
		 * Method is used to get Vector pointer to vector data array.
		 * @return	pointer to Vector data array.
		 */
		template <class T> const T* Vector4<T>::data() const
		{
			return xyzw;
		}

		/**
		 * Class index operator which is used to get indexed Vector value.
		 * @param	i is index of data.
		 * @return	indexed value.
		 */
		template <class T> T& Vector4<T>::operator[] (const unsigned int i)
		{
			return xyzw[i];
		}

		/**
		 * Class index operator which is used to get constatnt indexed Vector value.
		 * @param	i is index of data.
		 * @return	constant indexed value.
		 */
		template <class T> const T& Vector4<T>::operator[] (const unsigned int i) const
		{
			return xyzw[i];
		}

		/**
		 * Method is used to get Vector x value.
		 * @return	Vector x value.
		 */
		template <class T> const T Vector4<T>::x() const
		{
			return xyzw[0];
		}

		/**
		 * Method is used to get Vector y value.
		 * @return	Vector y value.
		 */
		template <class T> const T Vector4<T>::y() const
		{
			return xyzw[1];
		}

		/**
		 * Method is used to get Vector z value.
		 * @return	Vector z value.
		 */
		template <class T> const T Vector4<T>::z() const
		{
			return xyzw[2];
		}

		/**
		 * Method is used to get Vector w value.
		 * @return	Vector w value.
		 */
		template <class T> const T Vector4<T>::w() const
		{
			return xyzw[3];
		}

		/**
		 * Method is used set Vector values.
		 * @param	x is first value.
		 * @param	y is second value.
		 * @param	z is third value.
		 * @param	w is fourth value.
		 */
		template <class T> void Vector4<T>::set(const T x, const T y, const T z, const T w)
		{
			xyzw[0] = x;
			xyzw[1] = y;
			xyzw[2] = z;
			xyzw[3] = w;
		}

		/**
		 * Method is used set Vector first value
		 * @param	x is first value.
		 */
		template <class T> void Vector4<T>::setX(const T x)
		{
			xyzw[0] = x;
		}

		/**
		 * Method is used set Vector second value
		 * @param	y is second value.
		 */
		template <class T> void Vector4<T>::setY(const T y)
		{
			xyzw[1] = y;
		}

		/**
		 * Method is used set Vector third value
		 * @param	z is third value.
		 */
		template <class T> void Vector4<T>::setZ(const T z)
		{
			xyzw[2] = z;
		}

		/**
		 * Method is used set Vector fourth value
		 * @param	w is fourth value.
		 */
		template <class T> void Vector4<T>::setW(const T w)
		{
			xyzw[3] = w;
		}

		/**
		 * Class multiplication operator. Multiply Vector by scalar, inverse parameters order.
		 * @param	f is scalar.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T> operator*(const T f, const Vector4<T>& v)
		{
			return Vector4<T>(v[0]*f, v[1]*f, v[2]*f, v[3]*f);
		}

		/**
		 * Class multiplication operator. Multiply Vector by scalar, left to right parameters order.
		 * @param	v is source Vector.
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T> operator*(const Vector4<T>& v, const T f)
		{
			return Vector4<T>(v[0]*f, v[1]*f, v[2]*f, v[3]*f);
		}

		/**
		 * Class multiplication with assign operator. Multiply Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T>& Vector4<T>::operator*=(const T f)
		{
			xyzw[0] *= f;
			xyzw[1] *= f;
			xyzw[2] *= f;
			xyzw[3] *= f;
			return* this;
		}

		/**
		 * Class divide with assign operator. Divide Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T>& Vector4<T>::operator/=(const T f)
		{
			xyzw[0] /= f;
			xyzw[1] /= f;
			xyzw[2] /= f;
			xyzw[3] /= f;
			return* this;
		}

		/**
		 * Class multiplication. Multiply Vector by scalar. 
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T> Vector4<T>::operator*(const T f)
		{
			return Vector4<T>(xyzw[0]*= f, xyzw[1]*= f, xyzw[2]*= f, xyzw[3]*= f);
		}

		/**
		 * Class divide operator. Divide Vector by scalar. Left to right parameters order.
		 * @param	v is source Vector.
		 * @param	f is scalar.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T> operator/(const Vector4<T>& v, const T f)
		{
			return Vector4<T>(v[0] / f, v[1] / f, v[2] / f, v[3] / f);
		}

		/**
		 * Class addition with assign operator.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& v)
		{
			xyzw[0] += v[0];
			xyzw[1] += v[1];
			xyzw[2] += v[2];
			xyzw[3] += v[3];
			return* this;
		}

		/**
		 * Class substraction with assign operator.
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& v)
		{
			xyzw[0] -= v[0];
			xyzw[1] -= v[1];
			xyzw[2] -= v[2];
			xyzw[3] -= v[3];
			return* this;
		}

		/**
		 * Class negation operator. 
		 * @param	v is source Vector.
		 * @return	result Vector.
		 */
		template <class T> Vector4<T> Vector4<T>::operator-()
		{
			xyzw[0] = -xyzw[0];
			xyzw[1] = -xyzw[1];
			xyzw[2] = -xyzw[2];
			xyzw[3] = -xyzw[3];
			return* this;
		}

		/**
		 * Class addition operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	result vector.
		 */
		template <class T> Vector4<T> operator+(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return Vector4<T>( v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2], v1[3]+v2[3] );
		}

		/**
		 * Class substraction operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	result vector.
		 */
		template <class T> Vector4<T> operator-(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return Vector4<T>( v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2], v1[3]-v2[3] );
		}

		/**
		 * Class equal operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	true if vectors are equal, false otherwise.
		 */
		template <class T> bool operator==(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return ( v1[0] == v2[0] ) && ( v1[1] == v2[1] ) && ( v1[2] == v2[2] ) && ( v1[3] == v2[3] );
		}

		/**
		 * Class not equal operator.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	true if vectors are not equal, false otherwise.
		 */
		template <class T> bool operator!=(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return ( v1[0] != v2[0] ) || ( v1[1] != v2[1] ) || ( v1[2] != v2[2] ) || ( v1[3] != v2[3] );
		}

		/**
		 * Method is used to calculate Vector length.
		 * @return	calculated length.
		 */
		template <class T> const T Vector4<T>::length() const
		{
			return static_cast<T> ( sqrt( xyzw[0]*xyzw[0] + xyzw[1]*xyzw[1] + xyzw[2]*xyzw[2] + xyzw[3]*xyzw[3] ) );
		}

		/**
		 * Method is used to normalize Vector.
		 * @return	normalized Vector.
		 */
		template <class T> Vector4<T> Vector4<T>::normalize()
		{
			T d = length();
			if(d)
			{
				xyzw[0] /= d;
				xyzw[1] /= d;
				xyzw[2] /= d;
				xyzw[3] /= d;
			}
			return* this;
		}

		/**
		 * Method is used to normalize Vector in three dimension - without w value.
		 * @return	normalized Vector.
		 */
		template <class T> Vector4<T> Vector4<T>::normalize3()
		{
			T d = static_cast<T> ( sqrt( xyzw[0]*xyzw[0] + xyzw[1]*xyzw[1] + xyzw[2]*xyzw[2] ) );
			if(d)
			{
				xyzw[0] /= d;
				xyzw[1] /= d;
				xyzw[2] /= d;
			}
			return* this;
		}

		/**
		 * Method is used to normalize Vector.
		 * @param	v is source Vector.
		 * @return	normalized Vector.
		 */
		template <class T> Vector4<T> normalize(const Vector4<T>& v)
		{
			Vector4<T> vec( v );
			return vec.normalize();
		}

		/**
		 * Method is used to normalize Vector in three dimension - without w value.
		 * @param	v is source Vector.
		 * @return	normalized Vector.
		 */
		template <class T> Vector4<T> normalize3(const Vector4<T>& v)
		{
			Vector4<T> vec( v );
			return vec.normalize3();
		}

		/**
		 * Method is used to calculate vectors dot3 produkt.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	calculated dot product.
		 */
		template <class T> const T dot3(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
		}

		/**
		 * Method is used to calculate vectors dot4 produkt.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	calculated dot product.
		 */
		template <class T> const T dot4( const Vector4<T>& v1, const Vector4<T>& v2 )
		{
			return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3];
		}

		/**
		 * Method is used to calculate vectors cross produkt, w value is 0.
		 * @param	v1 is first Vector.
		 * @param	v2 is second Vector.
		 * @return	calculated dot product.
		 */
		template <class T> Vector4<T> cross3(const Vector4<T>& v1, const Vector4<T>& v2)
		{
			return Vector4<T>( v1[1]*v2[2] - v2[1]*v1[2],
								v2[0]*v1[2] - v1[0]*v2[2],
								v1[0]*v2[1] - v2[0]*v1[1],
								static_cast<T>( 0.0 ) );
		}
	}
}
#endif
