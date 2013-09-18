/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef GODRAYSSUNDEMO_HPP
#define GODRAYSSUNDEMO_HPP

#include <boost/lexical_cast.hpp>

#include "../Engine/ResearchEngine.hpp"
#include "../Engine/EngineFramework.hpp"

namespace ResearchEngine
{
	class GodraysSunDemo : public EngineFramework<GodraysSunDemo>
	{
		friend EngineFramework<GodraysSunDemo>;

	private:
		Camera camera;
		AyumiEngine::AyumiResource::Texture sunTexture;
		AyumiEngine::AyumiResource::Texture sunFlareTexture;
		AyumiEngine::AyumiResource::Texture cloudTexture;
		AyumiEngine::AyumiResource::Texture cloudTexture2;
		AyumiEngine::AyumiResource::Shader skyShader;
		AyumiEngine::AyumiResource::Shader godRays;
		AyumiEngine::AyumiUtils::FrameBufferObject* frame;
	
		AyumiEngine::AyumiMath::Vector3D sunPosition;
		AyumiEngine::AyumiMath::Vector3D cloudsPosition[6];
		
		float flareAngle;
		float flareScale;

		float exposure; 
		float decay;
		float density;
		float weight;

		void update();
		void render();
		void reshape(int w, int h);
		void keyboardInput(unsigned char key, int x, int y);
		void mouseInput(int x, int y);

		void initializeSky();
		void initializeRendering();

		void renderSun(float x, float y);
		void renderCloud(float x, float y);
		void renderSunAndOccluders();
	
		bool timeUpdate;
	public:
		GodraysSunDemo();
		~GodraysSunDemo();
		
		void initializeDemo();
		void doSomeMagic();
	};
}
#endif
