/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef OCEANDEMO_HPP
#define OCEANDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../SkyExamples/ONealModel.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../Engine/SceneEntity.hpp"

#include "../Engine/Ocean.hpp"

namespace ResearchEngine
{
	class OceanDemo : public EngineFramework<OceanDemo>
	{
		friend EngineFramework<OceanDemo>;

	private:
		ONealModel sky;
		Camera camera;
		AyumiEngine::AyumiResource::Shader* skyShader;
		AyumiEngine::AyumiResource::Shader* hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		RenderQueue renderQueue;
		SceneEntity* skyGrid;
		
		//
		Ocean* ocean;

		float timeAccumulator;
		float exposure;
		bool timeUpdate;
		bool useHdr;
		
		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeSky();
		void initializeRendering();

		void renderSky();
		void renderOcean();

	public:
		OceanDemo();
		~OceanDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
