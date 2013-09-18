/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "NoiseCloudDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

using namespace CW;

namespace ResearchEngine
{
	NoiseCloudDemo::NoiseCloudDemo()
	{
		srand(static_cast<unsigned int>(time(0)));
		timer = 0.0f;
		currentDemo = 0;
		mask.loadTexture("Data/mask.png");
		skyGrid = new SceneEntity("Data/skyDome.png","Data/sphere.vel");
		//test init
		accumulator = 0.0f;
		testAmount = 100;
		testAmountAccumulator = 0;
	}

	NoiseCloudDemo::~NoiseCloudDemo()
	{

	}

	void NoiseCloudDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D); 

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		initializeRendering();
	}

	void NoiseCloudDemo::doSomeMagic()
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

	void NoiseCloudDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		timer += deltaTime/5;

		if(timer > 1.0f)
			timer = 0.0f;


		glutPostRedisplay();
	}

	void NoiseCloudDemo::render()
	{
		camera.applyCamera();
		renderSky();
		renderHud();
		glutSwapBuffers();
	}

	void NoiseCloudDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void NoiseCloudDemo::keyboardInput(unsigned char key, int x, int y)
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
			currentDemo = (currentDemo + 1) % 8;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void NoiseCloudDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}
	
	void NoiseCloudDemo::initializeRendering()
	{
		classicCloud.height = classicCloud.width = 256;
		classicCloud.type = CLASSIC;
		cloudFactory.createStaticNoiseCloud(&classicCloud);

		diffCloud.height = diffCloud.width = 256;
		diffCloud.type = DIFF;
		cloudFactory.createStaticNoiseCloud(&diffCloud);

		expCloud.height = expCloud.width = 256;
		expCloud.type = EXPOTENTIAL;
		cloudFactory.createStaticNoiseCloud(&expCloud);

		classicCloudTexture.texture = cloudFactory.createCloudTexture(&classicCloud,100);
		diffCloudTexture.texture = cloudFactory.createCloudTexture(&diffCloud,100);
		expCloudTexture.texture = cloudFactory.createCloudTexture(&expCloud,100);

		classicAnimCloud.height = classicAnimCloud.width = 256;
		classicAnimCloud.type = CLASSIC;
		cloudFactory.createDynamicNoiseCloud(&classicAnimCloud,2);
		
		diffAnimCloud.height = diffAnimCloud.width = 256;
		diffAnimCloud.type = DIFF;
		cloudFactory.createDynamicNoiseCloud(&diffAnimCloud,2);

		expAnimCloud.height = expAnimCloud.width = 256;
		expAnimCloud.type = EXPOTENTIAL;
		cloudFactory.createDynamicNoiseCloud(&expAnimCloud,2);

		cloudShader.loadShader("Shader/realisticCloud.vert","Shader/realisticCloud.frag");
		cloudShader.bindShader();
		cloudShader.setUniformTexture("cloudTexture",0);
		cloudShader.unbindShader();	

		cloudShaderOverlay.loadShader("Shader/realisticCloudOverlay.vert","Shader/realisticCloudOverlay.frag");
		cloudShaderOverlay.bindShader();
		cloudShaderOverlay.setUniformTexture("cloudTexture",0);
		cloudShaderOverlay.unbindShader();	
	}

	void NoiseCloudDemo::renderSky()
	{	
		if(currentDemo == 3)
			classicAnimCloudTexture.texture = cloudFactory.createCloudTexture(&classicAnimCloud,0,1,timer,100);
		else if(currentDemo == 4)
			diffAnimCloudTexture.texture = cloudFactory.createCloudTexture(&diffAnimCloud,0,1,timer,100);
		else if(currentDemo == 5 || currentDemo == 6)
			expAnimCloudTexture.texture = cloudFactory.createCloudTexture(&expAnimCloud,0,1,timer,100);
		else if(currentDemo == 7)
		{
			cloudFactory.configureExpParameters(100.0f,0.95f);
			expAnimCloudTexture.texture = cloudFactory.createCloudTexture(&expAnimCloud,0,1,timer,100);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.365f,0.573f,0.710f,1.0f);
					  
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(currentDemo == 6 || currentDemo == 7)
			cloudShader.bindShader();
		else
		cloudShaderOverlay.bindShader();

		glActiveTexture(GL_TEXTURE0);
		
		if(currentDemo == 0)
			glBindTexture(GL_TEXTURE_2D, classicCloudTexture.texture);
		else if(currentDemo == 1)
			glBindTexture(GL_TEXTURE_2D, diffCloudTexture.texture);
		else if(currentDemo == 2)
			glBindTexture(GL_TEXTURE_2D, expCloudTexture.texture);
		else if(currentDemo == 3)
			glBindTexture(GL_TEXTURE_2D, classicAnimCloudTexture.texture);
		else if(currentDemo == 4)
			glBindTexture(GL_TEXTURE_2D, diffAnimCloudTexture.texture);
		else if(currentDemo == 5 || currentDemo == 6  || currentDemo == 7)
			glBindTexture(GL_TEXTURE_2D, expAnimCloudTexture.texture);

		renderSkyPlane(&camera);
	
	
		if(currentDemo == 7)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.365f,0.573f,0.710f,1.0f);

			renderSkyPlane(&camera);

			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glBindTexture(GL_TEXTURE_2D, mask.texture);
		
			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
		
			glBegin(GL_QUADS);
			glTexCoord2d(0,0); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2d(1,0); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2d(1,1); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2d(0,1); glVertex3f(-1.0f, 1.0f, -1.0f);
			glEnd();

			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glEnable(GL_DEPTH_TEST);

		
		}

		cloudShader.unbindShader();
		cloudShaderOverlay.unbindShader();
	}

	void NoiseCloudDemo::renderHud()
	{
		setOrthographicProjection();
		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"Press Space to next demo");
		restorePerspectiveProjection();
	}
}
