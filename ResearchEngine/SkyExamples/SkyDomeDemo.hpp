/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef SKYDOMEDEMO_HPP
#define SKYDOMEDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

#include "../Engine/SceneEntity.hpp"

namespace ResearchEngine
{
	class SkyDomeDemo : public EngineFramework<SkyDomeDemo>
	{
		friend EngineFramework<SkyDomeDemo>;

	private:
		RenderQueue renderQueue;
		Camera camera;
		AyumiEngine::AyumiResource::Shader skyShader;
		AyumiEngine::AyumiResource::Shader hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		
		SceneEntity* skyDome;

		float exposure;
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
		SkyDomeDemo();
		~SkyDomeDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
