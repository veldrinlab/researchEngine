/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef PARTICLEEMITER_HPP
#define PARTICLEEMITER_HPP

#include <vector>

#include "Particle.hpp"

namespace CW
{
	typedef std::vector<Particle*> Particles;

	class ParticleEmiter
	{
	protected:
		Particles particles;
		int particleAmount;
		AyumiEngine::AyumiMath::Vector3D origin;

		ParticleEmiter(const int particleAmount, AyumiEngine::AyumiMath::Vector3D origin)
		{
			this->particleAmount = particleAmount;
			this->origin = origin;
		}

		virtual ~ParticleEmiter() {};

		virtual void initializeEmiter() = 0;
		virtual void updateEmiter(const float deltaTime) = 0;
		virtual void renderEmiter() = 0;
	};
}
#endif
