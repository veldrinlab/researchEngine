/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "SkyDomeDemo.hpp"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	SkyDomeDemo::SkyDomeDemo()
	{
		frame = new FrameBufferObject(800,600,depth32);
		skyDome = new SceneEntity("Data/skyDome.png","Data/skyDome.vel");
	}

	SkyDomeDemo::~SkyDomeDemo()
	{
		delete frame;
		delete skyDome;
	}

	void SkyDomeDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_TEXTURE_2D);

		camera.sensivity = 0.1f;
		camera.position[1] = 0.5f;
		exposure = 2.0f;
		useHdr = false;

		initializeSky();
		initializeRendering();

		renderQueue.push_back(std::bind(&SkyDomeDemo::renderSky,this));
		renderQueue.push_back(std::bind(&SkyDomeDemo::renderGround,this));
	}

	void SkyDomeDemo::doSomeMagic()
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

	void SkyDomeDemo::update()
	{
		hdr.bindShader();
		hdr.setUniformf("exposure",exposure);
		hdr.setUniformTexture("frame",0);
		hdr.unbindShader();

		glutPostRedisplay();
	}

	void SkyDomeDemo::render()
	{
		camera.applyCamera();

		if(useHdr)
		{
			renderToTexture(renderQueue,frame);
			renderPostProcess(frame,&hdr);
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

	void SkyDomeDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void SkyDomeDemo::keyboardInput(unsigned char key, int x, int y)
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
		if(key == '1')
			exposure += 0.25f;
		if(key == '2')
			exposure -= 0.25f;
		if(key == 'h')
			useHdr = !useHdr;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void SkyDomeDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void SkyDomeDemo::initializeSky()
	{

	}

	void SkyDomeDemo::initializeRendering()
	{
		skyShader.loadShader("Shader/skyDome.vert","Shader/skyDome.frag");
		hdr.loadShader("Shader/hdr.vert","Shader/hdr.frag");
		
		skyShader.bindShader();
		skyShader.setUniformTexture("ColorMap",0);
		skyShader.unbindShader();

		hdr.bindShader();
		hdr.setUniformf("exposure",exposure);
		hdr.setUniformTexture("frame",0);
		hdr.unbindShader();

		frame->create();
		frame->bind();
		if(!frame->attachColorTexture(GL_TEXTURE_RECTANGLE, frame->createColorRectTexture(GL_RGBA,GL_RGB16F),0))
			printf ( "FBO error with color attachment\n");

		if(!frame->isOk())
			printf ( "Error with framebuffer \n" );
		frame->unbind();
	}

	void SkyDomeDemo::renderSky()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		skyShader.bindShader();
		skyShader.setUniformTexture("ColorMap",0);
		
		skyDome->render();
		skyShader.unbindShader();
	}

	void SkyDomeDemo::renderGround()
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

	void SkyDomeDemo::renderHud()
	{
		glColor3f(1.0, 1.0, 1.0);
		setOrthographicProjection();
		renderBitmapString(5,30,GLUT_BITMAP_HELVETICA_18,"HDR: ");
		if(useHdr)
			renderBitmapString(60,30,GLUT_BITMAP_HELVETICA_18,"True");
		else
			renderBitmapString(60,30,GLUT_BITMAP_HELVETICA_18,"False");

		restorePerspectiveProjection();
	}
}
