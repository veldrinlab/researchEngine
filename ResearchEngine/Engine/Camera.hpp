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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>

#include "Math/CommonMath.hpp"

namespace ResearchEngine
{
	struct Camera
	{	
		AyumiEngine::AyumiMath::Vector3D position;
		AyumiEngine::AyumiMath::Vector3D rotation;
		float sensivity;

		void applyCamera()
		{
			glLoadIdentity();  
			glRotatef(rotation[0],1.0,0.0,0.0); 
			glRotatef(rotation[1],0.0,1.0,0.0); 
			glTranslated(-position[0],-position[1],-position[2]);
		}
	};
}
#endif
