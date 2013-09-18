/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "BrunetonDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	BrunetonDemo::BrunetonDemo()
	{
		width = 800;
		height = 480;
		sunTheta = 3.14f / 2.0f - 0.05f;
		sunPhi = 0.0;
		hdrExposure = 0.4;

		skyGrid = new SceneEntity("Data/skyDome.png","Data/quad.vel");
	}

	BrunetonDemo::~BrunetonDemo()
	{
		delete skyGrid;
	}

	void BrunetonDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		timeUpdate = false;
		timeAccumulator = 0.0f;

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		camera.position[2] = 2.0f;
		updated = false;		
		initializeSky();
		initializeRendering();
	}

	void BrunetonDemo::doSomeMagic()
	{
		glutDisplayFunc(renderWrapper);
		glutReshapeFunc(reshapeWrapper);
		glutIdleFunc(updateWrapper);
		glutPassiveMotionFunc(mouseInputWrapper);
		glutKeyboardFunc(keyboardInputWrapper);
		glutWarpPointer(width/2,height/2);
		glutSetCursor(GLUT_CURSOR_NONE);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
		glutMainLoop();
	}

	void BrunetonDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(timeUpdate)
			timeAccumulator += deltaTime/15;
			
		glutPostRedisplay();
	}

	void BrunetonDemo::render()
	{
		camera.applyCamera();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		renderSky();
		glutSwapBuffers();
	}

	void BrunetonDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void BrunetonDemo::keyboardInput(unsigned char key, int x, int y)
	{
		if(key == 'w')
		{
			camera.position[0] += float(sin(camera.rotation[1] / 180 * 3.141592654f));
			camera.position[2] -= float(cos(camera.rotation[1] / 180 * 3.141592654f));
			sunTheta -= 0.1f;
		}
		if(key == 's')
		{
			camera.position[0] -= float(sin(camera.rotation[1] / 180 * 3.141592654f));
			camera.position[2] += float(cos(camera.rotation[1] / 180 * 3.141592654f));
			sunTheta += 0.02f;
		}
		if(key == 'a')
		{
			camera.position[0] -= float(cos(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			camera.position[2] -= float(sin(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			sunPhi += 0.02f;
		}
		if(key == 'd')
		{
			camera.position[0] += float(cos(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			camera.position[2] += float(sin(camera.rotation[1] / 180 * 3.141592654f))*0.2f;
			sunPhi -= 0.02f;
		}
		if(key == 'q')
			camera.position[1] += 0.1f;
		if(key == 'e')
			camera.position[1] -= 0.1f;
		if(key == 'u')
			timeUpdate = !timeUpdate;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void BrunetonDemo::mouseInput(int x, int y)
	{
		glutPostRedisplay();
	}

	void BrunetonDemo::initializeSky()
	{

	}

	void BrunetonDemo::loadTextures()
	{
		int res = 64;
		int nr = res / 2;
		int nv = res * 2;
		int nb = res / 2;
		int na = 8;
		FILE* f = fopen("Data/inscatter.raw", "rb");
		float* data = new float[nr*nv*nb*na*4];
		fread(data, 1, nr*nv*nb*na*4*sizeof(float), f);
		fclose(f);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &inscatterTex);
		glBindTexture(GL_TEXTURE_3D, inscatterTex);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F_ARB, na*nb, nv, nr, 0, GL_RGBA, GL_FLOAT, data);
		delete[] data;

		data = new float[256*64*3];
		f = fopen("Data/transmittance.raw", "rb");
		fread(data, 1, 256*64*3*sizeof(float), f);
		fclose(f);
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &transmittanceTex);
		glBindTexture(GL_TEXTURE_2D, transmittanceTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, 256, 64, 0, GL_RGB, GL_FLOAT, data);
		delete[] data;
	}

	void BrunetonDemo::initializeRendering()
	{
		loadTextures();

		skyShader.loadShader("Shader/BrunetonSky.vert","Shader/BrunetonSky.frag");
		skyShader.bindShader();
		skyShader.setUniformTexture("inscatterSampler", 0);
		skyShader.setUniformTexture("transmittanceSampler", 1);
		skyShader.unbindShader();
	}



	void BrunetonDemo::renderSky()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Vector4D sun = Vector4D(sin(sunTheta) * cos(sunPhi), sin(sunTheta) * sin(sunPhi), cos(sunTheta), 0.0);

		
		Matrix4D viewMatrix;

		viewMatrix.LoadIdentity();

		viewMatrix.Rotatef(90.0f,0.0f,1.0f,0.0f);
		viewMatrix.Rotatef(90.0f,0.0f,0.0f,1.0f);

	
		Matrix4D projectionMatrix;
		projectionMatrix.LoadIdentity();
		projectionMatrix.Perspective(90.0, 800.0f / 480.0f, 0.1f, 1000000.0f);


		skyShader.bindShader();
		skyShader.setUniformMatrix4fv("projectionMtx", projectionMatrix.data());
		skyShader.setUniformMatrix4fv("modelViewMtx",  viewMatrix.data());
		skyShader.setUniform3f("worldCamera", 0.0, 0.0, 0.0f);
		skyShader.setUniform3f("worldSunDir", sun[0], sun[1], sun[2]);
		skyShader.setUniformf("hdrExposure", hdrExposure);
		skyGrid->render();
		skyShader.unbindShader();

	}
}
