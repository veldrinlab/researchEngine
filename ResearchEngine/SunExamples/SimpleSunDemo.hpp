/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef SIMPLESUNDEMO_HPP
#define SIMPLESUNDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

namespace ResearchEngine
{
	class SimpleSunDemo : public EngineFramework<SimpleSunDemo>
	{
		friend EngineFramework<SimpleSunDemo>;

	private:
		RenderQueue renderQueue;
		Camera camera;

		AyumiEngine::AyumiResource::Shader skyShader;
		AyumiEngine::AyumiResource::Texture sunTexture;
		AyumiEngine::AyumiResource::Texture sunFlareTexture;
		AyumiEngine::AyumiResource::Texture cloudTexture;
		float flareAngle;
		float flareScale;

		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeSky();
		void initializeRendering();

		void renderSun(float x, float y);
		void renderCloud(float x, float y);
		void renderSky();
		void renderHud();

	public:
		SimpleSunDemo();
		~SimpleSunDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
