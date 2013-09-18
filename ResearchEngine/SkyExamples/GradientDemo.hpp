/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef GRADIENTDEMO_HPP
#define GRADIENTDEMO_HPP

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../Engine/SceneEntity.hpp"

namespace ResearchEngine
{
	class GradientDemo : public EngineFramework<GradientDemo>
	{
		friend EngineFramework<GradientDemo>;

	private:
		Camera camera;
		std::vector<AyumiEngine::AyumiResource::Shader*> skyShaders;
		RenderQueue renderQueue;
		int currentSky;

		//
		SceneEntity* skyGrid;
		
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
		GradientDemo();
		~GradientDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
