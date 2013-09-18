/**
 *  File contains declaration and definition of Matrix classes.
 *  @file    Matrix.hpp
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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace AyumiEngine
{
	namespace AyumiMath
	{
		/**
		 * Class represents three dimmension quadratic matrix. It it used to create
		 * NormalMatrix and Tangent space TangentBinormalNormal matrix. Class is templated for 
		 * data type. Matrix is in row order, need to be transpose before transmitted to shader.
		 */
		template <class T = GLfloat>
		class Matrix3
		{
		protected:
			T m33[9];
		public:
			Matrix3();
			Matrix3(const T a11, const T a12, const T a13,
						const T a21, const T a22, const T a23,
						const T a31, const T a32, const T a33);
			Matrix3(const T* m);
			Matrix3(const Matrix3<T>& m);

			const T* data() const;
			T& operator[] (const unsigned int i);
			const T& operator[] (const unsigned int i) const;

			Matrix3<T>& operator+=(const Matrix3<T>& m);
			Matrix3<T>& operator-=(const Matrix3<T>& m);
			Matrix3<T>& operator*=(const Matrix3<T>& m);
			Matrix3<T>& operator*=(const T f);
			Matrix3<T>& operator/=(const T f);
			Matrix3<T> operator*(const Matrix3<T>& m);

			Matrix3<T> identity();
			Matrix3<T> transpose();
			Matrix3<T> inverse();
			const T det() const;
		};

		template <class T> Matrix3<T> transpose(const Matrix3<T>& m);
		template <class T> Matrix3<T> inverse(const Matrix3<T>& m);
		template <class T> const T det(const Matrix3<T>& m);
		template <class T> bool operator==(const Matrix3<T>& m1, const Matrix3<T>& m2);
		template <class T> bool operator!=(const Matrix3<T>& m1, const Matrix3<T>& m2);
		template <class T> Matrix3<T> operator+(const Matrix3<T>& m1, const Matrix3<T>& m2);
		template <class T> Matrix3<T> operator-(const Matrix3<T>& m1, const Matrix3<T>& m2);
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m1, const Matrix3<T>& m2);
		template <class T> Matrix3<T> operator*(const T* m1, const Matrix3<T>& m2);
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m1, const T* m2);
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m, const T f);
		template <class T> Matrix3<T> operator*(const T f, const Matrix3<T>& m);
		template <class T> Vector3<T> operator*(const Matrix3<T>& m, const Vector3<T>& v);


		/**
		 * Class represents fourth dimmension quadratic matrix. It it used to create View, World, ModelMatirx
		 * and control all operation in 3D space by using OpenGL API.Matrix is in row order, need to be transpose before transmitted to shader.
		 */
		template <class T = GLfloat>
		class Matrix4
		{
		protected:
			T m44[16];
		public:
			Matrix4();
			Matrix4(const T a11, const T a12, const T a13, const T a14,
						const T a21, const T a22, const T a23, const T a24,
						const T a31, const T a32, const T a33, const T a34,
						const T a41, const T a42, const T a43, const T a44);
			Matrix4(const T* m );
			Matrix4(const Matrix4<T>& m);

			const T *data() const;
			T& operator[] (const unsigned int i);
			const T& operator[] (const unsigned int i) const;

			Matrix4<T>& operator+=(const Matrix4<T>& m);
			Matrix4<T>& operator-=(const Matrix4<T>& m);
			Matrix4<T>& operator*=(const Matrix4<T>& m);
			Matrix4<T>& operator*=(const T f);
			Matrix4<T>& operator/=(const T f);
			Matrix4<T> operator*(const Matrix4<T>& m);

			Matrix4<T> identity();
			Matrix4<T> transpose();
			Matrix4<T> inverse();
			const T det() const;

			void Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
			void Ortho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
			void Perspective(GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far);
			void LoadIdentity();
			void Frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
			void LoadMatrixd(const GLdouble* m);
			void LoadMatrixf(const GLfloat* m);
			void MultMatrixd(const GLdouble* m);
			void MultMatrixf(const GLfloat* m);
			void LoadTransposeMatrixf(const GLfloat* m);
			void LoadTransposeMatrixd(const GLdouble* m);
			void MultTransposeMatrixf(const GLfloat* m);
			void MultTransposeMatrixd(const GLdouble* m);
			void LookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
						GLdouble centerx, GLdouble centery, GLdouble centerz,
						GLdouble upx, GLdouble upy, GLdouble upz);
			void Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
			void Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
			void Rotatef(GLfloat angle, const Vector3<T>& axis);
			void Scaled(GLdouble x, GLdouble y, GLdouble z);
			void Scalef(GLfloat x, GLfloat y, GLfloat z);
			void Scalef(const Vector3<T>& scale);
			void Translated(GLdouble x, GLdouble y, GLdouble z);
			void Translatef(GLfloat x, GLfloat y, GLfloat z);
			void Translatef(const Vector3<T>& translate);
			void PickMatrix(GLdouble x, GLdouble y, GLdouble deltax, GLdouble deltay, GLint viewport[4]);
			int Project(GLdouble objx, GLdouble objy, GLdouble objz,
							const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
							const GLint viewport[4], GLdouble* winx, GLdouble* winy, GLdouble* winz);
			int UnProject( GLdouble winx, GLdouble winy, GLdouble winz, 
							const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
							const GLint viewport[4], GLdouble* objx, GLdouble* objy, GLdouble* objz);
			int UnProject4( GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
							const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
							const GLint viewport[4], GLclampd near, GLclampd far,
							GLdouble* objx, GLdouble* objy, GLdouble* objz, GLdouble* objw);
		};


		template <class T> Matrix4<T> transpose(const Matrix4<T>& m);
		template <class T> Matrix4<T> inverse(const Matrix4<T>& m);
		template <class T> const T det(const Matrix4<T>& m);
		template <class T> bool operator==(const Matrix4<T>& m1, const Matrix4<T>& m2);
		template <class T> bool operator!=(const Matrix4<T>& m1, const Matrix4<T>& m2);
		template <class T> Matrix4<T> operator+(const Matrix4<T>& m1, const Matrix4<T>& m2);
		template <class T> Matrix4<T> operator-(const Matrix4<T>& m1, const Matrix4<T>& m2);
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m1, const Matrix4<T>& m2);
		template <class T> Matrix4<T> operator*(const T* m1, const Matrix4<T>& m2);
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m1, const T* m2);
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m, const T f);
		template <class T> Matrix4<T> operator*(const T f, const Matrix4<T>& m);
		template <class T> Vector4<T> operator*(const Matrix4<T>& m, const Vector4<T>& v);

		/**
		 * Class default constructor. All matrix values are initialized by 0.
		 */
		template <class T> Matrix3<T>::Matrix3()
		{
			m33[0] = static_cast<T>( 0.0 );
			m33[1] = static_cast<T>( 0.0 );
			m33[2] = static_cast<T>( 0.0 );
			m33[3] = static_cast<T>( 0.0 );
			m33[4] = static_cast<T>( 0.0 );
			m33[5] = static_cast<T>( 0.0 );
			m33[6] = static_cast<T>( 0.0 );
			m33[7] = static_cast<T>( 0.0 );
			m33[8] = static_cast<T>( 0.0 );
		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	a11 is 1-1 value.
		 * @param	a12 is 1-2 value.
		 * @param	a13 is 1-3 value.
		 * @param	a21 is 2-1 value.
		 * @param	a22 is 2-2 value.
		 * @param	a23 is 2-3 value.
		 * @param	a31 is 3-1 value.
		 * @param	a32 is 3-2 value.
		 * @param	a33 is 3-3 value.
		 */
		template <class T> Matrix3<T>::Matrix3(const T a11, const T a12, const T a13,
													const T a21, const T a22, const T a23,
													const T a31, const T a32, const T a33)
		{
			m33[0] = a11;
			m33[1] = a12;
			m33[2] = a13;
			m33[3] = a21;
			m33[4] = a22;
			m33[5] = a23;
			m33[6] = a31;
			m33[7] = a32;
			m33[8] = a33;
		}

		/**
		 * Class constructor with vector parameter.
		 * @param	m is pointer to Matrix3 class.
		 */
		template <class T> Matrix3<T>::Matrix3(const T* m)
		{
			m33[0] = m[0];
			m33[1] = m[1];
			m33[2] = m[2];
			m33[3] = m[3];
			m33[4] = m[4];
			m33[5] = m[5];
			m33[6] = m[6];
			m33[7] = m[7];
			m33[8] = m[8];
		}

		/**
		 * Class copy constructor.
		 * @param	m is const reference of source Matrix.
		 */
		template <class T> Matrix3<T>::Matrix3(const Matrix3<T>& m)
		{
			m33[0] = m[0];
			m33[1] = m[1];
			m33[2] = m[2];
			m33[3] = m[3];
			m33[4] = m[4];
			m33[5] = m[5];
			m33[6] = m[6];
			m33[7] = m[7];
			m33[8] = m[8];
		}

		/**
		 * Method is used to get pointer to matirx data array.
		 * @return	pointer to Matrix data array.
		 */
		template <class T> const T* Matrix3<T>::data() const
		{
			return m33;
		}

		/**
		 * Class index operator which is used to get indexed Matrix value.
		 * @param	i is index of data.
		 * @return	indexed value.
		 */
		template <class T> T& Matrix3<T>::operator[](const unsigned int i)
		{
			return m33[i];
		}

		/**
		 * Class index operator which is used to get constatnt indexed Matrix value.
		 * @param	i is index of data.
		 * @return	constant indexed value.
		 */
		template <class T> const T& Matrix3<T>::operator[](const unsigned int i) const
		{
			return m33[i];
		}

		/**
		 * Class addition with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& m)
		{
			m33[0] += m[0];
			m33[1] += m[1];
			m33[2] += m[2];
			m33[3] += m[3];
			m33[4] += m[4];
			m33[5] += m[5];
			m33[6] += m[6];
			m33[7] += m[7];
			m33[8] += m[8];
			return *this;
		}

		/**
		 * Class substraction with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& m)
		{
			m33[0] -= m[0];
			m33[1] -= m[1];
			m33[2] -= m[2];
			m33[3] -= m[3];
			m33[4] -= m[4];
			m33[5] -= m[5];
			m33[6] -= m[6];
			m33[7] -= m[7];
			m33[8] -= m[8];
			return *this;
		}

		/**
		 * Class multiplication with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T>& m)
		{
			Matrix3<T> matrix( m33[0] * m[0] + m33[1] * m[3] + m33[2] * m[6],
								m33[0] * m[1] + m33[1] * m[4] + m33[2] * m[7],
								m33[0] * m[2] + m33[1] * m[5] + m33[2] * m[8],
								m33[3] * m[0] + m33[4] * m[3] + m33[5] * m[6],
								m33[3] * m[1] + m33[4] * m[4] + m33[5] * m[7],
								m33[3] * m[2] + m33[4] * m[5] + m33[5] * m[8],
								m33[6] * m[0] + m33[7] * m[3] + m33[8] * m[6],
								m33[6] * m[1] + m33[7] * m[4] + m33[8] * m[7],
								m33[6] * m[2] + m33[7] * m[5] + m33[8] * m[8]);
			*this = matrix;
			return *this;
		}

		/**
		 * Class multiplication with assign operator. Multiply Matrix by constant scalar.
		 * @param	f is scalar.
		 * @return	result Matrix.
		 */
		template <class T> Matrix3<T>& Matrix3<T>::operator*=(const T f)
		{
			m33[0] *= f;
			m33[1] *= f;
			m33[2] *= f;
			m33[3] *= f;
			m33[4] *= f;
			m33[5] *= f;
			m33[6] *= f;
			m33[7] *= f;
			m33[8] *= f;
			return *this;
		}

		/**
		 * Class divide with assign operator. Divide Matrix by constant scalar.
		 * @param	f is scalar.
		 * @return	result Matrix.
		 */
		template <class T> Matrix3<T>& Matrix3<T>::operator/=(const T f)
		{
			m33[0] /= f;
			m33[1] /= f;
			m33[2] /= f;
			m33[3] /= f;
			m33[4] /= f;
			m33[5] /= f;
			m33[6] /= f;
			m33[7] /= f;
			m33[8] /= f;
			return *this;
		}

		/**
		 * Class equal operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	true if matrices are equal, false otherwise.
		 */
		template <class T> bool operator==(const Matrix3<T>& m1, const Matrix3<T>& m2)
		{
			return ( m1[0] == m2[0] ) && ( m1[1] == m2[1] ) && ( m1[2] == m2[2] ) &&
					( m1[3] == m2[3] ) && ( m1[4] == m2[4] ) && ( m1[5] == m2[5] ) &&
					( m1[6] == m2[6] ) && ( m1[7] == m2[7] ) && ( m1[8] == m2[8] );
		}

		/**
		 * Class not equal operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	true if matrices are not equal, false otherwise.
		 */
		template <class T> bool operator!=(const Matrix3<T>& m1, const Matrix3<T>& m2)
		{
			return ( m1[0] != m2[0] ) || ( m1[1] != m2[1] ) || ( m1[2] != m2[2] ) ||
					( m1[3] != m2[3] ) || ( m1[4] != m2[4] ) || ( m1[5] != m2[5] ) ||
					( m1[6] != m2[6] ) || ( m1[7] != m2[7] ) || ( m1[8] != m2[8] );
		}

		/**
		 * Class addition operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator+(const Matrix3<T>& m1, const Matrix3<T>& m2)
		{
			return Matrix3<T>( m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2],
								m1[3] + m2[3], m1[4] + m2[4], m1[5] + m2[5],
								m1[6] + m2[6], m1[7] + m2[7], m1[8] + m2[8] );
		}

		/**
		 * Class substraction operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator-(const Matrix3<T>& m1, const Matrix3<T>& m2)
		{
			return Matrix3<T>( m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2],
								m1[3] - m2[3], m1[4] - m2[4], m1[5] - m2[5],
								m1[6] - m2[6], m1[7] - m2[7], m1[8] - m2[8] );
		}

		/**
		 * Class multiplication operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m1, const Matrix3<T>& m2)
		{
			return Matrix3<T>( m1[0] * m2[0] + m1[1] * m2[3] + m1[2] * m2[6],
								m1[0] * m2[1] + m1[1] * m2[4] + m1[2] * m2[7],
								m1[0] * m2[2] + m1[1] * m2[5] + m1[2] * m2[8],
								m1[3] * m2[0] + m1[4] * m2[3] + m1[5] * m2[6],
								m1[3] * m2[1] + m1[4] * m2[4] + m1[5] * m2[7],
								m1[3] * m2[2] + m1[4] * m2[5] + m1[5] * m2[8],
								m1[6] * m2[0] + m1[7] * m2[3] + m1[8] * m2[6],
								m1[6] * m2[1] + m1[7] * m2[4] + m1[8] * m2[7],
								m1[6] * m2[2] + m1[7] * m2[5] + m1[8] * m2[8] );
		}

		/**
		 * Class multiplication operator. Multiply matrix created from array data by matrix.
		 * @param	m1 is array of T type data.
		 * @param	m2 is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator*(const T *m1, const Matrix3<T>& m2)
		{
			return Matrix3<T>(m1) * m2;
		}

		/**
		 * Class multiplication operator. Multiply matrix by matrix created from array data.
		 * @param	m1 is Matrix.
		 * @param	m2  is array of T type data.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m1, const T* m2)
		{
			return m1 * Matrix3<T>(m2);
		}


		/**
		 * Class multiplication operator. Multiply matrix by constant scalar.
		 * @param	m is Matrix.
		 * @param	f is scalar.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator*(const Matrix3<T>& m, const T f)
		{
			return m * f;
		}

		/**
		 * Class multiplication operator. Multiply constant scalar by matrix.
		 * @param	f is scalar.
		 * @param	m is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> operator*(const T f, const Matrix3<T>& m)
		{
			return f * m;
		}

		/**
		 * Class multiplication operator.
		 * @param	m is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& m)
		{
			return Matrix3<T>( m33[0] * m[0] + m33[1] * m[3] + m33[2] * m[6],
								m33[0] * m[1] + m33[1] * m[4] + m33[2] * m[7],
								m33[0] * m[2] + m33[1] * m[5] + m33[2] * m[8],
								m33[3] * m[0] + m33[4] * m[3] + m33[5] * m[6],
								m33[3] * m[1] + m33[4] * m[4] + m33[5] * m[7],
								m33[3] * m[2] + m33[4] * m[5] + m33[5] * m[8],
								m33[6] * m[0] + m33[7] * m[3] + m33[8] * m[6],
								m33[6] * m[1] + m33[7] * m[4] + m33[8] * m[7],
								m33[6] * m[2] + m33[7] * m[5] + m33[8] * m[8] );
		}

		/**
		 * Class multiplication operator. Multiply matrix by three dimmension vector.
		 * @param	m is Matrix.
		 * @param	v is Vector
		 * @return	result vector.
		 */
		template <class T> Vector3<T> operator * ( const Matrix3<T>& m, const Vector3<T>& v)
		{
			return Vector3<T>( m[0] * v[0] + m[1] * v[1] + m[2] * v[2],
								m[3] * v[0] + m[4] * v[1] + m[5] * v[2],
								m[6] * v[0] + m[7] * v[1] + m[8] * v[2] );
		}

		/**
		 * Method is used to create identity matrix.
		 * @return	identity matrix.
		 */
		template <class T> Matrix3<T> Matrix3<T>::identity()
		{
			m33[0] = static_cast<T>( 1.0 );
			m33[1] = static_cast<T>( 0.0 );
			m33[2] = static_cast<T>( 0.0 );
			m33[3] = static_cast<T>( 0.0 );
			m33[4] = static_cast<T>( 1.0 );
			m33[5] = static_cast<T>( 0.0 );
			m33[6] = static_cast<T>( 0.0 );
			m33[7] = static_cast<T>( 0.0 );
			m33[8] = static_cast<T>( 1.0 );
			return *this;
		}

		/**
		 * Method is used to transpose matrix.
		 * @return	transponed matrix.
		 */
		template <class T> Matrix3<T> Matrix3<T>::transpose()
		{
			Matrix3<T> matrix( m33[0], m33[3], m33[6],
								m33[1], m33[4], m33[7],
								m33[2], m33[5], m33[8] );
			*this = matrix;
			return *this;
		}

		/**
		 * Method is used to inverse matrix.
		 * @return	inverted matrix.
		 */
		template <class T> Matrix3<T> Matrix3<T>::inverse()
		{
			const T determinant = det();
    
			if( determinant == static_cast<T> ( 0.0 ) )
				identity();
			else
			{
				const T invDet = static_cast<T> ( 1.0 / determinant );
				T matrix[9];
				matrix[0] = (m33[4] * m33[8] - m33[5] * m33[7]) * invDet;
				matrix[1] = (m33[2] * m33[7] - m33[1] * m33[8]) * invDet;
				matrix[2] = (m33[1] * m33[5] - m33[2] * m33[4]) * invDet;
				matrix[3] = (m33[5] * m33[6] - m33[3] * m33[8]) * invDet;
				matrix[4] = (m33[0] * m33[8] - m33[2] * m33[6]) * invDet;
				matrix[5] = (m33[2] * m33[3] - m33[0] * m33[5]) * invDet;
				matrix[6] = (m33[3] * m33[7] - m33[4] * m33[6]) * invDet;
				matrix[7] = (m33[1] * m33[6] - m33[0] * m33[7]) * invDet;
				matrix[8] = (m33[0] * m33[4] - m33[1] * m33[3]) * invDet;
				*this = Matrix3<T>( matrix );
			}
			return *this;
		}

		/**
		 * Method is used to calculate matrix determinant.
		 * @return	calculated matrix determinant.
		 */
		template <class T> const T Matrix3<T>::det() const
		{
			return m33[0] * (m33[4] * m33[8] - m33[7] * m33[5]) +
					m33[3] * (m33[7] * m33[2] - m33[1] * m33[8]) +
					m33[6] * (m33[1] * m33[5] - m33[4] * m33[2]);
		}

		/**
		 * Method is used to transpose matrix.
		 * @param	m is source Matrix.
		 * @return	transponed matrix.
		 */
		template <class T> Matrix3<T> transpose(const Matrix3<T>& m)
		{
			Matrix3<T> matrix(m);
			return matrix.transpose();
		}

		/**
		 * Method is used to inverse matrix.
		 * @param	m is source Matrix.
		 * @return	inverted matrix.
		 */
		template <class T> Matrix3<T> inverse(const Matrix3<T>& m)
		{
			Matrix3<T> matrix(m);
			return matrix.inverse();
		}

		/**
		 * Method is used to calculate matrix determinant.
		 * @param	m is source Matrix.
		 * @return	calculated matrix determinant.
		 */
		template <class T> const T det(const Matrix3<T>& m)
		{
			return m.det();
		}

		/**
		 * Class default constructor. All matrix values are initialized by 0.
		 */
		template <class T> Matrix4<T>::Matrix4()
		{
			m44[0] = static_cast<T>( 0.0 );
			m44[1] = static_cast<T>( 0.0 );
			m44[2] = static_cast<T>( 0.0 );
			m44[3] = static_cast<T>( 0.0 );
			m44[4] = static_cast<T>( 0.0 );
			m44[5] = static_cast<T>( 0.0 );
			m44[6] = static_cast<T>( 0.0 );
			m44[7] = static_cast<T>( 0.0 );
			m44[8] = static_cast<T>( 0.0 );
			m44[9] = static_cast<T>( 0.0 );
			m44[10] = static_cast<T>( 0.0 );
			m44[11] = static_cast<T>( 0.0 );
			m44[12] = static_cast<T>( 0.0 );
			m44[13] = static_cast<T>( 0.0 );
			m44[14] = static_cast<T>( 0.0 );
			m44[15] = static_cast<T>( 0.0 );
		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	a11 is 1-1 value.
		 * @param	a12 is 1-2 value.
		 * @param	a13 is 1-3 value.
		 * @param	a14 is 1-4 value
		 * @param	a21 is 2-1 value.
		 * @param	a22 is 2-2 value.
		 * @param	a23 is 2-3 value.
		 * @param	a24 is 2-4 value
		 * @param	a31 is 3-1 value.
		 * @param	a32 is 3-2 value.
		 * @param	a33 is 3-3 value.
		 * @param	a34 is 3-4 value.
		 * @param	a41 is 4-1 value.
		 * @param	a42 is 4-2 value.
		 * @param	a43 is 4-3 value.
		 * @param	a44 is 4-4 value.
		 */
		template <class T> Matrix4<T>::Matrix4(const T a11, const T a12, const T a13, const T a14,
													const T a21, const T a22, const T a23, const T a24,
													const T a31, const T a32, const T a33, const T a34,
													const T a41, const T a42, const T a43, const T a44)
		{
			m44[0] = a11;
			m44[1] = a12;
			m44[2] = a13;
			m44[3] = a14;
			m44[4] = a21;
			m44[5] = a22;
			m44[6] = a23;
			m44[7] = a24;
			m44[8] = a31;
			m44[9] = a32;
			m44[10] = a33;
			m44[11] = a34;
			m44[12] = a41;
			m44[13] = a42;
			m44[14] = a43;
			m44[15] = a44;
		}

		/**
		 * Class constructor with vector parameter.
		 * @param	m is pointer to Matrix3 class.
		 */
		template <class T> Matrix4<T>::Matrix4(const T * m)
		{
			m44[0] = m[0];
			m44[1] = m[1];
			m44[2] = m[2];
			m44[3] = m[3];
			m44[4] = m[4];
			m44[5] = m[5];
			m44[6] = m[6];
			m44[7] = m[7];
			m44[8] = m[8];
			m44[9] = m[9];
			m44[10] = m[10];
			m44[11] = m[11];
			m44[12] = m[12];
			m44[13] = m[13];
			m44[14] = m[14];
			m44[15] = m[15];
		}

		/**
		 * Class copy constructor.
		 * @param	m is const reference of source Matrix.
		 */
		template <class T> Matrix4<T>::Matrix4(const Matrix4<T>& m)
		{
			m44[0] = m[0];
			m44[1] = m[1];
			m44[2] = m[2];
			m44[3] = m[3];
			m44[4] = m[4];
			m44[5] = m[5];
			m44[6] = m[6];
			m44[7] = m[7];
			m44[8] = m[8];
			m44[9] = m[9];
			m44[10] = m[10];
			m44[11] = m[11];
			m44[12] = m[12];
			m44[13] = m[13];
			m44[14] = m[14];
			m44[15] = m[15];
		}

		/**
		 * Method is used to get pointer to matirx data array.
		 * @return	pointer to Matrix data array.
		 */
		template <class T> const T *Matrix4<T>::data() const
		{
			return m44;
		}

		/**
		 * Class index operator which is used to get indexed Matrix value.
		 * @param	i is index of data.
		 * @return	indexed value.
		 */
		template <class T> T& Matrix4<T>::operator[] (const unsigned int i)
		{
			return m44[i];
		}

		/**
		 * Class index operator which is used to get constatnt indexed Matrix value.
		 * @param	i is index of data.
		 * @return	constant indexed value.
		 */
		template <class T> const T& Matrix4<T>::operator[] (const unsigned int i) const
		{
			return m44[i];
		}

		/**
		 * Class addition with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& m)
		{
			m44[0] += m[0];
			m44[1] += m[1];
			m44[2] += m[2];
			m44[3] += m[3];
			m44[4] += m[4];
			m44[5] += m[5];
			m44[6] += m[6];
			m44[7] += m[7];
			m44[8] += m[8];
			m44[9] += m[9];
			m44[10] += m[10];
			m44[11] += m[11];
			m44[12] += m[12];
			m44[13] += m[13];
			m44[14] += m[14];
			m44[15] += m[15];
			return *this;
		}

		/**
		 * Class substraction with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& m)
		{
			m44[0] -= m[0];
			m44[1] -= m[1];
			m44[2] -= m[2];
			m44[3] -= m[3];
			m44[4] -= m[4];
			m44[5] -= m[5];
			m44[6] -= m[6];
			m44[7] -= m[7];
			m44[8] -= m[8];
			m44[9] -= m[9];
			m44[10] -= m[10];
			m44[11] -= m[11];
			m44[12] -= m[12];
			m44[13] -= m[13];
			m44[14] -= m[14];
			m44[15] -= m[15];
			return *this;
		}

		/**
		 * Class multiplication with assign operator.
		 * @param	m is source Matrix.
		 * @return	result Matrix.
		 */
		template <class T> Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& m)
		{
			Matrix4<T> matrix( m44[0] * m[0] + m44[1] * m[4] + m44[2] * m[8] + m44[3] * m[12],
								m44[0] * m[1] + m44[1] * m[5] + m44[2] * m[9] + m44[3] * m[13],
								m44[0] * m[2] + m44[1] * m[6] + m44[2] * m[10] + m44[3] * m[14],
								m44[0] * m[3] + m44[1] * m[7] + m44[2] * m[11] + m44[3] * m[15],
								m44[4] * m[0] + m44[5] * m[4] + m44[6] * m[8] + m44[7] * m[12],
								m44[4] * m[1] + m44[5] * m[5] + m44[6] * m[9] + m44[7] * m[13],
								m44[4] * m[2] + m44[5] * m[6] + m44[6] * m[10] + m44[7] * m[14],
								m44[4] * m[3] + m44[5] * m[7] + m44[6] * m[11] + m44[7] * m[15],
								m44[8] * m[0] + m44[9] * m[4] + m44[10] * m[8] + m44[11] * m[12],
								m44[8] * m[1] + m44[9] * m[5] + m44[10] * m[9] + m44[11] * m[13],
								m44[8] * m[2] + m44[9] * m[6] + m44[10] * m[10] + m44[11] * m[14],
								m44[8] * m[3] + m44[9] * m[7] + m44[10] * m[11] + m44[11] * m[15],
								m44[12] * m[0] + m44[13] * m[4] + m44[14] * m[8] + m44[15] * m[12],
								m44[12] * m[1] + m44[13] * m[5] + m44[14] * m[9] + m44[15] * m[13],
								m44[12] * m[2] + m44[13] * m[6] + m44[14] * m[10] + m44[15] * m[14],
								m44[12] * m[3] + m44[13] * m[7] + m44[14] * m[11] + m44[15] * m[15] );
			*this = matrix;
			return *this;
		}

		/**
		 * Class multiplication with assign operator. Multiply Matrix by constant scalar.
		 * @param	f is scalar.
		 * @return	result Matrix.
		 */
		template <class T> Matrix4<T>& Matrix4<T>::operator*=(const T f)
		{
			m44[0] *= f;
			m44[1] *= f;
			m44[2] *= f;
			m44[3] *= f;
			m44[4] *= f;
			m44[5] *= f;
			m44[6] *= f;
			m44[7] *= f;
			m44[8] *= f;
			m44[9] *= f;
			m44[10] *= f;
			m44[11] *= f;
			m44[12] *= f;
			m44[13] *= f;
			m44[14] *= f;
			m44[15] *= f;
			return *this;
		}

		/**
		 * Class divide with assign operator. Divide Matrix by constant scalar.
		 * @param	f is scalar.
		 * @return	result Matrix.
		 */
		template <class T> Matrix4<T>& Matrix4<T>::operator/=(const T f)
		{
			m44[0] /= f;
			m44[1] /= f;
			m44[2] /= f;
			m44[3] /= f;
			m44[4] /= f;
			m44[5] /= f;
			m44[6] /= f;
			m44[7] /= f;
			m44[8] /= f;
			m44[9] /= f;
			m44[10] /= f;
			m44[11] /= f;
			m44[12] /= f;
			m44[13] /= f;
			m44[14] /= f;
			m44[15] /= f;
			return *this;
		}

		/**
		 * Class equal operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	true if matrices are equal, false otherwise.
		 */
		template <class T> bool operator==(const Matrix4<T>& m1, const Matrix4<T>& m2)
		{
			return ( m1[0] == m2[0] ) && ( m1[1] == m2[1] ) && ( m1[2] == m2[2] ) &&
					( m1[3] == m2[3] ) && ( m1[4] == m2[4] ) && ( m1[5] == m2[5] ) &&
					( m1[6] == m2[6] ) && ( m1[7] == m2[7] ) && ( m1[8] == m2[8] ) &&
					( m1[9] == m2[9] ) && ( m1[10] == m2[10] ) && ( m1[11] == m2[11] ) &&
					( m1[12] == m2[12] )&& ( m1[13] == m2[13] ) && ( m1[14] == m2[14] ) &&
					( m1[15] == m2[15] );
		}

		/**
		 * Class not equal operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	true if matrices are not equal, false otherwise.
		 */
		template <class T> bool operator!=(const Matrix4<T>& m1, const Matrix4<T>& m2)
		{
			return ( m1[0] != m2[0] ) || ( m1[1] != m2[1] ) || ( m1[2] != m2[2] ) ||
					( m1[3] != m2[3] ) || ( m1[4] != m2[4] ) || ( m1[5] != m2[5] ) ||
					( m1[6] != m2[6] ) || ( m1[7] != m2[7] ) || ( m1[8] != m2[8] ) ||
					( m1[9] != m2[9] ) || ( m1[10] != m2[10] ) || ( m1[11] != m2[11] ) ||
					( m1[12] != m2[12] ) || ( m1[13] != m2[13] ) || ( m1[14] != m2[14] ) ||
					( m1[15] != m2[15] );
		}

		/**
		 * Class addition operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator+(const Matrix4<T>& m1, const Matrix4<T>& m2)
		{
			return Matrix4<T>( m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2],
								m1[3] + m2[3], m1[4] + m2[4], m1[5] + m2[5],
								m1[6] + m2[6], m1[7] + m2[7], m1[8] + m2[8],
								m1[9] + m2[9], m1[10] + m2[10], m1[11] + m2[11],
								m1[12] + m2[12], m1[13] + m2[13], m1[14] + m2[14],
								m1[15] + m2[15] );
		}

		/**
		 * Class substraction operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator-(const Matrix4<T>& m1, const Matrix4<T>& m2)
		{
			return Matrix4<T>( m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2],
								m1[3] - m2[3], m1[4] - m2[4], m1[5] - m2[5],
								m1[6] - m2[6], m1[7] - m2[7], m1[8] - m2[8],
								m1[9] - m2[9], m1[10] - m2[10], m1[11] - m2[11],
								m1[12] - m2[12], m1[13] - m2[13], m1[14] - m2[14],
								m1[15] - m2[15] );
		}

		/**
		 * Class multiplication operator.
		 * @param	m1 is first Matrix.
		 * @param	m2 is second Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m1, const Matrix4<T>& m2)
		{

			return Matrix4<T>( m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12],
								m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13],
								m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14],
								m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15],
								m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12],
								m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13],
								m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14],
								m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15],
								m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12],
								m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13],
								m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14],
								m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15],
								m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12],
								m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13],
								m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14],
								m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15] );
		}

		/**
		 * Class multiplication operator. Multiply matrix created from array data by matrix.
		 * @param	m1 is array of T type data.
		 * @param	m2 is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator*(const T* m1, const Matrix4<T>& m2)
		{
			return Matrix4<T>( m1 ) * m2;
		}

		/**
		 * Class multiplication operator. Multiply matrix by matrix created from array data.
		 * @param	m1 is Matrix.
		 * @param	m2  is array of T type data.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m1, const T* m2)
		{
			return m1 * Matrix4<T>( m2 );
		}

		/**
		 * Class multiplication operator. Multiply matrix by constant scalar.
		 * @param	m is Matrix.
		 * @param	f is scalar.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator*(const Matrix4<T>& m, const T f)
		{
			return m * f;
		}

		/**
		 * Class multiplication operator. Multiply constant scalar by matrix.
		 * @param	f is scalar.
		 * @param	m is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> operator*(const T f, const Matrix4<T>& m)
		{
			return f * m;
		}

		/**
		 * Class multiplication operator.
		 * @param	m is Matrix.
		 * @return	result matrix.
		 */
		template <class T> Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& m)
		{
			return Matrix4<T>( m44[0] * m[0] + m44[1] * m[4] + m44[2] * m[8] + m44[3] * m[12],
								m44[0] * m[1] + m44[1] * m[5] + m44[2] * m[9] + m44[3] * m[13],
								m44[0] * m[2] + m44[1] * m[6] + m44[2] * m[10] + m44[3] * m[14],
								m44[0] * m[3] + m44[1] * m[7] + m44[2] * m[11] + m44[3] * m[15],
								m44[4] * m[0] + m44[5] * m[4] + m44[6] * m[8] + m44[7] * m[12],
								m44[4] * m[1] + m44[5] * m[5] + m44[6] * m[9] + m44[7] * m[13],
								m44[4] * m[2] + m44[5] * m[6] + m44[6] * m[10] + m44[7] * m[14],
								m44[4] * m[3] + m44[5] * m[7] + m44[6] * m[11] + m44[7] * m[15],
								m44[8] * m[0] + m44[9] * m[4] + m44[10] * m[8] + m44[11] * m[12],
								m44[8] * m[1] + m44[9] * m[5] + m44[10] * m[9] + m44[11] * m[13],
								m44[8] * m[2] + m44[9] * m[6] + m44[10] * m[10] + m44[11] * m[14],
								m44[8] * m[3] + m44[9] * m[7] + m44[10] * m[11] + m44[11] * m[15],
								m44[12] * m[0] + m44[13] * m[4] + m44[14] * m[8] + m44[15] * m[12],
								m44[12] * m[1] + m44[13] * m[5] + m44[14] * m[9] + m44[15] * m[13],
								m44[12] * m[2] + m44[13] * m[6] + m44[14] * m[10] + m44[15] * m[14],
								m44[12] * m[3] + m44[13] * m[7] + m44[14] * m[11] + m44[15] * m[15] );
		}

		/**
		 * Class multiplication operator. Multiply matrix by four dimmension vector.
		 * @param	m is Matrix.
		 * @param	v is Vector
		 * @return	result vector.
		 */
		template <class T> Vector4<T> operator*(const Matrix4<T>& m, const Vector4<T>& v)
		{
			return Vector4<T>( m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3],
								m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7] * v[3],
								m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11] * v[3],
								m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3] );
		}

		/**
		 * Method is used to create identity matrix.
		 * @return	identity matrix.
		 */
		template <class T> Matrix4<T> Matrix4<T>::identity()
		{
			m44[0] = static_cast<T>( 1.0 );
			m44[1] = static_cast<T>( 0.0 );
			m44[2] = static_cast<T>( 0.0 );
			m44[3] = static_cast<T>( 0.0 );
			m44[4] = static_cast<T>( 0.0 );
			m44[5] = static_cast<T>( 1.0 );
			m44[6] = static_cast<T>( 0.0 );
			m44[7] = static_cast<T>( 0.0 );
			m44[8] = static_cast<T>( 0.0 );
			m44[9] = static_cast<T>( 0.0 );
			m44[10] = static_cast<T>( 1.0 );
			m44[11] = static_cast<T>( 0.0 );
			m44[12] = static_cast<T>( 0.0 );
			m44[13] = static_cast<T>( 0.0 );
			m44[14] = static_cast<T>( 0.0 );
			m44[15] = static_cast<T>( 1.0 );
			return *this;
		}

		/**
		 * Method is used to transpose matrix.
		 * @return	transponed matrix.
		 */
		template <class T> Matrix4<T> Matrix4<T>::transpose()
		{
			Matrix4<T> matrix( m44[0], m44[4], m44[8], m44[12],
								m44[1], m44[5], m44[9], m44[13],
								m44[2], m44[6], m44[10], m44[14],
								m44[3], m44[7], m44[11], m44[15] );
			*this = matrix;
			return *this;
		}

		/**
		 * Method is used to inverse matrix.
		 * @return	inverted matrix.
		 */
		template <class T> Matrix4<T> Matrix4<T>::inverse()
		{
			const T determinant = det();

			if( determinant == static_cast<T> ( 0.0 ) )
				identity();
			else
			{
				T matrix[16];
				const T invDet = static_cast<T> ( 1.0 / determinant );
				T d12 = m44[8] * m44[13] - m44[12] * m44[9];
				T d13 = m44[8] * m44[14] - m44[12] * m44[10];
				T d23 = m44[9] * m44[14] - m44[13] * m44[10];
				T d24 = m44[9] * m44[15] - m44[13] * m44[11];
				T d34 = m44[10] * m44[15] - m44[14] * m44[11];
				T d41 = m44[11] * m44[12] - m44[15] * m44[8];

				matrix[0] =  (m44[5] * d34 - m44[6] * d24 + m44[7] * d23) * invDet;
				matrix[4] = -(m44[4] * d34 + m44[6] * d41 + m44[7] * d13) * invDet;
				matrix[8] =  (m44[4] * d24 + m44[5] * d41 + m44[7] * d12) * invDet;
				matrix[12] = -(m44[4] * d23 - m44[5] * d13 + m44[6] * d12) * invDet;
				matrix[1] = -(m44[1] * d34 - m44[2] * d24 + m44[3] * d23) * invDet;
				matrix[5] =  (m44[0] * d34 + m44[2] * d41 + m44[3] * d13) * invDet;
				matrix[9] = -(m44[0] * d24 + m44[1] * d41 + m44[3] * d12) * invDet;
				matrix[13] =  (m44[0] * d23 - m44[1] * d13 + m44[2] * d12) * invDet;

				d12 = m44[0] * m44[5] - m44[4] * m44[1];
				d13 = m44[0] * m44[6] - m44[4] * m44[2];
				d23 = m44[1] * m44[6] - m44[5] * m44[2];
				d24 = m44[1] * m44[7] - m44[5] * m44[3];
				d34 = m44[2] * m44[7] - m44[6] * m44[3];
				d41 = m44[0] * m44[4] - m44[7] * m44[0];

				matrix[2]  =  (m44[13] * d34 - m44[14] * d24 + m44[15] * d23) * invDet;
				matrix[6]  = -(m44[12] * d34 + m44[14] * d41 + m44[15] * d13) * invDet;
				matrix[10] =  (m44[12] * d24 + m44[13] * d41 + m44[15] * d12) * invDet;
				matrix[14] = -(m44[12] * d23 - m44[13] * d13 + m44[14] * d12) * invDet;
				matrix[3] = -(m44[9] * d34 - m44[10] * d24 + m44[11] * d23) * invDet;
				matrix[7] =  (m44[8] * d34 + m44[10] * d41 + m44[11] * d13) * invDet;
				matrix[11] = -(m44[8] * d24 + m44[9] * d41 + m44[11] * d12) * invDet;
				matrix[15] =  (m44[8] * d23 - m44[9] * d13 + m44[10] * d12) * invDet;
				*this = Matrix4<T>( matrix );
			}
			return *this;
		}

		/**
		 * Method is used to calculate matrix determinant.
		 * @return	calculated matrix determinant.
		 */
		template <class T> const T Matrix4<T>::det() const
		{
			const T d12 = m44[8] * m44[13] - m44[12] * m44[9];
			const T d13 = m44[8] * m44[14] - m44[12] * m44[10];
			const T d23 = m44[9] * m44[14] - m44[13] * m44[10];
			const T d24 = m44[9] * m44[15] - m44[13] * m44[11];
			const T d34 = m44[10] * m44[15] - m44[14] * m44[11];
			const T d41 = m44[11] * m44[12] - m44[15] * m44[8];
			return m44[0] * (m44[5] * d34 - m44[6] * d24 + m44[7] * d23) - 
					m44[1] * (m44[4] * d34 + m44[6] * d41 + m44[7] * d13) + 
					m44[2] * (m44[4] * d24 + m44[5] * d41 + m44[7] * d12) -
					m44[3] * (m44[4] * d23 - m44[5] * d13 + m44[6] * d12);
		}

		/**
		 * Method is used to transpose matrix.
		 * @param	m is source Matrix.
		 * @return	transponed matrix.
		 */
		template <class T> Matrix4<T> transpose(const Matrix4<T>& m)
		{
			Matrix4<T> matrix(m);
			return matrix.transpose();
		}

		/**
		 * Method is used to inverse matrix.
		 * @param	m is source Matrix.
		 * @return	inverted matrix.
		 */
		template <class T> Matrix4<T> inverse(const Matrix4<T>& m)
		{
			Matrix4<T> matrix(m);
			return matrix.inverse();
		}

		/**
		 * Method is used to calculate matrix determinant.
		 * @param	m is source Matrix.
		 * @return	calculated matrix determinant.
		 */
		template <class T> const T det(const Matrix4<T>& m)
		{
			return m.det();
		}

		/**
		 * Method is used create Ortogonal 2D projection Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	left is projection left value.
		 * @param	right is projection right value.
		 * @param	bottom is projection bottom value.
		 * @param	top is projection top value.
		 */
		template <class T> void Matrix4<T>::Ortho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
		{
			Ortho( left, right, bottom, top, -1.0, 1.0 );
		}

		/**
		 * Method is used create Ortogonal 3D projection Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix.
		 * @param	left is projection left value.
		 * @param	right is projection right value.
		 * @param	bottom is projection bottom value.
		 * @param	top is projection top value.
		 * @param	near is projection near value.
		 * @param	far is projection far value.
		 */
		template <class T> void Matrix4<T>::Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
		{
			Matrix4<T> matrix ( static_cast<T>( 2.0 / (right - left) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -(right + left) / (right - left) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 2.0 / (top - bottom) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -(top + bottom) / (top - bottom) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -2.0 / (far - near) ),
								static_cast<T>( -(far + near) / (far - near) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used create Pespective 3D projection Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix.
		 * @param	fovy is projection fovy.
		 * @param	aspect is projection aspect.
		 * @param	near is projection near value.
		 * @param	far is projection far value.
		 */
		template <class T> void Matrix4<T>::Perspective( GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far )
		{
			T radians = static_cast<T>( fovy / 2.0 * 3.14159265358979323846 / 180.0 );
			T sine = static_cast<T>( sin( radians ) );
			if( (far == near) || (sine == static_cast<T>( 0.0 )) || (aspect == static_cast<T>( 0.0 )) ) return;
			T cotangent = static_cast<T>( cos( radians ) / sine );

			Matrix4<T> matrix( static_cast<T>( cotangent / aspect ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( cotangent ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -(far + near) / (far - near) ),
								static_cast<T>( -2.0 * near * far / (far - near) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -1.0 ),
								static_cast<T>( 0.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used create identity Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Current Matrix is changed to identity,
		 */
		template <class T> void Matrix4<T>::LoadIdentity()
		{
			identity();
		}

		/**
		 * Method is used create Frustrum Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix.
		 * @param	left is projection left value.
		 * @param	right is projection right value.
		 * @param	bottom is projection bottom value.
		 * @param	top is projection top value.
		 * @param	near is projection near value.
		 * @param	far is projection far value.
		 */
		template <class T> void Matrix4<T>::Frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
		{
			Matrix4<T> matrix ( static_cast<T>( 2.0 * near / (right - left) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( (right + left) / (right - left) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 2.0 * near / (top - bottom) ),
								static_cast<T>( (top + bottom) / (top - bottom) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -(far + near) / (far - near) ),
								static_cast<T>( -2.0 * far * near / (far - near) ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( -1.0 ),
								static_cast<T>( 0.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used create matrix from array of GLdouble data. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::LoadMatrixd(const GLdouble* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[1] ), static_cast<T>( m[2] ), static_cast<T>( m[3] ),
								static_cast<T>( m[4] ), static_cast<T>( m[5] ), static_cast<T>( m[6] ), static_cast<T>( m[7] ),
								static_cast<T>( m[8] ), static_cast<T>( m[9] ), static_cast<T>( m[10] ), static_cast<T>( m[11] ),
								static_cast<T>( m[12] ), static_cast<T>( m[13] ), static_cast<T>( m[14] ), static_cast<T>( m[15] ) );
			*this = matrix;
		}

		/**
		 * Method is used create matrix from array of GLfloat data. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::LoadMatrixf(const GLfloat* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[1] ), static_cast<T>( m[2] ), static_cast<T>( m[3] ),
								static_cast<T>( m[4] ), static_cast<T>( m[5] ), static_cast<T>( m[6] ), static_cast<T>( m[7] ),
								static_cast<T>( m[8] ), static_cast<T>( m[9] ), static_cast<T>( m[10] ), static_cast<T>( m[11] ),
								static_cast<T>( m[12] ), static_cast<T>( m[13] ), static_cast<T>( m[14] ), static_cast<T>( m[15] ) );
			*this = matrix;
		}

		/**
		 * Method is used create matrix from array of GLdouble data and multiply it by current Matrix.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::MultMatrixd(const GLdouble* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[1] ), static_cast<T>( m[2] ), static_cast<T>( m[3] ),
								static_cast<T>( m[4] ), static_cast<T>( m[5] ), static_cast<T>( m[6] ), static_cast<T>( m[7] ),
								static_cast<T>( m[8] ), static_cast<T>( m[9] ), static_cast<T>( m[10] ), static_cast<T>( m[11] ),
								static_cast<T>( m[12] ), static_cast<T>( m[13] ), static_cast<T>( m[14] ), static_cast<T>( m[15] ) );
			*this *= matrix;
		}

		/**
		 * Method is used create matrix from array of GLfloat data and multiply it by current Matrix.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::MultMatrixf(const GLfloat* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[1] ), static_cast<T>( m[2] ), static_cast<T>( m[3] ),
								static_cast<T>( m[4] ), static_cast<T>( m[5] ), static_cast<T>( m[6] ), static_cast<T>( m[7] ),
								static_cast<T>( m[8] ), static_cast<T>( m[9] ), static_cast<T>( m[10] ), static_cast<T>( m[11] ),
								static_cast<T>( m[12] ), static_cast<T>( m[13] ), static_cast<T>( m[14] ), static_cast<T>( m[15] ) );
			*this *= matrix;
		}

		/**
		 * Method is used create transposed matrix from array of GLfloat data.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::LoadTransposeMatrixf(const GLfloat* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[4] ), static_cast<T>( m[8] ), static_cast<T>( m[12] ),
								static_cast<T>( m[1] ), static_cast<T>( m[5] ), static_cast<T>( m[9] ), static_cast<T>( m[13] ),
								static_cast<T>( m[2] ), static_cast<T>( m[6] ), static_cast<T>( m[10] ), static_cast<T>( m[14] ),
								static_cast<T>( m[3] ), static_cast<T>( m[7] ), static_cast<T>( m[11] ), static_cast<T>( m[15] ) );
			*this = matrix;
		}

		/**
		 * Method is used create transposed matrix from array of GLdouble data.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::LoadTransposeMatrixd(const GLdouble* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[4] ), static_cast<T>( m[8] ), static_cast<T>( m[12] ),
								static_cast<T>( m[1] ), static_cast<T>( m[5] ), static_cast<T>( m[9] ), static_cast<T>( m[13] ),
								static_cast<T>( m[2] ), static_cast<T>( m[6] ), static_cast<T>( m[10] ), static_cast<T>( m[14] ),
								static_cast<T>( m[3] ), static_cast<T>( m[7] ), static_cast<T>( m[11] ), static_cast<T>( m[15] ) );
			*this = matrix;
		}

		/**
		 * Method is used create transposed matrix from array of GLfloat data and multiply by current Matrix.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::MultTransposeMatrixf(const GLfloat* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[4] ), static_cast<T>( m[8] ), static_cast<T>( m[12] ),
								static_cast<T>( m[1] ), static_cast<T>( m[5] ), static_cast<T>( m[9] ), static_cast<T>( m[13] ),
								static_cast<T>( m[2] ), static_cast<T>( m[6] ), static_cast<T>( m[10] ), static_cast<T>( m[14] ),
								static_cast<T>( m[3] ), static_cast<T>( m[7] ), static_cast<T>( m[11] ), static_cast<T>( m[15] ) );
			*this *= matrix;
		}

		/**
		 * Method is used create transposed matrix from array of GLdouble data and multiply by current Matrix.
		 * It is implementation of OpenGL 2.1 and GLU depracated API.
		 * @param	m is pointer to array data.
		 */
		template <class T> void Matrix4<T>::MultTransposeMatrixd(const GLdouble* m)
		{
			Matrix4<T> matrix( static_cast<T>( m[0] ), static_cast<T>( m[4] ), static_cast<T>( m[8] ), static_cast<T>( m[12] ),
								static_cast<T>( m[1] ), static_cast<T>( m[5] ), static_cast<T>( m[9] ), static_cast<T>( m[13] ),
								static_cast<T>( m[2] ), static_cast<T>( m[6] ), static_cast<T>( m[10] ), static_cast<T>( m[14] ),
								static_cast<T>( m[3] ), static_cast<T>( m[7] ), static_cast<T>( m[11] ), static_cast<T>( m[15] ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create projection matrix by using observer position vector, center point vector
		 * and up vector. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix.
		 * @param	eyex is position x value.
		 * @param	eyey is position y value.
		 * @param	eyez is position z value.
		 * @param	centerx is center point x value.
		 * @param	centery is center point y value.
		 * @param	centerz is center point z value.
		 * @param	upx is up vector x value.
		 * @param	upy is up vector y value.
		 * @param	upz is up vector z value.
		 */
		template <class T> void Matrix4<T>::LookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
														GLdouble centerx, GLdouble centery, GLdouble centerz,
														GLdouble upx, GLdouble upy, GLdouble upz)
		{
			T forward[3];
			forward[0] = static_cast<T>( centerx - eyex );
			forward[1] = static_cast<T>( centery - eyey );
			forward[2] = static_cast<T>( centerz - eyez );
			T d = static_cast<T>( sqrt( forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2] ) );
			if( d != static_cast<T>( 1.0 ) && d != static_cast<T>( 0.0 ) )
			{
				forward[0] /= d;
				forward[1] /= d;
				forward[2] /= d;
			}

			T up[3];
			up[0] = static_cast<T>( upx );
			up[1] = static_cast<T>( upy );
			up[2] = static_cast<T>( upz );

			T side[3];
			side[0] = forward[1]*up[2] - forward[2]*up[1];
			side[1] = forward[2]*up[0] - forward[0]*up[2];
			side[2] = forward[0]*up[1] - forward[1]*up[0];

			d = static_cast<T>( sqrt( side[0] * side[0] + side[1] * side[1] + side[2] * side[2] ) );
			if( d != static_cast<T>( 1.0 ) && d != static_cast<T>( 0.0 ) )
			{
				forward[0] /= d;
				forward[1] /= d;
				forward[2] /= d;
			}

			up[0] = side[1]*forward[2] - side[2]*forward[1];
			up[1] = side[2]*forward[0] - side[0]*forward[2];
			up[2] = side[0]*forward[1] - side[1]*forward[0];

			Matrix4<T> matrix( side[0], side[1], side[2], static_cast<T>( -eyex ),
								up[0], up[1], up[2], static_cast<T>( -eyey ),
								-forward[0], -forward[1], -forward[2], static_cast<T>( -eyez ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create rotation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLdouble values.
		 * @param	angle is rotation angle
		 * @param	x is rotation axis x value.
		 * @param	y is rotation axis y value.
		 * @param	z is rotation axis z value.
		 */
		template <class T> void Matrix4<T>::Rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
		{
			const T d = static_cast<T>( sqrt( x * x + y * y + z * z ) );
			if( d != 1.0 && d != 0.0 )
			{
				x /= static_cast<GLdouble>( d );
				y /= static_cast<GLdouble>( d );
				z /= static_cast<GLdouble>( d );
			}
			const T c = static_cast<T>( cos( angle * 3.141592653589793 / 180.0 ) );
			const T s = static_cast<T>( sin( angle * 3.141592653589793 / 180.0 ) );
			Matrix4<T> matrix( static_cast<T>( x * x * (1 - c) + c ),
								static_cast<T>( x * y * (1 - c) - z * s ),
								static_cast<T>( x * z * (1 - c) + y * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y * x * (1 - c) + z * s ),
								static_cast<T>( y * y * (1 - c) + c ),
								static_cast<T>( y * z * (1 - c) - x * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x * z * (1 - c) - y * s ),
								static_cast<T>( y * z * (1 - c) + x * s ),
								static_cast<T>( z * z * (1 - c) + c ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create rotation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLfloat values.
		 * @param	angle is rotation angle
		 * @param	x is rotation axis x value.
		 * @param	y is rotation axis y value.
		 * @param	z is rotation axis z value.
		 */
		template <class T> void Matrix4<T>::Rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
		{
			const T d = static_cast<T>( sqrt( x * x + y * y + z * z ) );
			if( d != 1.0 && d != 0.0 )
			{
				x /= static_cast<GLfloat>( d );
				y /= static_cast<GLfloat>( d );
				z /= static_cast<GLfloat>( d );
			}
			const T c = static_cast<T>( cos( angle * 3.141592653589793 / 180.0 ) );
			const T s = static_cast<T>( sin( angle * 3.141592653589793 / 180.0 ) );
			Matrix4<T> matrix( static_cast<T>( x * x * (1 - c) + c ),
								static_cast<T>( x * y * (1 - c) - z * s ),
								static_cast<T>( x * z * (1 - c) + y * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y * x * (1 - c) + z * s ),
								static_cast<T>( y * y * (1 - c) + c ),
								static_cast<T>( y * z * (1 - c) - x * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x * z * (1 - c) - y * s ),
								static_cast<T>( y * z * (1 - c) + x * s ),
								static_cast<T>( z * z * (1 - c) + c ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create rotation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use Vector3D<Glfloat> class.
		 * @param	angle is rotation angle.
		 * @param	axis is rotation x,y,z values.
		 */
		template <class T> void Matrix4<T>::Rotatef(GLfloat angle, const Vector3<T>& axis)
		{
			T x = axis[0];
			T y = axis[1];
			T z = axis[2];

			const T d = static_cast<T>( sqrt( x * x + y * y + z * z ) );
			if( d != 1.0 && d != 0.0 )
			{
				x /= static_cast<GLfloat>( d );
				y /= static_cast<GLfloat>( d );
				z /= static_cast<GLfloat>( d );
			}
			const T c = static_cast<T>( cos( angle * 3.141592653589793 / 180.0 ) );
			const T s = static_cast<T>( sin( angle * 3.141592653589793 / 180.0 ) );
			Matrix4<T> matrix( static_cast<T>( x * x * (1 - c) + c ),
								static_cast<T>( x * y * (1 - c) - z * s ),
								static_cast<T>( x * z * (1 - c) + y * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y * x * (1 - c) + z * s ),
								static_cast<T>( y * y * (1 - c) + c ),
								static_cast<T>( y * z * (1 - c) - x * s ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x * z * (1 - c) - y * s ),
								static_cast<T>( y * z * (1 - c) + x * s ),
								static_cast<T>( z * z * (1 - c) + c ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create scale transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLdouble values.
		 * @param	x is scale vector x value.
		 * @param	y is scale vector y value.
		 * @param	z is scale vector z value.
		 */
		template <class T> void Matrix4<T>::Scaled(GLdouble x, GLdouble y, GLdouble z)
		{
			Matrix4<T> matrix( static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create scale transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLfloat values.
		 * @param	x is scale vector x value.
		 * @param	y is scale vector y value.
		 * @param	z is scale vector z value.
		 */
		template <class T> void Matrix4<T>::Scalef(GLfloat x, GLfloat y, GLfloat z)
		{
			Matrix4<T> matrix( static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create scale transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use Vector3<T>.
		 * @param	scale is scale vector.
		 */
		template <class T> void Matrix4<T>::Scalef(const Vector3<T>& scale)
		{
			T x = scale[0];
			T y = scale[1];
			T z = scale[2];
			Matrix4<T> matrix( static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create transation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLdouble values.
		 * @param	x is translate vector x value.
		 * @param	y is translate vector y value.
		 * @param	z is translate vector z value.
		 */
		template <class T> void Matrix4<T>::Translated(GLdouble x, GLdouble y, GLdouble z)
		{
			Matrix4<T> matrix( static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create transation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use GLfloat values.
		 * @param	x is translate vector x value.
		 * @param	y is translate vector y value.
		 * @param	z is translate vector z value.
		 */
		template <class T> void Matrix4<T>::Translatef( GLfloat x, GLfloat y, GLfloat z )
		{
			Matrix4<T> matrix( static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to create transation transformation Matrix. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Result is multiplicated by current Matrix. Use Vector3<T>.
		 * @param	translate is translation vector
		 */
		template <class T> void Matrix4<T>::Translatef(const Vector3<T>& translate)
		{
			T x = translate[0];
			T y = translate[1];
			T z = translate[2];

			Matrix4<T> matrix( static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( x ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( y ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ),
								static_cast<T>( z ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 0.0 ),
								static_cast<T>( 1.0 ) );
			*this *= matrix;
		}

		/**
		 * Method is used to limit clipping plane of rendering area. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Use GLU SGI implementation.
		 * @param	x is x value.
		 * @param	y is y value.
		 * @param	deltax is deltax value.
		 * @param	deltay is deltay value.
		 * @param	vieport is viewport data array.
		 */
		template <class T> void Matrix4<T>::PickMatrix(GLdouble x, GLdouble y, GLdouble deltax, GLdouble deltay, GLint viewport[4])
		{
			if( deltax <= 0 || deltay <= 0 ) return;

			Translated( (viewport[2] - 2.0 * (x - viewport[0])) / deltax, (viewport[3] - 2.0 * (y - viewport[1])) / deltay, 0.0 );
			Scaled( viewport[2] / deltax, viewport[3] / deltay, 1.0 );
		}

		/**
		 * Method is used to translate model-space coordinates to screen-space coordinates. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Use GLU SGI implementation.
		 * @param	objx is object x coord.
		 * @param	objy is object y coord.
		 * @param	objz is object z coord.
		 * @param	modelMatrix is object model Matrix.
		 * @param	projMatrix is scene projection matrix.
		 * @param	viewport is array of viewport data.
		 * @param	winx is pointer screen x coord.
		 * @param	winy is pointer screen y coord. 
		 * @param	winz is pointer screen z coord. 
		 */
		template <class T> int Matrix4<T>::Project( GLdouble objx, GLdouble objy, GLdouble objz,
														const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
														const GLint viewport[4], GLdouble* winx, GLdouble* winy, GLdouble* winz)
		{
			Vector4<T> in(objx, objy, objz, 1.0);

			Matrix4<T> finalMatrix = modelMatrix * projMatrix;
			finalMatrix.transpose();

			in = finalMatrix * in;

			if( in[3] == 0.0 ) return GL_FALSE;
			in[0] /= in[3];
			in[1] /= in[3];
			in[2] /= in[3];

			in[0] = in[0] * 0.5 + 0.5;
			in[1] = in[1] * 0.5 + 0.5;
			in[2] = in[2] * 0.5 + 0.5;

			in[0] = in[0] * viewport[2] + viewport[0];
			in[1] = in[1] * viewport[3] + viewport[1];
			*winx = in[0];
			*winy = in[1];
			*winz = in[2];
			return GL_TRUE;
		}

		/**
		 * Method is used to translate screen-space coordinates to model-space coordinates. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Use GLU SGI implementation.
		 * @param	winx is screen x coord.
		 * @param	winy is screen y coord. 
		 * @param	winz is screen z coord.
		 * @param	modelMatrix is object model Matrix.
		 * @param	projMatrix is scene projection matrix.
		 * @param	viewport is array of viewport data.
		 * @param	objx is pointer to object x coord.
		 * @param	objy is pointer to object y coord.
		 * @param	objz is pointer to object z coord.
		 */
		template <class T> int Matrix4<T>::UnProject(GLdouble winx, GLdouble winy, GLdouble winz,
														const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
														const GLint viewport[4], GLdouble* objx, GLdouble* objy, GLdouble* objz)
		{
			Matrix4<T> finalMatrix = modelMatrix * projMatrix;
			finalMatrix.transpose();
			finalMatrix.inverse();

			Vector4<T> in( static_cast<T>( winx ), static_cast<T>( winy ), static_cast<T>( winz ), static_cast<T>( 1.0 ) );

			in[0] = (in[0] - viewport[0]) / viewport[2];
			in[1] = (in[1] - viewport[1]) / viewport[3];

			in[0] = static_cast<T>( in[0] * 2.0 - 1.0 );
			in[1] = static_cast<T>( in[1] * 2.0 - 1.0 );
			in[2] = static_cast<T>( in[2] * 2.0 - 1.0 );

			Vector4<T> out = finalMatrix * in;
			if( out[3] == 0.0 ) return GL_FALSE;
			out[0] /= out[3];
			out[1] /= out[3];
			out[2] /= out[3];
			*objx = out[0];
			*objy = out[1];
			*objz = out[2];
			return GL_TRUE;
		}

		/**
		 * Method is used to translate screen-space coordinates to model-space coordinates. It is implementation of OpenGL 2.1 and GLU depracated API.
		 * Use GLU SGI implementation.
		 * @param	winx is screen x coord.
		 * @param	winy is screen y coord. 
		 * @param	winz is screen z coord.
		 * @param	clipw is screen clip w value.
		 * @param	modelMatrix is object model Matrix.
		 * @param	projMatrix is scene projection matrix.
		 * @param	viewport is array of viewport data.
		 * @param	near is clamp near value.
		 * @param	far i clamp far value.
		 * @param	objx is pointer to object x coord.
		 * @param	objy is pointer to object y coord.
		 * @param	objz is pointer to object z coord.
		 * @param	objw is pointer to object w coord.
		 */
		template <class T> int Matrix4<T>::UnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
															const Matrix4<T> modelMatrix, const Matrix4<T> projMatrix,
															const GLint viewport[4], GLclampd near, GLclampd far,
															GLdouble* objx, GLdouble* objy, GLdouble* objz, GLdouble* objw)
		{
			Matrix4<T> finalMatrix = modelMatrix * projMatrix;
			finalMatrix.transpose();
			finalMatrix.inverse();

			Vector4<T> in( static_cast<T>( winx ), static_cast<T>( winy ), static_cast<T>( winz ), static_cast<T>( clipw ) );

			in[0] = (in[0] - viewport[0]) / viewport[2];
			in[1] = (in[1] - viewport[1]) / viewport[3];
			in[2] = (in[2] - near) / (far - near);

			in[0] = static_cast<T>( in[0] * 2.0 - 1.0 );
			in[1] = static_cast<T>( in[1] * 2.0 - 1.0 );
			in[2] = static_cast<T>( in[2] * 2.0 - 1.0 );

			Vector4<T> out = finalMatrix * in;
			if( out[3] == 0.0 ) return GL_FALSE;
			*objx = out[0];
			*objy = out[1];
			*objz = out[2];
			*objw = out[3];
			return GL_TRUE;
		}
	}
}
#endif
