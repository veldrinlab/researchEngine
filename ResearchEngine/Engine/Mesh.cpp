/**
 * File contains definition of Mesh class.
 * @file    Mesh.cpp
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

#include "Mesh.hpp"

using namespace std;
using namespace AyumiEngine::AyumiMath;

namespace AyumiEngine
{
	namespace AyumiResource
	{
		/**
		 * Class default constructor, set object member default values.
		 */
		Mesh::Mesh()
		{
			vertices = nullptr;
			indices = nullptr;
			verticesAmount = 0;
			trianglesAmount = 0;
		}

		/**
		 * Class copy constructor.
		 * @param	mesh is copy source object.
		 */
		Mesh::Mesh(const Mesh& mesh)
		{
			verticesAmount = mesh.verticesAmount;
			trianglesAmount = mesh.trianglesAmount;
			vertices = mesh.vertices;
			indices = mesh.indices;
		}

		/**
		 * Class destructor, free allocated memory.
		 */
		Mesh::~Mesh()
		{
			if(vertices != nullptr)
				delete[] vertices;
			if(indices != nullptr)
				delete[] indices;
		}

		/**
		 * Method is used to load mesh data from *vel file.
		 * @param	filePath is path to mesh data file.
		 */
		void Mesh::loadMesh(const char* filePath)
		{
			int integerData = 0;
			float floatData = 0.0f;
			unsigned int indexData = 0;

			ifstream meshFile;
			meshFile.open(filePath);
			
			if(meshFile.is_open())
			{
				meshFile >> integerData;
				setVerticesAmount(integerData);
				meshFile >> integerData;
				setTrianglesAmount(integerData);
				initializeDataArrays();
				
				for(int i = 0; i < getVerticesAmount(); ++i)
				{
					meshFile >> floatData;
					getVertices()[i].x = floatData;
					meshFile >> floatData;
					getVertices()[i].y = floatData;
					meshFile >> floatData;
					getVertices()[i].z = floatData;		
				}
				for(int i = 0; i < getVerticesAmount(); ++i)
				{
					meshFile >> floatData;
					getVertices()[i].nx = floatData;
					meshFile >> floatData;
					getVertices()[i].ny = floatData;
					meshFile >> floatData;
					getVertices()[i].nz = floatData;	
				}
				for(int i = 0; i < getVerticesAmount(); ++i)
				{
					meshFile >> floatData;
					getVertices()[i].u = floatData;
					meshFile >> floatData;
					getVertices()[i].v = floatData;
				}
				for(int i = 0; i < getTrianglesAmount()*3; ++i)
				{
					meshFile >> indexData;
					setIndex(i,indexData);
				}
				calculateTangent();
			}
			else
				std::cerr << "Vel Mesh file opening error occurred !" << std::endl;	
			
			meshFile.close();
		}

		/**
		 * Class index operator which is used to get constatnt indexed Mesh vertices array value.
		 * @param	i is index of data.
		 * @return	indexed value.
		 */
		Vertex<>& Mesh::operator[](const unsigned int i)
		{
			return vertices[i];
		}

		/**
		 * Class index operator which is used to get constatnt indexed Mesh vertices array value.
		 * @param	i is index of data.
		 * @return	constant indexed value.
		 */
		const Vertex<>& Mesh::operator[](const unsigned int i) const
		{
			return vertices[i];
		}

		/**
		 * Method is used to alocate memory for vertices data array.
		 */
		void Mesh::initializeVerticesArray()
		{
			vertices = new Vertex<>[verticesAmount];
		}

		/**
		 * Method is used to alocate memory for indices data array.
		 */
		void Mesh::initializeIndicesArray()
		{
			indices = new unsigned int[trianglesAmount*3];
		}

		/**
		 * Method is used to allocate memory for vertices and indices data arrays.
		 */
		void Mesh::initializeDataArrays()
		{
			initializeVerticesArray();
			initializeIndicesArray();
		}

		/**
		 * Method is used to calculate mesh vertices normals for light operations.
		 */
		void Mesh::calculateNormals()
		{
			int counter = 0;
			Vector3D vector1;
			Vector3D vector2;
			Vector3D normal;

			Vector3D* tempNormals = new Vector3D[trianglesAmount];

			// Per-face normal calculation
			for(int i = 0; i < (trianglesAmount*3); i = i+3)
			{
				int i1 = indices[i];
				int i2 = indices[i+1];
				int i3 = indices[i+2];

				Vertex<> v1 = vertices[i1];
				Vertex<> v2 = vertices[i2];
				Vertex<> v3 = vertices[i3];
			
				vector1 = Vector3D(v3.x-v1.x,v3.y-v1.y,v3.z-v1.z);
				vector2 = Vector3D(v2.x-v1.x,v2.y-v1.y,v2.z-v1.z);

				normal = cross3(vector1,vector2);
				tempNormals[counter++] = normal;
			}
			
			//Per-vertex normal calculation
			Vector3D sum;
			int shared = 0;
    
			for(int i = 0; i < verticesAmount; ++i)
			{
				int triangleCounter = 0;
				for(int j = 0; j < (trianglesAmount* 3); j = j + 3)
				{
					int i1 = indices[j];
					int i2 = indices[j+1];
					int i3 = indices[j+2];

					if( i1 == i || i2 == i || i3 == i)
					{
						sum += tempNormals[triangleCounter];
						shared++;
					}
					triangleCounter++;
				}

				Vector3D finalNormal = sum / float(-shared);
				finalNormal.normalize();

				vertices[i].nx = finalNormal.x();
				vertices[i].ny = finalNormal.y();
				vertices[i].nz = finalNormal.z();

				sum.set(0.0f,0.0f,0.0f);
				shared = 0;

			}

			delete [] tempNormals;     
		}

		/**
		 * Method is used to calculate mesh tangets for tangent space operations like bump-mapping.
		 */
		void Mesh::calculateTangent()
		{
			Vector3D* tan1 = new Vector3D[verticesAmount * 2];
			Vector3D* tan2 = tan1 + verticesAmount;

			for( int i = 0; i < (trianglesAmount * 3); i = i + 3)
			{
				int i1 = indices[i];
				int i2 = indices[i+1];
				int i3 = indices[i+2];

				Vertex<> v1 = vertices[i1];
				Vertex<> v2 = vertices[i2];
				Vertex<> v3 = vertices[i3];

				float x1 = v2.x - v1.x;
				float x2 = v3.x - v1.x;
				float y1 = v2.y - v1.y;
				float y2 = v3.y - v1.y;
				float z1 = v2.z - v1.z;
				float z2 = v3.z - v1.z;

				float s1 = v2.u - v1.u;
				float s2 = v3.u - v1.u;
				float t1 = v2.v - v1.v;
				float t2 = v3.v - v1.v;

				float r = 1.0f / (s1 * t2 - s2 * t1);
				Vector3D sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
						(t2 * z1 - t1 * z2) * r);
				Vector3D tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
						(s1 * z2 - s2 * z1) * r);
        
				tan1[i1] += sdir;
				tan1[i2] += sdir;
				tan1[i3] += sdir;
        
				tan2[i1] += tdir;
				tan2[i2] += tdir;
				tan2[i3] += tdir;
			}

			for (int i = 0; i < verticesAmount; i++)
			{
				Vector3D n = Vector3D(vertices[i].nx,vertices[i].ny,vertices[i].nz);
				Vector3D t = tan1[i];

				// Gram-Schmidt orthogonalize
				Vector3D tangent = (t - n * dot3(n,t)).normalize(); // x y z
				float w = (dot3(cross3(n,t),tan2[i]) < 0.0f) ? -1.0f : 1.0f;

				vertices[i].tx = tangent.x();
				vertices[i].ty = tangent.y();
				vertices[i].tz = tangent.z();
				vertices[i].tw = w;
			}

			delete[] tan1;
		}

		/**
		 * Accessor to mesh vertices private member.
		 * @return	mesh vertices array.
		 */
		Vertex<>* Mesh::getVertices() const
		{
			return vertices;
		}

		/**
		 * Accessor to mesh vertices amount private member.
		 * @return	mesh vertices amount.
		 */
		int Mesh::getVerticesAmount() const
		{
			return verticesAmount;
		}

		/**
		 * Accessor to mesh triangle amount private member.
		 * @return	mesh idnices amount.
		 */
		int Mesh::getTrianglesAmount() const
		{
			return trianglesAmount;
		}

		/**
		 * Accessor to mesh indices array private member.
		 * @return	pointer to mesh indieces array.
		 */
		unsigned int* Mesh::getIndices() const
		{
			return indices;
		}

		/**
		 * Setter of mesh indices array element.
		 * @param	arrayIndex is indieces array index.
		 * @param	index is new mesh triangle index.
		 */
		void Mesh::setIndex(const int arrayIndex, const unsigned int index)
		{
			indices[arrayIndex] = index;
		}

		/**
		 * Setter of mesh vertices amount private member.
		 * @param	verticesAmount is new amount of vertices.
		 */
		void Mesh::setVerticesAmount(const int verticesAmount)
		{
			this->verticesAmount = verticesAmount;
		}

		/**
		 * Setter of mesh triangles amount private member.
		 * @param	trianglesAmount is new amount of triangles.
		 */
		void Mesh::setTrianglesAmount(const int trianglesAmount)
		{
			this->trianglesAmount = trianglesAmount;
		}

	}
}
