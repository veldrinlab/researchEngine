/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "LensFlareSunDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	LensFlareSunDemo::LensFlareSunDemo()
	{
		
	}

	LensFlareSunDemo::~LensFlareSunDemo()
	{

	}

	void LensFlareSunDemo::initializeDemo()
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

		renderQueue.push_back(std::bind(&LensFlareSunDemo::renderSky,this));
	}

	void LensFlareSunDemo::doSomeMagic()
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

	void LensFlareSunDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		glutPostRedisplay();
	}

	void LensFlareSunDemo::render()
	{
		camera.applyCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderSky();
		glutSwapBuffers();
	}

	void LensFlareSunDemo::renderSun(float x, float y)
	{		
		glBindTexture(GL_TEXTURE_2D, sunTexture.texture);
		
		glBindTexture(GL_TEXTURE_2D,bigGlow3.texture);
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
	
	}

	void LensFlareSunDemo::renderFlare()
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(64.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(64.0f,64.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f,64.0f);
		glEnd();
	}

	void LensFlareSunDemo::renderSky()
	{
		skyShader.bindShader();
		renderSkyPlane(&camera);
		skyShader.unbindShader();

		setOrthographicProjection();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		renderSun(-64.0f,-64.0f);

		GLfloat center_x = WIDTH * 0.5f;
		GLfloat center_y = HEIGHT * 0.5f;

		GLfloat screenPos_x = -64.0f;
		GLfloat screenPos_y = -64.0f;

		// screenPos is the onscreen position of the sun
		// mind that the pixel coordinates have been scaled
		// to fit our viewport
		GLfloat dx = center_x - screenPos_x;
		GLfloat dy = center_y - screenPos_y;
		GLfloat len = sqrt(dx * dx + dy * dy);

		// normalize the vector
		GLfloat vx = dx / len;
		GLfloat vy = dy / len;
		// choose a spacing between elements
		dx = vx * len * 0.4f;
		dy = vy * len * 0.4f;

		// note that we already are in 2D (orthogonal)
		// mode here

		glPushMatrix();
		glTranslatef(screenPos_x, screenPos_y, 0);

		// travel down the line and draw the elements
		int numElements = 4;

		glBindTexture(GL_TEXTURE_2D,streaks4.texture);
		glTranslatef(dx, dy, 0);
		renderFlare();

		glBindTexture(GL_TEXTURE_2D,halo3.texture);
		glTranslatef(dx, dy, 0);
		renderFlare();

		glBindTexture(GL_TEXTURE_2D,bigGlow3.texture);
		glTranslatef(dx, dy, 0);
		renderFlare();

		glBindTexture(GL_TEXTURE_2D,hardGlow2.texture);
		glTranslatef(dx, dy, 0);
		renderFlare();


		glPopMatrix();

		glDisable(GL_BLEND);
		restorePerspectiveProjection();
	}

	void LensFlareSunDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void LensFlareSunDemo::keyboardInput(unsigned char key, int x, int y)
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

	void LensFlareSunDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void LensFlareSunDemo::initializeSky()
	{
		
	}

	void LensFlareSunDemo::initializeRendering()
	{
		sunTexture.loadTexture("Data/sun.png");
		bigGlow3.loadTexture("Data/BigGlow3.png");
		halo3.loadTexture("Data/Halo3.png");
		hardGlow2.loadTexture("Data/HardGlow2.png");
		streaks4.loadTexture("Data/Streaks4.png");
		skyShader.loadShader("Shader/Gradients/comicBackground.vert","Shader/Gradients/comicBackground.frag");
	}

	void LensFlareSunDemo::renderHud()
	{
		setOrthographicProjection();
		restorePerspectiveProjection();
	}
}

