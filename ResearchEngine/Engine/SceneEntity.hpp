/**
 * File contains declaration of SceneEntity structure.
 * @file    SceneEntity.hpp
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

#ifndef SCENEENTITY_HPP
#define SCENEENTITY_HPP

#include <string>

#include "Mesh.hpp"
#include "VertexBufferObject.hpp"
#include "Texture.hpp"

namespace ResearchEngine
{
	struct SceneEntity 
	{
		AyumiEngine::AyumiResource::Mesh* mesh;
		AyumiEngine::AyumiResource::Texture texture;
		AyumiEngine::AyumiUtils::VertexBufferObject* buffer;	
		


		SceneEntity(const std::string& texturePath, const std::string& meshPath);
		~SceneEntity();

		void render();
	};
}

#endif
