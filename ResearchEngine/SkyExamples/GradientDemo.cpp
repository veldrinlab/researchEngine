/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "GradientDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	GradientDemo::GradientDemo()
	{
		for(unsigned int i = 0; i < 13; ++i)
			skyShaders.push_back(new Shader());

		currentSky = 0;

		skyGrid = new SceneEntity("Data/skyDome.png","Data/skyDome.vel");
	}

	GradientDemo::~GradientDemo()
	{
		for(unsigned int i = 0; i < skyShaders.size(); ++i)
			delete skyShaders[i];
	}

	void GradientDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;

		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&GradientDemo::renderSky,this));
		renderQueue.push_back(std::bind(&GradientDemo::renderGround,this));
	}

	void GradientDemo::doSomeMagic()
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

	void GradientDemo::update()
	{
		glutPostRedisplay();
	}

	void GradientDemo::render()
	{
		camera.applyCamera();

	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		renderSky();

	/*	for(RenderQueue::const_iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
			(*it)();

		renderHud();*/

		glutSwapBuffers();

	}

	void GradientDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void GradientDemo::keyboardInput(unsigned char key, int x, int y)
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
			currentSky = (currentSky + 1) % 13;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void GradientDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void GradientDemo::initializeSky()
	{

	}

	void GradientDemo::initializeRendering()
	{
		skyShaders[0]->loadShader("Shader/Gradients/solidColor.vert","Shader/Gradients/solidColor.frag");
		skyShaders[1]->loadShader("Shader/Gradients/standard.vert","Shader/Gradients/standard.frag");
		skyShaders[2]->loadShader("Shader/Gradients/sky1.vert","Shader/Gradients/sky1.frag");
		skyShaders[3]->loadShader("Shader/Gradients/sky2.vert","Shader/Gradients/sky2.frag");
		skyShaders[4]->loadShader("Shader/Gradients/sky3.vert","Shader/Gradients/sky3.frag");
		skyShaders[5]->loadShader("Shader/Gradients/sky4.vert","Shader/Gradients/sky4.frag");
		skyShaders[6]->loadShader("Shader/Gradients/sky5.vert","Shader/Gradients/sky5.frag");
		skyShaders[7]->loadShader("Shader/Gradients/sky6.vert","Shader/Gradients/sky6.frag");
		skyShaders[8]->loadShader("Shader/Gradients/sky7.vert","Shader/Gradients/sky7.frag");
		skyShaders[9]->loadShader("Shader/Gradients/sky8.vert","Shader/Gradients/sky8.frag");
		skyShaders[10]->loadShader("Shader/Gradients/sky9.vert","Shader/Gradients/sky9.frag");
		skyShaders[11]->loadShader("Shader/Gradients/sky10.vert","Shader/Gradients/sky10.frag");
		skyShaders[12]->loadShader("Shader/Gradients/sky11.vert","Shader/Gradients/sky11.frag");
	}

	void GradientDemo::renderSky()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
		skyShaders[currentSky]->bindShader();	
		renderSkyPlane(&camera);
		skyShaders[currentSky]->unbindShader();
	}

	void GradientDemo::renderGround()
	{
		glPushMatrix();
		glTranslatef(0.0f,0.0f,0.0f);
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glVertex3f(-25,0, -25);
			glVertex3f(-25,0,  25);
			glVertex3f(25,0,  25);
			glVertex3f(25,0, -25);
		glEnd();
	}

	void GradientDemo::renderHud()
	{
		glColor3f(1.0, 1.0, 1.0);
		setOrthographicProjection();
		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"Press Space to next sky");
		restorePerspectiveProjection();
	}
}
