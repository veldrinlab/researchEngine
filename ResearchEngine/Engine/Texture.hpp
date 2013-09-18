/**
 * File contains declaration of Texture class.
 * @file    Texture.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-08-09
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <IL/il.h>
#include <iostream>
#include <string>

namespace AyumiEngine
{
	namespace AyumiResource
	{
		/**
		 * Class represents one of basic Engine resource - Texture resource. Texture is bacis
		 * element of all 3D engine and games, where it is used in geometry mesh mapping. In Engine,
		 * textures are used in Entity Materials, 2D Sprites, Canvas and post-process effects.
		 */
		struct Texture
		{
			GLuint texture;

			Texture();
			~Texture();

			void loadTexture(const std::string& path);
		};
	}
}
#endif
