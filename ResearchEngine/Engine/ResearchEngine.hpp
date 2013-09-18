/**
 * File contains declaraion of ResearchEngine functions.
 * @file    ResearchEngine.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2012-05-23
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef RESEARCHENGINE_HPP
#define RESEARCHENGINE_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <functional>

#include "Camera.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "FrameBufferObject.hpp"

namespace ResearchEngine
{
	typedef std::vector<std::function<void()>> RenderQueue;

	static const int WIDTH = 800;
	static const int HEIGHT = 480;

	void initializeEngine(int argc, char* argv[]);
	void initializeDevIL();
	void initializeGlew();

	void setOrthographicProjection();
	void restorePerspectiveProjection();
	void renderBitmapString(float x, float y, void* font, const char* string);

	void renderSkyPlane(Camera* camera);
	void renderSkyBox(Camera* camera);
	void renderSkyDome(Camera* camera, bool infinity = true);
	


	void renderToTexture(const RenderQueue& renderQueue, AyumiEngine::AyumiUtils::FrameBufferObject* target);
	void renderPostProcess(AyumiEngine::AyumiUtils::FrameBufferObject* target, AyumiEngine::AyumiResource::Shader* shader, bool clear = true);


	// timer

	float getElapsedTime();
}
#endif
