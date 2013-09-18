/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "ComicCloudDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

using namespace CW;

namespace ResearchEngine
{
	ComicCloudDemo::ComicCloudDemo()
	{
		currentDemo = 0;
		Vector3D origin;
		origin.set(400.0f-300,300.0f-128.0f,0.0f);
		particleEmiter = new CloudParticleEmiter(40,origin);
		particleEmiter2 = new CloudParticleEmiter(40,origin);
		particleEmiter3 = new CloudParticleEmiter(40,origin);
		particleEmiter4 = new CloudParticleEmiter(40,origin);
		particleEmiter5 = new CloudParticleEmiter(40,origin);


		cloud.loadTexture("Data/cloud.png"); 
		cloudSimple.loadTexture("Data/cloudSimple.png"); 
		cloudBase.loadTexture("Data/cloudBase.png"); 

		cloudTest.loadTexture("Data/cloudTest.png"); 
		cloudTest2.loadTexture("Data/cloudTest2.png"); 
		cloudTest3.loadTexture("Data/cloudTest3.png"); 
		cloudTest4.loadTexture("Data/cloudTest4.png"); 

		// coœ bajkowego
		cloudCartoon.loadTexture("Data/cloudC.png"); 


		particleEmiter->textures[0].texture = cloud.texture;
		particleEmiter->textures[1].texture = cloud.texture;
		particleEmiter->textures[2].texture = cloud.texture;
		particleEmiter->textures[3].texture = cloud.texture;

		particleEmiter2->textures[0] = cloudSimple;
		particleEmiter2->textures[1] = cloudSimple;
		particleEmiter2->textures[2] = cloudSimple;
		particleEmiter2->textures[3] = cloudSimple;

		particleEmiter3->textures[0] = cloudBase;
		particleEmiter3->textures[1] = cloudBase;
		particleEmiter3->textures[2] = cloudBase;
		particleEmiter3->textures[3] = cloudBase;

		particleEmiter4->textures[0] = cloudTest;
		particleEmiter4->textures[1] = cloudTest2;
		particleEmiter4->textures[2] = cloudTest3;
		particleEmiter4->textures[3] = cloudTest4;

		particleEmiter5->textures[0].texture = cloudCartoon.texture;
		particleEmiter5->textures[1].texture = cloudCartoon.texture;
		particleEmiter5->textures[2].texture = cloudCartoon.texture;
		particleEmiter5->textures[3].texture = cloudCartoon.texture;

		//test init
		accumulator = 0.0f;
		testAmount = 10000;
		testAmountAccumulator = 0;
	}

	ComicCloudDemo::~ComicCloudDemo()
	{

	}

	void ComicCloudDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		
		particleEmiter->initializeEmiter();
		particleEmiter2->initializeEmiter();
		particleEmiter3->initializeEmiter();
		particleEmiter4->initializeEmiter();
		particleEmiter5->initializeEmiter();

		initializeRendering();
	}

	void ComicCloudDemo::doSomeMagic()
	{
		glutDisplayFunc(renderWrapper);
		glutReshapeFunc(reshapeWrapper);
		glutIdleFunc(updateWrapper);
		glutPassiveMotionFunc(mouseInputWrapper);
		glutKeyboardFunc(keyboardInputWrapper);
		glutWarpPointer(WIDTH/2,HEIGHT/2);
		glutSetCursor(GLUT_CURSOR_NONE);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		glutMainLoop();
	}

	void ComicCloudDemo::update()
	{

		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(currentDemo == 0)
			particleEmiter->updateEmiter(deltaTime);
		else if(currentDemo == 1)
			particleEmiter2->updateEmiter(deltaTime);
		else if(currentDemo == 2)
			particleEmiter3->updateEmiter(deltaTime);
		else if(currentDemo == 3)
			particleEmiter4->updateEmiter(deltaTime);
		else if(currentDemo == 4)
			particleEmiter5->updateEmiter(deltaTime);

		glutPostRedisplay();
	}

	void ComicCloudDemo::render()
	{
		camera.applyCamera();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		skyShader.bindShader();
		renderSkyPlane(&camera);
		skyShader.unbindShader();

		if(currentDemo == 0)
			particleEmiter->renderEmiter();
		else if(currentDemo == 1)
			particleEmiter2->renderEmiter();
		else if(currentDemo == 2)
			particleEmiter3->renderEmiter();
		else if(currentDemo == 3)
			particleEmiter4->renderEmiter();
		else if(currentDemo == 4)
			particleEmiter5->renderEmiter();

		glDisable(GL_BLEND);

		renderHud();
		glutSwapBuffers();
	}

	void ComicCloudDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void ComicCloudDemo::keyboardInput(unsigned char key, int x, int y)
	{
		if(key == 'w')
		{
			camera.position[0] += float(sin(camera.rotation[1] / 180 * 3.141592654f));
			camera.position[2] -= float(cos(camera.rotation[1] / 180 * 3.141592654f));
		}
		if(key == 's')
		{
			camera.position[0] -= float(sin(camera.rotation[1] / 180 * 3.141592654f));
			camera.position[2] += float(cos(camera.rotation[1] / 180 * 3.141592654f));
		}
		if(key == 'a')
		{
			camera.position[0] -= float(cos(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			camera.position[2] -= float(sin(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
		}
		if(key == 'd')
		{
			camera.position[0] += float(cos(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			camera.position[2] += float(sin(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
		}
		if(key == 'q')
			camera.position[1] += 0.1f;
		if(key == 'e')
			camera.position[1] -= 0.1f;
		if(key == 32)
			currentDemo = (currentDemo + 1) % 5;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void ComicCloudDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void ComicCloudDemo::initializeRendering()
	{
		skyShader.loadShader("Shader/Gradients/comicBackground.vert","Shader/Gradients/comicBackground.frag");
	}

	void ComicCloudDemo::renderHud()
	{
	//	glColor3f(1.0, 1.0, 1.0);
		setOrthographicProjection();
		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"Press Space to next demo");
		restorePerspectiveProjection();
	}
}
