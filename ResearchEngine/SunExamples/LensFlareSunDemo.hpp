/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef LENSFLARESUNDEMO_HPP
#define LENSFLARESUNDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

namespace ResearchEngine
{
	class LensFlareSunDemo : public EngineFramework<LensFlareSunDemo>
	{
		friend EngineFramework<LensFlareSunDemo>;

	private:
		RenderQueue renderQueue;
		Camera camera;

		AyumiEngine::AyumiResource::Shader skyShader;
		AyumiEngine::AyumiResource::Texture sunTexture;

		

		AyumiEngine::AyumiResource::Texture bigGlow3;
		AyumiEngine::AyumiResource::Texture halo3;
		AyumiEngine::AyumiResource::Texture hardGlow2;
		AyumiEngine::AyumiResource::Texture streaks4;
		
		

		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeSky();
		void initializeRendering();

		void renderSun(float x, float y);
		void renderFlare();
		void renderSky();
		void renderHud();

	public:
		LensFlareSunDemo();
		~LensFlareSunDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
