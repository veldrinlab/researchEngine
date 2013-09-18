/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef CLOUDPARTICLEEMITER_HPP
#define CLOUDPARTICLEEMITER_HPP

#include <vector>
#include <time.h>

#include "../Engine/ResearchEngine.hpp"

#include "ParticleEmiter.hpp"

namespace CW
{
	//TODO CW math types
	class CloudParticleEmiter : public ParticleEmiter
	{
	private:
		AyumiEngine::AyumiMath::Vector3D velocity;
		float spawnAccumulator;
		float elipseRadiusOX;
		float elipseRadiusOY;

		Particle* spawnParticle();

	public:
		//temporary
		AyumiEngine::AyumiResource::Texture textures[4];

		CloudParticleEmiter(const int particleAmount, AyumiEngine::AyumiMath::Vector3D origin);
		~CloudParticleEmiter();

		void initializeEmiter();
		void updateEmiter(const float deltaTime);
		void renderEmiter();
	};
}
#endif
