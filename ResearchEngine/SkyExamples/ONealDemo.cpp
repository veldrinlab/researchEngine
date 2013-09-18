/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
 * Szymon Jab�o�ski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "ONealDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	ONealDemo::ONealDemo()
	{
		frame = new FrameBufferObject(800,600,depth32);
		skyShader = new Shader();
		hdr = new Shader();
		skyGrid = new SceneEntity("Data/skyDome.png","Data/test2.vel");
	}

	ONealDemo::~ONealDemo()
	{
		delete frame;
		delete skyShader;
		delete hdr;
	}

	void ONealDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		timeUpdate = false;
		timeAccumulator = -0.75f;
		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		exposure = 2.0f;
		useHdr = false;

		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&ONealDemo::renderSky,this));
		renderQueue.push_back(std::bind(&ONealDemo::renderGround,this));

	}

	void ONealDemo::doSomeMagic()
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

	void ONealDemo::update()
	{
		// start test
		float testStart = getElapsedTime();

		// timer - delta time
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(timeUpdate)
		{
			timeAccumulator += deltaTime/10;

			sky.lightDirection[0] = sin(CommonMath::PI*timeAccumulator);
			sky.lightDirection[1] = cos(CommonMath::PI*timeAccumulator);
		}

		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("waveLength",sky.waveLength);
		skyShader->setUniformf("rayleighConstantSun",sky.rayleighConstant*sky.sunBrightness);
		skyShader->setUniformf("mieConstantSun",sky.mieConstant*sky.sunBrightness);
		skyShader->setUniformf("rayleighConstant",sky.rayleighConstant*4*CommonMath::PI);
		skyShader->setUniformf("mieConstant",sky.mieConstant*4*CommonMath::PI);
		skyShader->setUniformf("skyRadius",sky.skyRadius);
		skyShader->setUniformf("scatteringSymmetry",sky.scatteringSymmetry);
		skyShader->setUniformf("scatteringSymmetry2",sky.scatteringSymmetry*sky.scatteringSymmetry);
		skyShader->setUniformi("samples",sky.samples);
		skyShader->unbindShader();

		hdr->bindShader();
		hdr->setUniformf("exposure",exposure);
		hdr->setUniformTexture("frame",0);
		hdr->unbindShader();

				


		glutPostRedisplay();
	}

	void ONealDemo::render()
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

	void ONealDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void ONealDemo::keyboardInput(unsigned char key, int x, int y)
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
		if(key == '1')
			sky.sunBrightness += 0.1f;
		if(key == '2')
			sky.sunBrightness -= 0.1f;
		if(key == '3')
			sky.rayleighConstant += 0.0001f;
		if(key == '4')
			sky.rayleighConstant -= 0.0001f;
		if(key == '5')
			sky.mieConstant += 0.0001f;
		if(key == '6')
			sky.mieConstant -= 0.0001f;
		if(key == '7')
			exposure += 0.25f;
		if(key == '8')
			exposure -= 0.25f;
		if(key == 'h')
			useHdr = !useHdr;
		if(key == 27)
			glutLeaveMainLoop();
    
		glutPostRedisplay();
	}

	void ONealDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void ONealDemo::initializeSky()
	{
		sky.sunBrightness = 20.0f;
		sky.rayleighConstant = 0.0025f;
		sky.mieConstant = 0.0015f;
		sky.scatteringSymmetry = -0.991f;
		sky.skyRadius = 10.0f;
		sky.waveLength[0] = 1/powf(0.650f,4.0f);
		sky.waveLength[1] = 1/powf(0.570f,4.0f);
		sky.waveLength[2] = 1/powf(0.475f,4.0f);
		sky.samples = 2;
		sky.lightDirection[0] = sin(CommonMath::PI*timeAccumulator);
		sky.lightDirection[1] = cos(CommonMath::PI*timeAccumulator);
		sky.lightDirection[2] = 0.0f;
	}

	void ONealDemo::initializeRendering()
	{
		skyShader->loadShader("Shader/oneil.vert","Shader/oneil.frag");
		hdr->loadShader("Shader/hdr.vert","Shader/hdr.frag");
		
		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("waveLength",sky.waveLength);
		skyShader->setUniformf("rayleighConstantSun",sky.rayleighConstant*sky.sunBrightness);
		skyShader->setUniformf("mieConstantSun",sky.mieConstant*sky.sunBrightness);
		skyShader->setUniformf("rayleighConstant",sky.rayleighConstant*4*CommonMath::PI);
		skyShader->setUniformf("mieConstant",sky.mieConstant*4*CommonMath::PI);
		skyShader->setUniformf("skyRadius",sky.skyRadius);
		skyShader->setUniformf("scatteringSymmetry",sky.scatteringSymmetry);
		skyShader->setUniformf("scatteringSymmetry2",sky.scatteringSymmetry*sky.scatteringSymmetry);
		skyShader->setUniformi("samples",sky.samples);
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

	void ONealDemo::renderSky()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
		camera.applyCamera();
		skyShader->bindShader();

	//	glPushMatrix();

		//glScalef(200.0,200.0,200.0);
		//glTranslatef(0,-10.0f-1.0e-6f,0);
		skyGrid->render();
	//	glScalef(100.0f,100.0f,100.0f);

		//TODO bardziej niebiesko, zbyt czarno jest

//		glutSolidSphere(10.25,128,128);
		skyShader->unbindShader();
	//	glPopMatrix();
	}

	void ONealDemo::renderGround()
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

	void ONealDemo::renderHud()
	{
		glColor3f(1.0, 1.0, 1.0);
		setOrthographicProjection();
		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"Sun Intesity: ");
		renderBitmapString(112,30, GLUT_BITMAP_HELVETICA_18,boost::lexical_cast<string>(sky.sunBrightness).c_str());

		renderBitmapString(5,60,GLUT_BITMAP_HELVETICA_18,"Raylegh: ");
		renderBitmapString(81,60, GLUT_BITMAP_HELVETICA_18,boost::lexical_cast<string>(sky.rayleighConstant).c_str());

		renderBitmapString(5,90,GLUT_BITMAP_HELVETICA_18,"Mie: ");
		renderBitmapString(40,90, GLUT_BITMAP_HELVETICA_18,boost::lexical_cast<string>(sky.mieConstant).c_str());
	
		renderBitmapString(5,120,GLUT_BITMAP_HELVETICA_18,"Update: ");
		if(timeUpdate)
			renderBitmapString(72,120,GLUT_BITMAP_HELVETICA_18,"True");
		else
			renderBitmapString(72,120,GLUT_BITMAP_HELVETICA_18,"False");

		renderBitmapString(5,150,GLUT_BITMAP_HELVETICA_18,"HDR: ");
		if(useHdr)
			renderBitmapString(48,150,GLUT_BITMAP_HELVETICA_18,"On");
		else
			renderBitmapString(48,150,GLUT_BITMAP_HELVETICA_18,"Off");

		restorePerspectiveProjection();
	}
}

