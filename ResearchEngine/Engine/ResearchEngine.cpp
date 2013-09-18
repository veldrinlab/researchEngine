/**
 * File contains definition of ResearchEngine functions.
 * @file    ResearchEngine.cpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2012-05-23
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "ResearchEngine.hpp"

using namespace std;
using namespace AyumiEngine::AyumiUtils;
using namespace AyumiEngine::AyumiResource;

namespace ResearchEngine
{
	// timer
	LARGE_INTEGER uFreq;

	void initializeEngine(int argc, char* argv[])
	{
		// timer
		QueryPerformanceFrequency (&uFreq);

		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(WIDTH,HEIGHT);
		glutInitContextVersion(2, 1);
		glutCreateWindow("Research Engine");

		initializeGlew();
		initializeDevIL();
	}

	void initializeDevIL()
	{
		if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)	
			std::cerr << "Wrong DevIL version detected!" << std::endl;
			
		ilInit();
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	}

	void initializeGlew()
	{
		glewExperimental = GL_TRUE;
		glewInit();

		if(!GLEW_VERSION_2_1)
			cerr << "OpenGL 2.1 not supported.\n" << endl;
	}

	void setOrthographicProjection()
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, HEIGHT, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

	}

	void restorePerspectiveProjection()
	{
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void renderBitmapString(float x,float y,void* font, const char* string)
	{
		char* c;
		glRasterPos2f(x, y);
		for(c=const_cast<char*>(string); *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
	  }
	}

	void renderSkyPlane(Camera* camera)
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		glBegin(GL_QUADS);
		glTexCoord2d(0,0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2d(1,0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2d(1,1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2d(0,1); glVertex3f(-1.0f, 1.0f, -1.0f);
		glEnd();

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

	void renderSkyBox(Camera* camera)
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRotatef(camera->rotation[0],1.0,0.0,0.0); 
		glRotatef(camera->rotation[1],0.0,1.0,0.0); 
		glutSolidCube(10.25);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

	void renderSkyDome(Camera* camera, bool infinity)
	{
		if(infinity)
		{
			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glRotatef(camera->rotation[0],1.0,0.0,0.0); 
			glRotatef(camera->rotation[1],0.0,1.0,0.0);
 
			glutSolidSphere(1.025,128,128);
			glPopMatrix();
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			//alwayssome offset, from atmosphere/sky radius - camera height
			glPushMatrix();
			glTranslatef(camera->position[0],camera->position[1]-10.0f-1.0e-6f,camera->position[2]);
			glutSolidSphere(10.25,64,64);
			glPopMatrix();
		}
	}

	void renderToTexture(const RenderQueue& renderQueue, FrameBufferObject* target)
	{
		target->bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		for(RenderQueue::const_iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
			(*it)();

		target->unbind();
	}

	void renderPostProcess(FrameBufferObject* target, Shader* shader, bool clear)
	{
		if(clear)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}

		glEnable(GL_TEXTURE_RECTANGLE);
		setOrthographicProjection();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_RECTANGLE, target->getColorBuffer());
	
		shader->bindShader();
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
		shader->unbindShader();
		
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		glDisable(GL_TEXTURE_RECTANGLE);
		restorePerspectiveProjection();
	}

	float getElapsedTime()
	{
		LARGE_INTEGER uTicks;
		QueryPerformanceCounter (&uTicks);
		return static_cast<float>(uTicks.QuadPart / static_cast<double>(uFreq.QuadPart));
	}
}