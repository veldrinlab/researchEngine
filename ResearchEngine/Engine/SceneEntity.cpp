/**
 * File contains definition of SceneEntity structure.
 * @file    SceneEntity.cpp
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

#include "SceneEntity.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	SceneEntity::SceneEntity(const string& texturePath, const string& meshPath)
	{
		texture.loadTexture(texturePath);
		mesh = new Mesh();
		mesh->loadMesh(meshPath.c_str());
		mesh->calculateNormals();
		mesh->calculateTangent();

		buffer = new VertexBufferObject();
		buffer->initializeBufferObject(*mesh);
	}
	
	SceneEntity::~SceneEntity()
	{
		delete buffer;
		delete mesh;
		glDeleteTextures(1,&texture.texture);
	}

	void SceneEntity::render()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(Vertex<>),reinterpret_cast<const GLubyte *>(0) + 0);
		glNormalPointer(GL_FLOAT,sizeof(Vertex<>),reinterpret_cast<const GLubyte *>(0) + sizeof(float)*3);   //The starting point of normals, 12 bytes awa
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex<>),reinterpret_cast<const GLubyte *>(0) + sizeof(float)*6);   //The starting point of texcoords, 24 bytes away



		glDrawElements(GL_TRIANGLES,mesh->getTrianglesAmount()*3,GL_UNSIGNED_INT,NULL);
		

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}
}
