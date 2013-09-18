/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "GodraysSunDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	GodraysSunDemo::GodraysSunDemo()
	{
		frame = new FrameBufferObject(WIDTH,HEIGHT,depth32);
		timeUpdate = false;
	}

	GodraysSunDemo::~GodraysSunDemo()
	{
		delete frame;
	}

	void GodraysSunDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_RECTANGLE);

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		camera.position[2] = 1.5f;

		initializeSky();
		initializeRendering();
	}

	void GodraysSunDemo::doSomeMagic()
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

	void GodraysSunDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(timeUpdate) {
			for(int i = 0; i < 6; ++i)
			{
				cloudsPosition[i][0] -= 100.0f * deltaTime;
				if(cloudsPosition[i][0] < -WIDTH/2)
					cloudsPosition[i].set(WIDTH+CommonMath::random(0.0f,200.0f),sunPosition[1]+CommonMath::random(0.0f,420.0f),0.0f);
			}

		}

		// update rendering

		godRays.bindShader();
		godRays.setUniformf("lightPositionOnScreenX",sunPosition[0]+96*0.5f);
		godRays.setUniformf("lightPositionOnScreenY",sunPosition[1]+96*0.5f);
		godRays.unbindShader();

		glutPostRedisplay();
	}

	void GodraysSunDemo::render()
	{
		camera.applyCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// render pre pass to frame buffer
		frame->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderSunAndOccluders();
		frame->unbind();
		glClear(GL_DEPTH_BUFFER_BIT);

		// render normal scene

		skyShader.bindShader();
		renderSkyPlane(&camera);
		skyShader.unbindShader();

		setOrthographicProjection();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, cloudTexture.texture);
		for(int i = 0; i < 6; ++i)
			renderCloud(cloudsPosition[i][0],cloudsPosition[i][1]);
		restorePerspectiveProjection();

		// render post-process to make god rays with blending

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glEnable(GL_TEXTURE_RECTANGLE);
		setOrthographicProjection();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_RECTANGLE, frame->getColorBuffer());
	
		godRays.bindShader();
		glBegin(GL_QUADS);
			glTexCoord2i(0,HEIGHT);
			glVertex2i(0,0);
			glTexCoord2i(WIDTH,HEIGHT);
			glVertex2i(WIDTH,0);
			glTexCoord2i(WIDTH,0);
			glVertex2i(WIDTH,HEIGHT);
			glTexCoord2i(0,0);
			glVertex2i(0,HEIGHT);
		glEnd();
		godRays.unbindShader();
		
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		glDisable(GL_TEXTURE_RECTANGLE);
		restorePerspectiveProjection();

		glutSwapBuffers();
	}

	void GodraysSunDemo::renderSun(float x, float y)
	{		
		glBindTexture(GL_TEXTURE_2D, sunTexture.texture);
		
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y,0.0f);
		glScalef(1.0f,1.0f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f,0.0f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(96.0f,0.0f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(96.0f,96.0f);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f,96.0f);
		glEnd();
		glPopMatrix();
	
		//glBindTexture(GL_TEXTURE_2D, sunFlareTexture.texture);
		//
		//glPushMatrix();
		//glLoadIdentity();
		//
		//glTranslatef(128.0f+x,128.0f+y,0.0f);
		//glScalef(flareScale*2.0f,flareScale*2.0f,1.0f);
		//glRotatef(flareAngle,0.0f,0.0f,1.0f);
		//glTranslatef(-64.0f,-64.0f,0.0f);
		//
		//glBegin(GL_QUADS);
		//	glTexCoord2f(0.0f,0.0f);
		//	glVertex2f(0.0f,0.0f);
		//	glTexCoord2f(1.0f,0.0f);
		//	glVertex2f(128.0f,0.0f);
		//	glTexCoord2f(1.0f,1.0f);
		//	glVertex2f(128.0f,128.0f);
		//	glTexCoord2f(0.0f,1.0f);
		//	glVertex2f(0.0f,128.0f);
		//glEnd();
		//glPopMatrix();
	}

	void GodraysSunDemo::renderCloud(float x, float y)
	{				
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x,y,0.0f);

		//
	//	glScalef(1.4f,1.4f,1.0f);

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

	void GodraysSunDemo::renderSunAndOccluders()
	{
		// TODO optymalizacje mo¿liwe - mniejszy rozmiar, wy³¹czyæ tekstury/g³êbiê
		setOrthographicProjection();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		renderSun(sunPosition[0],sunPosition[1]);

		glBindTexture(GL_TEXTURE_2D, cloudTexture2.texture);
		for(int i = 0; i < 6; ++i)
			renderCloud(cloudsPosition[i][0],cloudsPosition[i][1]);
		
		glDisable(GL_BLEND);
		restorePerspectiveProjection();
	}

	void GodraysSunDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void GodraysSunDemo::keyboardInput(unsigned char key, int x, int y)
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
			timeUpdate = !timeUpdate;
		if(key == 'e')
			camera.position[1] -= 0.1f;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void GodraysSunDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void GodraysSunDemo::initializeSky()
	{
		flareAngle = 0.0f;
		flareScale = 0.9f;

		sunPosition[0] = WIDTH*0.5f-96*0.5f;
		sunPosition[1] = HEIGHT*0.5f-96.0f*0.5f;

		cloudsPosition[0] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);
		cloudsPosition[1] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);
		cloudsPosition[2] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);
		cloudsPosition[3] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);
		cloudsPosition[4] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);
		cloudsPosition[5] = Vector3D(sunPosition[0]+CommonMath::random(0.0f,HEIGHT),sunPosition[1]+CommonMath::random(0.0f,420.0f),0);

		cloudsPosition[0] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);
		cloudsPosition[1] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);
		cloudsPosition[2] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);
		cloudsPosition[3] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);
		cloudsPosition[4] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);
		cloudsPosition[5] = Vector3D(+CommonMath::random(0.0f,HEIGHT),CommonMath::random(0.0f,WIDTH),0);

		exposure = 0.0034f;
		decay = 1.0f;
		density = 0.84f;
		weight = 5.65f;
	}

	void GodraysSunDemo::initializeRendering()
	{
		sunTexture.loadTexture("Data/sun.png");
		sunFlareTexture.loadTexture("Data/sunFlare.png");
		cloudTexture.loadTexture("Data/cloud.png");
		cloudTexture2.loadTexture("Data/cloud2.png");

		skyShader.loadShader("Shader/Gradients/comicBackground.vert","Shader/Gradients/comicBackground.frag");
		godRays.loadShader("Shader/godRays.vert","Shader/godRays.frag");

		godRays.bindShader();
		godRays.setUniformTexture("frame",0);
		godRays.setUniformf("exposure",exposure);
		godRays.setUniformf("decay",decay);
		godRays.setUniformf("density",density);
		godRays.setUniformf("weight",weight);
		godRays.unbindShader();

		frame->create();
		frame->bind();
		if(!frame->attachColorTexture(GL_TEXTURE_RECTANGLE, frame->createColorRectTexture(GL_RGBA,GL_RGB8),0))
			printf ( "FBO error with color attachment\n");

		if(!frame->isOk())
			printf ( "Error with framebuffer \n" );
		frame->unbind();
	}
}

