/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef OCEANDEMO2_HPP
#define OCEANDEMO2_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../SkyExamples/PreethamModel.hpp"
#include "../Engine/EngineFramework.hpp"

#include "../Engine/Ocean.hpp"

namespace ResearchEngine
{
	class OceanDemo2 : public EngineFramework<OceanDemo2>
	{
		friend EngineFramework<OceanDemo2>;

	private:
		PreethamModel sky;
		Ocean* ocean;
		Camera camera;
		AyumiEngine::AyumiResource::Shader* skyShader;
		AyumiEngine::AyumiResource::Shader* hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		RenderQueue renderQueue;
		
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
		void updatePreethamCoefficients();
		void updateZenithColor();
		void initializeRendering();

		void renderSky();
		void renderOcean();

	public:
		OceanDemo2();
		~OceanDemo2();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
