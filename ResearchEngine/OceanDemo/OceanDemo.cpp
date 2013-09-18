/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "OceanDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	OceanDemo::OceanDemo()
	{
		frame = new FrameBufferObject(WIDTH,HEIGHT,depth32);
		skyShader = new Shader();
		hdr = new Shader();
		skyGrid = new SceneEntity("Data/skyDome.png","Data/test2.vel");

		ocean = new Ocean();
	}

	OceanDemo::~OceanDemo()
	{
		delete frame;
		delete skyShader;
		delete hdr;
	}

	void OceanDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		timeUpdate = false;
		timeAccumulator = -0.5f;
		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		camera.rotation[1] = -90.0f;
		exposure = 2.0f;
		useHdr = false;

		ocean->initialize();
		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&OceanDemo::renderSky,this));
		renderQueue.push_back(std::bind(&OceanDemo::renderOcean,this));
	}

	void OceanDemo::doSomeMagic()
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

	void OceanDemo::update()
	{
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

		ocean->cameraTheta = camera.rotation[0];
		ocean->cameraPhi = camera.rotation[1];
			
		ocean->setSunPosition(-3.14f/2,CommonMath::PI*timeAccumulator);
		ocean->update(time);

		glutPostRedisplay();
	}

	void OceanDemo::render()
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

		glutSwapBuffers();
	}

	void OceanDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void OceanDemo::keyboardInput(unsigned char key, int x, int y)
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
		if(key == 27)
			glutLeaveMainLoop();
    

		if (key == 'i') 
			 ocean->cameraTheta = min(ocean->cameraTheta + 5.0f, 90.0f - 0.001f);
		if (key == 'o') 
		  ocean->cameraTheta = ocean->cameraTheta - 5.0;
    

		glutPostRedisplay();
	}

	void OceanDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);		
	//	camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void OceanDemo::initializeSky()
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

	void OceanDemo::initializeRendering()
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

	void OceanDemo::renderSky()
	{
		glViewport(0,0,WIDTH,HEIGHT);
		skyShader->bindShader();
		glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glRotatef(camera.rotation[0],1.0,0.0,0.0); 
			glRotatef(camera.rotation[1],0.0,1.0,0.0);
 
			glutSolidSphere(10.25,128,128);
			glPopMatrix();
			glEnable(GL_DEPTH_TEST);


//		glutSolidSphere(10.25,128,128);
		skyShader->unbindShader();
	}

	void OceanDemo::renderOcean()
	{
		ocean->preRender();
		ocean->renderOcean();
	}
}

