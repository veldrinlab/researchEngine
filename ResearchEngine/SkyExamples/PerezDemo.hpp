/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef PEREZDEMO_HPP
#define PEREZDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "PerezModel.hpp"

namespace ResearchEngine
{
	class PerezDemo : public EngineFramework<PerezDemo>
	{
		friend EngineFramework<PerezDemo>;

	private:
		PerezModel sky;
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
		void updatePerezCoefficients();
		void updateZenithColor();
		void initializeRendering();

		void renderSky();
		void renderGround();
		void renderHud();

	public:
		PerezDemo();
		~PerezDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
