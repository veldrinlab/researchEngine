/**
 * File contains declaration of Mesh class.
 * @file    Mesh.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-08-10
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <fstream>

#include "Math/CommonMath.hpp"

namespace AyumiEngine
{
	namespace AyumiResource
	{
		template<class T = float>
		struct Vertex
		{
			T x,y,z;
			T nx, ny, nz;
			T u, v;
			T tx,ty,tz,tw;
		};

		/**
		 * Class represents one of basic Engine resource - Mesh resource. Mesh is required in any
		 * kind of 2D/3D games, engines and tools. It is used to store geometry data: vertices, normals,
		 * tangents, texture uv coordinates and indices for faster rendering of triangles. Class store
		 * arrays of data, and also can calculate some necessary data like normals and tangents from
		 * vertices and indices. It is also used to calculate Bounding Volumes. Mesh resource store
		 * data in special Vertex<> structure.
		 */
		class Mesh
		{
		private:
			Vertex<>* vertices;
			unsigned int* indices;
			int verticesAmount;
			int trianglesAmount;
		public:
			Mesh();
			Mesh(const Mesh& resource);
			~Mesh();

			void loadMesh(const char* filePath);

			Vertex<>& operator[] (const unsigned int i);
			const Vertex<>& operator[] (const unsigned int i) const;

			void initializeVerticesArray();
			void initializeIndicesArray();
			void initializeDataArrays();
			void calculateNormals();
			void calculateTangent();

			Vertex<>* getVertices() const;
			unsigned int* getIndices() const;
			int getVerticesAmount() const;
			int getTrianglesAmount() const;
			
			void setIndex(const int arrayIndex, const unsigned int index);
			void setVerticesAmount(const int verticesAmount);
			void setTrianglesAmount(const int trianglesAmount);
		};
	}
}
#endif