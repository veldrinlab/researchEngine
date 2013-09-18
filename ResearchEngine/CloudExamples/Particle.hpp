/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef CLOUDPARTICLE_HPP
#define CLOUDPARTICLE_HPP

#include "../Engine/Math/CommonMath.hpp"

namespace CW
{
	//TOODO change to CW math types
	struct Particle
	{
		AyumiEngine::AyumiMath::Vector3D position;
		AyumiEngine::AyumiMath::Vector3D velocity;
		AyumiEngine::AyumiMath::Vector3D rotation;
		AyumiEngine::AyumiMath::Vector3D scale;
		AyumiEngine::AyumiMath::Vector4D color;
		AyumiEngine::AyumiResource::Texture texture;
		float lifeTime;
		float initLifeTime;
		float shapeRandomVariable;
		bool infinity;
	};
}
#endif
