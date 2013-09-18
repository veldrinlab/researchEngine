/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "OceanDemo2.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	OceanDemo2::OceanDemo2()
	{
		frame = new FrameBufferObject(WIDTH,HEIGHT,depth32);
		skyShader = new Shader();
		hdr = new Shader();
		ocean = new Ocean();
	}

	OceanDemo2::~OceanDemo2()
	{
		delete frame;
		delete skyShader;
		delete hdr;
	}

	void OceanDemo2::initializeDemo()
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

		renderQueue.push_back(std::bind(&OceanDemo2::renderSky,this));
		renderQueue.push_back(std::bind(&OceanDemo2::renderOcean,this));
	}

	void OceanDemo2::doSomeMagic()
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

	void OceanDemo2::update()
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

		updatePreethamCoefficients();
		updateZenithColor();

		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("zenith",sky.zenith);
		skyShader->setUniformf("x",0.0f);
		skyShader->setUniform3fv("A",sky.A);
		skyShader->setUniform3fv("B",sky.B);
		skyShader->setUniform3fv("C",sky.C);
		skyShader->setUniform3fv("D",sky.D);
		skyShader->setUniform3fv("E",sky.E);		
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

	void OceanDemo2::render()
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

	void OceanDemo2::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void OceanDemo2::keyboardInput(unsigned char key, int x, int y)
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

	void OceanDemo2::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);		
	//	camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void OceanDemo2::initializeSky()
	{
		sky.turbidity = 3;

		sky.lightDirection[0] = sin(CommonMath::PI*timeAccumulator);
		sky.lightDirection[1] = cos(CommonMath::PI*timeAccumulator);
		sky.lightDirection[2] = 0.0f;
		
		updatePreethamCoefficients();
		updateZenithColor();
	}

	void OceanDemo2::updatePreethamCoefficients()
	{
		sky.A[0] = -0.01925f * sky.turbidity - 0.25922f;
		sky.A[1] = -0.01669f * sky.turbidity - 0.26078f;
		sky.A[2] = 0.17872f * sky.turbidity - 1.46303f;
		
		sky.B[0] = -0.06651f * sky.turbidity + 0.00081f;
		sky.B[1] = -0.09495f * sky.turbidity + 0.00921f;
		sky.B[2] = -0.35540f * sky.turbidity + 0.42749f;
		
		sky.C[0] = -0.00041f * sky.turbidity + 0.21247f;
		sky.C[1] = -0.00792f * sky.turbidity + 0.21023f;
		sky.C[2] = -0.02266f * sky.turbidity + 5.32505f;

		//sky.C[0] *= 0.5f;
		//sky.C[1] *= 0.5f;
		//sky.C[2] *= 0.5f;

		sky.D[0] = -0.06409f * sky.turbidity - 0.89887f;
		sky.D[1] = -0.04405f * sky.turbidity - 1.65369f;
		sky.D[2] = 0.12064f * sky.turbidity - 2.57705f;

		//sky.D[0] *= 4.5f;
		//sky.D[1] *= 4.5f;
		//sky.D[2] *= 4.5f;

		sky.E[0] = -0.00325f * sky.turbidity + 0.04517f;
		sky.E[1] = -0.01092f * sky.turbidity + 0.05291f;
		sky.E[2] = -0.06696f * sky.turbidity + 0.37027f;
	}

	void OceanDemo2::updateZenithColor()
	{
		const float cosTheta = sky.lightDirection[1];
		float theta = acos(cosTheta);
		Vector4D theta2 = Vector4D(1, theta, theta*theta, theta*theta*theta);
		const float turbidity2 = sky.turbidity * sky.turbidity;
		
		Vector4D cx1 = Vector4D(0.0f,0.00209f, -0.00375f, 0.00165f);
		Vector4D cx2 = Vector4D(0.00394f, -0.03202f, 0.06377f, -0.02903f);
		Vector4D cx3 = Vector4D(0.25886f, 0.06052f, -0.21196f, 0.11693f);
		Vector4D cy1 = Vector4D(0.0f, 0.00317f, -0.00610f, 0.00275f);
		Vector4D cy2 = Vector4D(0.00516f, -0.04153f, 0.08970f, -0.04214f);
		Vector4D cy3 = Vector4D(0.26688f, 0.06670f, -0.26756f, 0.15346f);
		
		Vector3D zenithYxy;
		
		zenithYxy[0] = (4.0453f * sky.turbidity - 4.971f) * tan((4/9 - sky.turbidity/120) * (CommonMath::PI - 2 * theta)) - 0.2155f * sky.turbidity + 2.4192f;
		zenithYxy[1] = turbidity2 * dot4(cx1, theta2) + sky.turbidity * dot4(cx2, theta2) + dot4(cx3, theta2);
		zenithYxy[2] = turbidity2 * dot4(cy1, theta2) + sky.turbidity * dot4(cy2, theta2) + dot4(cy3, theta2);

		Vector3D zenithColor(zenithYxy[1],zenithYxy[2], zenithYxy[0]); 
					
		Vector3D f0ThetaSun;
		f0ThetaSun[0] = (1.0f + sky.A[0] * exp(sky.B[0])) * (1.0f + sky.C[0] * exp(sky.D[0] * theta) + sky.E[0] * cosTheta * cosTheta);
		f0ThetaSun[1] = (1.0f + sky.A[1] * exp(sky.B[1])) * (1.0f + sky.C[1] * exp(sky.D[1] * theta) + sky.E[1] * cosTheta * cosTheta);
		f0ThetaSun[2] = (1.0f + sky.A[2] * exp(sky.B[2])) * (1.0f + sky.C[2] * exp(sky.D[2] * theta) + sky.E[2] * cosTheta * cosTheta);
			
		zenithColor[0] /= f0ThetaSun[0];
		zenithColor[1] /= f0ThetaSun[1];
		zenithColor[2] /= f0ThetaSun[2];

		float Kn = 0.0f;
		Kn = f0ThetaSun[2];
		Kn /= (1.0f + sky.A[2] * exp(sky.B[2]/cosTheta)) * (1.0f + sky.C[2] * exp(sky.D[2] * 0) + sky.E[2] * 1.0f);
		zenithColor[2] *= Kn;

		sky.zenith[0] = zenithColor[0];
		sky.zenith[1] = zenithColor[1];
		sky.zenith[2] = zenithColor[2];
	}

	void OceanDemo2::initializeRendering()
	{
		skyShader->loadShader("Shader/preetham.vert","Shader/preetham.frag");
		hdr->loadShader("Shader/hdr.vert","Shader/hdr.frag");
		
		skyShader->bindShader();
		skyShader->setUniform3fv("lightDirection",sky.lightDirection);
		skyShader->setUniform3fv("zenith",sky.zenith);
		skyShader->setUniform3fv("A",sky.A);
		skyShader->setUniform3fv("B",sky.B);
		skyShader->setUniform3fv("C",sky.C);
		skyShader->setUniform3fv("D",sky.D);
		skyShader->setUniform3fv("E",sky.E);
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

	void OceanDemo2::renderSky()
	{
		glViewport(0,0,WIDTH,HEIGHT);
		skyShader->bindShader();
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glTranslatef(0.0f,-1.0f,0.0f);
		glScalef(10.0f,10.0f,10.0f);
		glRotatef(camera.rotation[0],1.0,0.0,0.0); 
		glRotatef(camera.rotation[1],0.0,1.0,0.0);

		glutSolidSphere(1.0,128,128);

		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		
		skyShader->unbindShader();
	}

	void OceanDemo2::renderOcean()
	{
		ocean->preRender();
		ocean->renderOcean();
	}
}

