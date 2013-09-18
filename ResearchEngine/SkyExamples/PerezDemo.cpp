/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "PerezDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	PerezDemo::PerezDemo()
	{
		frame = new FrameBufferObject(800,600,depth32);
		skyShader = new Shader();
		hdr = new Shader();

		//test init
		accumulator = 0.0f;
		testAmount = 10000;
		testAmountAccumulator = 0;
	}

	PerezDemo::~PerezDemo()
	{
		delete frame;
		delete skyShader;
		delete hdr;
	}

	void PerezDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		timeUpdate = true;
		timeAccumulator = -0.75f;
		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		exposure = 2.0f;
		useHdr = false;
		updated = false;

		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&PerezDemo::renderSky,this));
		renderQueue.push_back(std::bind(&PerezDemo::renderGround,this));
	}

	void PerezDemo::doSomeMagic()
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

	void PerezDemo::update()
	{
		// start test
		float testStart = getElapsedTime();


		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(timeUpdate)
		{
			timeAccumulator += deltaTime/10;
			sky.lightDirection[0] = sin(CommonMath::PI*timeAccumulator);
			sky.lightDirection[1] = cos(CommonMath::PI*timeAccumulator);
			updatePerezCoefficients();
			updateZenithColor();
		}

		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("zenithYxy",sky.zenithYxy);
		
		if(updated)
		{
			updatePerezCoefficients();
			updateZenithColor();
			skyShader->setUniformf("turbidity",sky.turbidity);
			skyShader->setUniform3fv("AYxy",sky.AYxy);
			skyShader->setUniform3fv("BYxy",sky.BYxy);
			skyShader->setUniform3fv("CYxy",sky.CYxy);
			skyShader->setUniform3fv("DYxy",sky.DYxy);
			skyShader->setUniform3fv("EYxy",sky.EYxy);
			updated = !updated;
		}
		skyShader->unbindShader();

		hdr->bindShader();
		hdr->setUniformf("exposure",exposure);
		hdr->setUniformTexture("frame",0);
		hdr->unbindShader();


		//test end
		float testEnd = getElapsedTime();

		// test calculation
		accumulator += testEnd - testStart;
		testAmountAccumulator++;
		if(testAmountAccumulator == testAmount)
		{
			cout << (accumulator / testAmount) << endl;
			system("pause");
			exit(0);
		}
		

		glutPostRedisplay();
	}

	void PerezDemo::render()
	{
		camera.applyCamera();

		if(useHdr)
		{
			renderToTexture(renderQueue,frame);
			renderPostProcess(frame,hdr);
		}
		else
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			for(RenderQueue::const_iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
				(*it)();
		}

		renderHud();
		glutSwapBuffers();
	}

	void PerezDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void PerezDemo::keyboardInput(unsigned char key, int x, int y)
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
		if(key == 'u')
			timeUpdate = !timeUpdate;
		if(key == 'h')
			useHdr = !useHdr;
		if(key == '1')
		{
			sky.turbidity += 0.5f;
			updated = !updated;
		}
		if(key == '2')
		{
			sky.turbidity -= 0.5f;
			updated = !updated;
		}
		if(key == '3')
			exposure += 1.0f;
		if(key == '4')
			exposure -= 1.0f;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void PerezDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void PerezDemo::initializeSky()
	{
		sky.turbidity = 10;
		sky.lightDirection[0] = -1.0f;
		sky.lightDirection[1] = 0.0f;
		sky.lightDirection[2] = 0.0;

		updatePerezCoefficients();
		updateZenithColor();
	}

	void PerezDemo::updatePerezCoefficients()
	{
		sky.AYxy[0] = 0.17872f * sky.turbidity - 1.46303f;
		sky.AYxy[1] = -0.01925f * sky.turbidity - 0.25922f;
		sky.AYxy[2] = -0.01669f * sky.turbidity - 0.26078f;
		
		sky.BYxy[0] = -0.35540f * sky.turbidity + 0.42749f;
		sky.BYxy[1] = -0.06651f * sky.turbidity + 0.00081f;
		sky.BYxy[2] = -0.09495f * sky.turbidity + 0.00921f;
		
		sky.CYxy[0] = -0.02266f * sky.turbidity + 5.32505f;
		sky.CYxy[1] = -0.00041f * sky.turbidity + 0.21247f;
		sky.CYxy[2] = -0.00792f * sky.turbidity + 0.21023f;
		
		sky.DYxy[0] = 0.12064f * sky.turbidity - 2.57705f;
		sky.DYxy[1] = -0.06409f * sky.turbidity - 0.89887f;
		sky.DYxy[2] = -0.04405f * sky.turbidity - 1.65369f;
		
		sky.EYxy[0] = -0.06696f * sky.turbidity + 0.37027f;
		sky.EYxy[1] = -0.00325f * sky.turbidity + 0.04517f;
		sky.EYxy[2] = -0.01092f * sky.turbidity + 0.05291f;
	}

	void PerezDemo::updateZenithColor()
	{
		const float thetaSun = acos(sky.lightDirection[1]);
		const float turbidity2 = sky.turbidity * sky.turbidity;
		const float chi = (4.0f / 9.0f - sky.turbidity / 120.0f) * (CommonMath::PI - 2.0f * thetaSun);

		Vector4D cx1 = Vector4D(0.0f,0.00209f, -0.00375f, 0.00165f);
		Vector4D cx2 = Vector4D(0.00394f, -0.03202f, 0.06377f, -0.02903f);
		Vector4D cx3 = Vector4D(0.25886f, 0.06052f, -0.21196f, 0.11693f);
		Vector4D cy1 = Vector4D(0.0f, 0.00317f, -0.00610f, 0.00275f);
		Vector4D cy2 = Vector4D(0.00516f, -0.04153f, 0.08970f, -0.04214f);
		Vector4D cy3 = Vector4D(0.26688f, 0.06670f, -0.26756f, 0.15346f);

		Vector4D theta = Vector4D(1, thetaSun, thetaSun*thetaSun, thetaSun*thetaSun*thetaSun);

		sky.zenithYxy[0] = (4.0453f * sky.turbidity - 4.9710f) * tan(chi) - 0.2155f * sky.turbidity + 2.4192f;
		sky.zenithYxy[1] = turbidity2 * dot4(cx1, theta) + sky.turbidity * dot4(cx2, theta) + dot4(cx3, theta);
		sky.zenithYxy[2] = turbidity2 * dot4(cy1, theta) + sky.turbidity * dot4(cy2, theta) + dot4(cy3, theta);
	}

	void PerezDemo::initializeRendering()
	{
		skyShader->loadShader("Shader/perez.vert","Shader/perez.frag");
		hdr->loadShader("Shader/hdr.vert","Shader/hdr.frag");
		
		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("zenithYxy",sky.zenithYxy);
		skyShader->setUniformf("turbidity",sky.turbidity);
		skyShader->setUniform3fv("AYxy",sky.AYxy);
		skyShader->setUniform3fv("BYxy",sky.BYxy);
		skyShader->setUniform3fv("CYxy",sky.CYxy);
		skyShader->setUniform3fv("DYxy",sky.DYxy);
		skyShader->setUniform3fv("EYxy",sky.EYxy);
		skyShader->unbindShader();

		hdr->bindShader();
		hdr->setUniformf("exposure",exposure);
		hdr->setUniformTexture("frame",0);
		hdr->unbindShader();

		frame->create();
		frame->bind();
		if(!frame->attachColorTexture(GL_TEXTURE_RECTANGLE, frame->createColorRectTexture(GL_RGBA,GL_RGB16F),0))
			printf ( "FBO error with color attachment\n");

		if(!frame->isOk())
			printf ( "Error with framebuffer \n" );
		frame->unbind();
	}

	void PerezDemo::renderSky()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
		skyShader->bindShader();
	//	glutSolidSphere ( 22, 40, 40 );
	//	renderSkyBox(&camera);
		renderSkyDome(&camera,true);
		skyShader->unbindShader();
	}

	void PerezDemo::renderGround()
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

	void PerezDemo::renderHud()
	{
		glColor3f(1.0, 1.0, 1.0);
		setOrthographicProjection();

		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"Turbidity: ");
		renderBitmapString(88,30, GLUT_BITMAP_HELVETICA_18,boost::lexical_cast<string>(sky.turbidity).c_str());

		renderBitmapString(5,60,GLUT_BITMAP_HELVETICA_18,"Exposure: ");
		renderBitmapString(88,60, GLUT_BITMAP_HELVETICA_18,boost::lexical_cast<string>(exposure).c_str());
	
		renderBitmapString(5,90,GLUT_BITMAP_HELVETICA_18,"Update: ");
		if(timeUpdate)
			renderBitmapString(72,90,GLUT_BITMAP_HELVETICA_18,"True");
		else
			renderBitmapString(72,90,GLUT_BITMAP_HELVETICA_18,"False");
		
		renderBitmapString(5,120,GLUT_BITMAP_HELVETICA_18,"HDR: ");
		if(useHdr)
			renderBitmapString(48,120,GLUT_BITMAP_HELVETICA_18,"On");
		else
			renderBitmapString(48,120,GLUT_BITMAP_HELVETICA_18,"Off");

		restorePerspectiveProjection();
	}
}
