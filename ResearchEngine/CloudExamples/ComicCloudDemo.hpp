/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef ComicCloudDemo_HPP
#define ComicCloudDemo_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

#include "CloudParticleEmiter.hpp"

namespace ResearchEngine
{
	class ComicCloudDemo : public EngineFramework<ComicCloudDemo>
	{
		friend EngineFramework<ComicCloudDemo>;

	private:
		RenderQueue renderQueue;
		Camera camera;
		AyumiEngine::AyumiResource::Shader skyShader;
		CW::CloudParticleEmiter* particleEmiter;
		CW::CloudParticleEmiter* particleEmiter2;
		CW::CloudParticleEmiter* particleEmiter3;
		CW::CloudParticleEmiter* particleEmiter4;
		CW::CloudParticleEmiter* particleEmiter5;

		AyumiEngine::AyumiResource::Texture cloud;
		AyumiEngine::AyumiResource::Texture cloudSimple;
		AyumiEngine::AyumiResource::Texture cloudBase;

		AyumiEngine::AyumiResource::Texture cloudTest;
		AyumiEngine::AyumiResource::Texture cloudTest2;
		AyumiEngine::AyumiResource::Texture cloudTest3;
		AyumiEngine::AyumiResource::Texture cloudTest4;


		AyumiEngine::AyumiResource::Texture cloudCartoon;
		int currentDemo;

		
		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeRendering();
	public:
		ComicCloudDemo();
		~ComicCloudDemo();
		
		void initializeDemo();
		void doSomeMagic();

		void renderHud();
	};
}
#endif
