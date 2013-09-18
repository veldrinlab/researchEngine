/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef CLOUDFACTORY_HPP
#define CLOUDFACTORY_HPP

#include "ProceduralTextureFactory.hpp"
#include "StaticNoiseCloud.hpp"
#include "DynamicNoiseCloud.hpp"

#include "CloudParticleEmiter.hpp"

//TODO add create method for particle emiter
namespace CW
{
	class CloudFactory
	{
	private:
		ProceduralTextureFactory textureFactory;
		float cover;
		float sharpness;

	public:
		CloudFactory();
		~CloudFactory();

		

		void createStaticNoiseCloud(StaticNoiseCloud* noiseCloud);
		void createDynamicNoiseCloud(DynamicNoiseCloud* noiseCloud, const int statesAmount);
		
		GLuint createCloudTexture(StaticNoiseCloud* noiseCloud, const int alphaThreshold);
		GLuint createCloudTexture(DynamicNoiseCloud* noiseCloud, const int begin, const int end, const float a, const int alphaThreshold);

		void configureExpParameters(const float cover, const float shapness);
		void configureNoise(const float persistance, const int octaveAmount, const int seed, const float zoom, const float initialFrequency, const float initialAmplitude);
	};
}
#endif
