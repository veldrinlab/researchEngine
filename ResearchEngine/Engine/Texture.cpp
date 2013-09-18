/**
 * File contains declaration of Texture class.
 * @file    Texture.cpp
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

#include "Texture.hpp"

using namespace std;

namespace AyumiEngine
{
	namespace AyumiResource
	{
		/**
		 * Class default constructor.
		 */
		Texture::Texture()
		{
			texture = 0;
		}

		/**
		 * Class destructor, free allocated memory. Delete texture.
		 */
		Texture::~Texture()
		{
			glDeleteTextures(1,&texture);
		}

		/**
		 * Method is used to load texture from external file.
		 * @param	path is texture file path.
		 */
		void Texture::loadTexture(const string& path)
		{
			ILuint texID;
			ilGenImages(1,&texID);
			ilBindImage(texID);
			
			if(!ilLoadImage((const ILstring)path.c_str()))
				cerr << "Texture 2D " << path << " loading error occurred!" << endl;

			if(!ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE))
				cerr << "Texture 2D " << path << " converting error occurred!" << endl;

			glGenTextures(1,&texture);
			glBindTexture(GL_TEXTURE_2D,texture);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

			float maximumAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&maximumAnisotropy);
			glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,maximumAnisotropy);
			glTexImage2D(GL_TEXTURE_2D,0,ilGetInteger(IL_IMAGE_BPP),ilGetInteger(IL_IMAGE_WIDTH),
				  ilGetInteger(IL_IMAGE_HEIGHT),0,ilGetInteger(IL_IMAGE_FORMAT),GL_UNSIGNED_BYTE,ilGetData());
			
			ilDeleteImages(1,&texID);
		}
	}
}
