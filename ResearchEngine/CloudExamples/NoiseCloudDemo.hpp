/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef NOISECLOUDDEMO_HPP
#define NOISECLOUDDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

#include "../Engine/SceneEntity.hpp"


#include "CloudFactory.hpp"

namespace ResearchEngine
{
	class NoiseCloudDemo : public EngineFramework<NoiseCloudDemo>
	{
		friend EngineFramework<NoiseCloudDemo>;

	private:
		SceneEntity* skyGrid;
		Camera camera;
		RenderQueue renderQueue;
		int currentDemo;
		float timer;
		CW::CloudFactory cloudFactory;
		AyumiEngine::AyumiResource::Shader cloudShader;
		AyumiEngine::AyumiResource::Shader cloudShaderOverlay;

		AyumiEngine::AyumiResource::Texture classicCloudTexture;
		AyumiEngine::AyumiResource::Texture diffCloudTexture;
		AyumiEngine::AyumiResource::Texture expCloudTexture;

		AyumiEngine::AyumiResource::Texture classicAnimCloudTexture;
		AyumiEngine::AyumiResource::Texture diffAnimCloudTexture;
		AyumiEngine::AyumiResource::Texture expAnimCloudTexture;


		AyumiEngine::AyumiResource::Texture mask;


		CW::StaticNoiseCloud classicCloud; 
		CW::StaticNoiseCloud diffCloud; 
		CW::StaticNoiseCloud expCloud; 

		CW::DynamicNoiseCloud classicAnimCloud;
		CW::DynamicNoiseCloud diffAnimCloud;
		CW::DynamicNoiseCloud expAnimCloud;

		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeRendering();

		void renderSky();
		void renderHud();

	public:
		NoiseCloudDemo();
		~NoiseCloudDemo();


		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
