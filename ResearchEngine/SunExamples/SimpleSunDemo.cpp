/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "SimpleSunDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	SimpleSunDemo::SimpleSunDemo()
	{
		
	}

	SimpleSunDemo::~SimpleSunDemo()
	{

	}

	void SimpleSunDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		camera.position[2] = 1.5f;

		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&SimpleSunDemo::renderSky,this));
	}

	void SimpleSunDemo::doSomeMagic()
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

	void SimpleSunDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		flareAngle += 10.5f * deltaTime;

		if(flareAngle > 360.0f)
			flareAngle = 0.0f;

		flareScale += 0.125f * deltaTime;
		if(flareScale > 1.2f)
			flareScale = 0.9f;

		glutPostRedisplay();
	}

	void SimpleSunDemo::render()
	{
		camera.applyCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderSky();
		glutSwapBuffers();
	}

	void SimpleSunDemo::renderSun(float x, float y)
	{		
		glBindTexture(GL_TEXTURE_2D, sunTexture.texture);
		
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y,0.0f);
		glScalef(2.0f,2.0f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(128.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(128.0f,128.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f,128.0f);
		glEnd();
		glPopMatrix();
	
		glBindTexture(GL_TEXTURE_2D, sunFlareTexture.texture);
		
		glPushMatrix();
		glLoadIdentity();
		
		glTranslatef(128.0f+x,128.0f+y,0.0f);
		glScalef(flareScale*2.0f,flareScale*2.0f,1.0f);
		glRotatef(flareAngle,0.0f,0.0f,1.0f);
		glTranslatef(-64.0f,-64.0f,0.0f);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(128.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(128.0f,128.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f,128.0f);
		glEnd();
		glPopMatrix();
	}

	void SimpleSunDemo::renderCloud(float x, float y)
	{
		glBindTexture(GL_TEXTURE_2D,cloudTexture.texture);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y,0.0f);
		glScalef(1.4f,1.4f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(256.0f,0.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(256.0f,128.0f,0.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(0.0f,128.0f,0.0f);
		glEnd();
		glPopMatrix();
	}

	void SimpleSunDemo::renderSky()
	{
		skyShader.bindShader();
		renderSkyPlane(&camera);
		skyShader.unbindShader();

		setOrthographicProjection();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		renderSun(-64.0f,-64.0f);

		renderCloud(340.0f,170.0f);
		renderCloud(100.0f,170.0f);

		renderCloud(100.0f,370.0f);
		renderCloud(400.0f,370.0f);
		glDisable(GL_BLEND);
		
		restorePerspectiveProjection();
	}

	void SimpleSunDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void SimpleSunDemo::keyboardInput(unsigned char key, int x, int y)
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
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void SimpleSunDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void SimpleSunDemo::initializeSky()
	{
		flareAngle = 0.0f;
		flareScale = 0.9f;
	}

	void SimpleSunDemo::initializeRendering()
	{
		sunTexture.loadTexture("Data/sun.png");
		sunFlareTexture.loadTexture("Data/sunFlare.png");
		cloudTexture.loadTexture("Data/cloud.png");
		skyShader.loadShader("Shader/Gradients/comicBackground.vert","Shader/Gradients/comicBackground.frag");
	}

	void SimpleSunDemo::renderHud()
	{
		setOrthographicProjection();
		restorePerspectiveProjection();
	}
}

