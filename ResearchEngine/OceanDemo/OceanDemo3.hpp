/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef OCEANDEMO3_HPP
#define OCEANDEMO3_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"
#include "../Engine/Ocean.hpp"

namespace ResearchEngine
{
	class OceanDemo3 : public EngineFramework<OceanDemo3>
	{
		friend EngineFramework<OceanDemo3>;

	private:
		Ocean* ocean;
		Camera camera;
		AyumiEngine::AyumiResource::Shader* skyShader;
		AyumiEngine::AyumiResource::Shader* hdr;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
		RenderQueue renderQueue;
	
		// data 
		GLuint transmittanceTex;
		GLuint inscatterTex;
		float sunTheta;
		float sunPhi;
		float hdrExposure;
		int width;
		int height;

		void loadTextures();

		float timeAccumulator;
		float exposure;
		bool timeUpdate;
		bool useHdr;
		
		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeRendering();

		void renderSky();
		void renderOcean();

	public:
		OceanDemo3();
		~OceanDemo3();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
