/**
 * File contains definition of VertexBufferObject class.
 * @file    VertexBufferObject.cpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-08-05
 *
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "VertexBufferObject.hpp"

namespace AyumiEngine
{
	namespace AyumiUtils
	{
		/**
		 * Class default constructor, set object member default values.
		 */
		VertexBufferObject::VertexBufferObject()
		{
			vertexBuffer = 0;
			indicesBuffer = 0;
			vertexPosition = 0;
			normalPosition = 0;
			texCoordPosition = 0;
			tangentPosition = 0;
		}

		/**
		 * Class constructor with initialize parameters.
		 * @param	entityMesh is geometry data which will be stored in buffer.
		 */
		VertexBufferObject::VertexBufferObject(const AyumiResource::Mesh& entityMesh)
		{
			glGenBuffers(1,&vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER,entityMesh.getVerticesAmount()*12*sizeof(float),entityMesh.getVertices(),GL_STATIC_DRAW);

			glGenBuffers(1,&indicesBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indicesBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,entityMesh.getTrianglesAmount()*3*sizeof(unsigned int),entityMesh.getIndices(),GL_STATIC_DRAW);
		}

		/**
		 * Class copy constructor.
		 * @param	vertexBuffer is copy source object.
		 */
		VertexBufferObject::VertexBufferObject(const VertexBufferObject& vertexBuffer)
		{
			this->vertexBuffer = vertexBuffer.vertexBuffer;
			this->indicesBuffer = vertexBuffer.indicesBuffer;
			this->vertexPosition = vertexBuffer.vertexPosition;
			this->normalPosition = vertexBuffer.normalPosition;
			this->tangentPosition = vertexBuffer.tangentPosition;
			this->texCoordPosition = vertexBuffer.texCoordPosition;
		}

		/**
		 * Class destructor, free allocated memory. Delete generated vertex buffers.
		 */
		VertexBufferObject::~VertexBufferObject()
		{
			glDeleteBuffers(1,&vertexBuffer);
			glDeleteBuffers(1,&indicesBuffer);
		}

		/**
		 * Method is used to initialize buffer object vertex and indices buffers. It is used instead
		 * of constructor withi initialize parameters.
		 * @param	entityMesh is geometry data which will be stored in buffer.
		 */ 
		void VertexBufferObject::initializeBufferObject(const AyumiResource::Mesh& entityMesh)
		{
			glGenBuffers(1,&vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER,entityMesh.getVerticesAmount()*12*sizeof(float),entityMesh.getVertices(),GL_STATIC_DRAW);

			glGenBuffers(1,&indicesBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indicesBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,entityMesh.getTrianglesAmount()*3*sizeof(unsigned int),entityMesh.getIndices(),GL_STATIC_DRAW);
		}

		/**
		 * Method is used to bind object buffers - vertices and indices - and enable vertex attrib array of
		 * vertex, normal, texCoord and tanget positions.
		 */
		void VertexBufferObject::bindBufferObject()
		{
			glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indicesBuffer);
		}

		/**
		 * Method is used to unbind object buffers - vertices and indices - and disable vertex attrib array of
		 * vertex, normal, texCoord and tanget positions.
		 */
		void VertexBufferObject::unbindBufferObject()
		{

		}
	}
}
