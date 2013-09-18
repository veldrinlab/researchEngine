#ifndef GRASSDEMO_HPP
#define GRASSDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../SkyExamples/PreethamModel.hpp"
#include "../Engine/SceneEntity.hpp"

namespace ResearchEngine
{
	class GrassDemo : public EngineFramework<GrassDemo>
	{
		friend EngineFramework<GrassDemo>;

	private:
		PreethamModel sky;
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
		bool updated;
		bool changed;

		
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

		////////////////////
		AyumiEngine::AyumiResource::Shader grassShader;
		AyumiEngine::AyumiResource::Texture grassTexture;
		AyumiEngine::AyumiResource::Texture decalTexture;
		AyumiEngine::AyumiResource::Texture noiseTexture;
		///////////////////

	public:
		GrassDemo();
		~GrassDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}

#endif
