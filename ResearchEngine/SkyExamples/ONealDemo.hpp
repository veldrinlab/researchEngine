/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef ONEALDEMO_HPP
#define ONEALDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "ONealModel.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../Engine/SceneEntity.hpp"


namespace ResearchEngine
{
	class ONealDemo : public EngineFramework<ONealDemo>
	{
		friend EngineFramework<ONealDemo>;

	private:
		ONealModel sky;
		Camera camera;
		AyumiEngine::AyumiResource::Shader* skyShader;
		AyumiEngine::AyumiResource::Shader* hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		RenderQueue renderQueue;
		SceneEntity* skyGrid;
		
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
		void renderGround();
		void renderHud();

	public:
		ONealDemo();
		~ONealDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
