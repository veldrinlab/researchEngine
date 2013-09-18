/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef BRUNETONDEMO_HPP
#define BRUNETONDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../Engine/SceneEntity.hpp"

namespace ResearchEngine
{
	class BrunetonDemo : public EngineFramework<BrunetonDemo>
	{
		friend EngineFramework<BrunetonDemo>;

	private:
		Camera camera;
		AyumiEngine::AyumiResource::Shader skyShader;
		SceneEntity* skyGrid;
	
		float timeAccumulator;
		bool timeUpdate;
		bool updated;
		
		// data 
		GLuint transmittanceTex;
		GLuint inscatterTex;
		float sunTheta;
		float sunPhi;
		float hdrExposure;
		int width;
		int height;

		void loadTextures();


		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeSky();
		void initializeRendering();

		void renderSky();
	

	public:
		BrunetonDemo();
		~BrunetonDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
