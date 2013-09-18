/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef PREETHAMDEMO_HPP
#define PREETHAMDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "PreethamModel.hpp"

namespace ResearchEngine
{
	class PreethamDemo : public EngineFramework<PreethamDemo>
	{
		friend EngineFramework<PreethamDemo>;

	private:
		PreethamModel sky;
		Camera camera;
		AyumiEngine::AyumiResource::Shader* skyShader;
		AyumiEngine::AyumiResource::Shader* hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		RenderQueue renderQueue;

		float timeAccumulator;
		float exposure;
		bool timeUpdate;
		bool useHdr;
		bool updated;
		
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
		void renderGround();
		void renderHud();

	public:
		PreethamDemo();
		~PreethamDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
