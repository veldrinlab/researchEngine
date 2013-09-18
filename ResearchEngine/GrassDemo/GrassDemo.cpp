/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include <list>

#include "GrassDemo.hpp"

#include "noise.h"

using namespace std;
using namespace AyumiEngine::AyumiResource;
using namespace AyumiEngine::AyumiMath;
using namespace AyumiEngine::AyumiUtils;

namespace ResearchEngine
{
	//dane trawy
	Noise			noise;
											// size of height map
	#define	N1	32
	#define	N2	32

	Vec3	vertex [N1][N2];			// vertices of a heightmap

	struct	GrassVertex
	{
		Vec3	pos;
		Vec3	tex;					// z component is stiffness
		Vec3	refPoint;
	};

	struct	GrassObject
	{
		Vec3	point;					// ref point
		GrassVertex	vertex [4*3];
	};

	list<GrassObject *>	grass;


	inline	float	rnd ()
	{
		return (float) rand () / (float) RAND_MAX;
	}

	inline	float rnd ( float x1, float x2 )
	{
		return x1 + (x2 - x1) * rnd ();
	}

	inline float	heightFunc ( float x, float y )
	{
		return 5 * ( 1.0 + noise.noise ( 0.09375268*Vec3 ( x, y, 0.12387 ) ) );
	}

	void	initLandscape ()
	{
		for ( int i = 0; i < N1; i++ )
			for ( int j = 0; j < N2; j++ )
			{
				vertex [i][j].x = i - 0.5 * N1;
				vertex [i][j].y = j - 0.5 * N2;
				vertex [i][j].z = heightFunc ( vertex [i][j].x, vertex [i][j].y );
			}
	}

	void	buildGrassBase ( float start, float r, float delta, Vec3 * tri, Vec3 * gr )
	{
		for ( int i = 0; i < 3; i++ )
		{
			tri [i].x = r * cos ( start + i * M_PI * 2.0 / 3.0 );
			tri [i].y = r * sin ( start + i * M_PI * 2.0 / 3.0 );
			tri [i].z = 0;
		}
												// create grass base
		gr [0] = tri [0] - Vec3 ( 0, delta, 0 );
		gr [1] = tri [1] - Vec3 ( 0, delta, 0 );
		gr [2] = tri [1] + Vec3 ( delta, 0, 0 );
		gr [3] = tri [2] + Vec3 ( delta, 0, 0 );
		gr [4] = tri [2] + Vec3 ( 0, delta, 0 );
		gr [5] = tri [0] + Vec3 ( 0, delta, 0 );
	}

	void	initGrass ( float averageDist )
	{
												// now fill with grass objects
		Vec3	tri [3];					// triangle base (with respect to O)
		Vec3	gr  [6];					// grass base
		float		r        = 1.0;
		float		delta    = 0.3*r;
		float		upScale  = 1.0;
		float		texStart = 0.0;
		Vec3 	up ( 0, 0, r );
		int			count = 0;
		int			line  = 0;
											
		for ( float x0 = -0.5*N1; x0 < 0.5*N1; x0 += averageDist, line++ )
			for ( float y0 = -0.5*N2; y0 < 0.5*N2; y0 += averageDist )
			{
				float	x = x0 + ( rnd () - 0.5 ) * averageDist*1.2;
				float	y = y0 + ( rnd () - 0.5 ) * averageDist*1.2;
			
				if ( line & 1 )
					x += 0.5 * averageDist;
				
				if ( ((count++) % 17) == 0 )
					buildGrassBase ( rnd (), r, delta, tri, gr );
				
				Vec3	  point  = Vec3 ( x, y, heightFunc ( x, y ) );
				GrassObject * object = new GrassObject;
				
	//			texStart = 0.25*(rand () % 4);	
				up.x     = upScale*(rnd () - 0.5);
				up.y     = upScale*(rnd () - 0.5);
			
				object -> point = point;
				object -> vertex [0].pos = point + gr [0];
				object -> vertex [0].tex = Vec3 ( 0, 0, 0 );
				object -> vertex [1].pos = point + gr [1];
				object -> vertex [1].tex = Vec3 ( texStart + 0.25, 0, 0 );
				object -> vertex [2].pos = point + gr [1] + up;
				object -> vertex [2].tex = Vec3 ( texStart + 0.25, 1, 1 );
				object -> vertex [3].pos = point + gr [0] + up;
				object -> vertex [3].tex = Vec3 ( 0, 1, 1 );
	
	//			texStart = 0.25*(rand () % 4);	
				up.x     = upScale*(rnd () - 0.5);
				up.y     = upScale*(rnd () - 0.5);
			
				object -> vertex [4].pos = point + gr [2];
				object -> vertex [4].tex = Vec3 ( 0, 0, 0 );
				object -> vertex [5].pos = point + gr [3];
				object -> vertex [5].tex = Vec3 ( texStart + 0.25, 0, 0 );
				object -> vertex [6].pos = point + gr [3] + up;
				object -> vertex [6].tex = Vec3 ( texStart + 0.25, 1, 1 );
				object -> vertex [7].pos = point + gr [2] + up;
				object -> vertex [7].tex = Vec3 ( 0, 1, 1 );
	
	//			texStart = 0.25*(rand () % 4);	
				up.x     = upScale*(rnd () - 0.5);
				up.y     = upScale*(rnd () - 0.5);
			
				object -> vertex [8].pos  = point + gr [3];
				object -> vertex [8].tex  = Vec3 ( 0, 0, 0 );
				object -> vertex [9].pos  = point + gr [4];
				object -> vertex [9].tex  = Vec3 ( texStart + 0.25, 0, 0 );
				object -> vertex [10].pos = point + gr [4] + up;
				object -> vertex [10].tex = Vec3 ( texStart + 0.25, 1, 1 );
				object -> vertex [11].pos = point + gr [3] + up;
				object -> vertex [11].tex = Vec3 ( 0, 1, 1 );
			
				grass.push_back ( object );
			}
		
		printf ( "Created %d grass objects\n", grass.size () );
	}

	///////////////////////////////////


	GrassDemo::GrassDemo()
	{
		frame = new FrameBufferObject(800,600,depth32);
		skyShader = new Shader();
		hdr = new Shader();
		skyGrid = new SceneEntity("Data/skyDome.png","Data/skyDome.vel");
	}

	GrassDemo::~GrassDemo()
	{
		delete frame;
		delete skyShader;
		delete hdr;
	}

	void GrassDemo::initializeDemo()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		timeUpdate = false;
		timeAccumulator = 0.0f;

		camera.sensivity = 0.1f;
		camera.position[1] = 7.5f;
		camera.position[2] = 2.0f;
		exposure = 2.0f;
		useHdr = false;
		updated = false;
		changed = false;

		initializeSky();
		initializeRendering();
		
		renderQueue.push_back(std::bind(&GrassDemo::renderSky,this));
		renderQueue.push_back(std::bind(&GrassDemo::renderGround,this));
	}

	void GrassDemo::doSomeMagic()
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

	void GrassDemo::update()
	{
		static float lastTime = 0.0;
		float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = time-lastTime;
		lastTime = time;

		if(timeUpdate)
		{
			timeAccumulator += deltaTime/15;
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

		// grass update
		grassShader.bindShader();
		grassShader.setUniformf("time",timeAccumulator+14*deltaTime/15);
		grassShader.setUniform3f("eyePos",camera.position[0],camera.position[1],camera.position[2]);
		grassShader.unbindShader();

		glutPostRedisplay();
	}

	void GrassDemo::render()
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

	void GrassDemo::reshape(int w, int h)
	{
		glViewport(0,0,(GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(GLfloat)w/(GLfloat)h, 0.1, 10000000.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void GrassDemo::keyboardInput(unsigned char key, int x, int y)
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
			sky.turbidity += 0.01f;
			updated = !updated;
		}
		if(key == '2')
		{
			sky.turbidity -= 0.01f;
			updated = !updated;
		}
		if(key == '3')
			exposure += 1.0f;
		if(key == '4')
			exposure -= 1.0f;
		if(key == 32)
			changed = !changed;
		if(key == 27)
    		glutLeaveMainLoop();

		glutPostRedisplay();
	}

	void GrassDemo::mouseInput(int x, int y)
	{
		int dx = WIDTH/2 - x;
		int dy = -(HEIGHT/2 - y);

		if(dx != 0 || dy != 0)
			glutWarpPointer(WIDTH/2,HEIGHT/2);
	
		camera.rotation[1] -= static_cast<float>(dx * camera.sensivity);
		camera.rotation[0] += static_cast<float>(dy * camera.sensivity);

		glutPostRedisplay();
	}

	void GrassDemo::initializeSky()
	{
		sky.turbidity = 3;

		sky.lightDirection[0] = sin(CommonMath::PI*timeAccumulator);
		sky.lightDirection[1] = cos(CommonMath::PI*timeAccumulator);
		sky.lightDirection[2] = 0.0f;
		
		updatePreethamCoefficients();
		updateZenithColor();
	}

	void GrassDemo::updatePreethamCoefficients()
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

		sky.D[0] = -0.06409f * sky.turbidity - 0.89887f;
		sky.D[1] = -0.04405f * sky.turbidity - 1.65369f;
		sky.D[2] = 0.12064f * sky.turbidity - 2.57705f;

		sky.E[0] = -0.00325f * sky.turbidity + 0.04517f;
		sky.E[1] = -0.01092f * sky.turbidity + 0.05291f;
		sky.E[2] = -0.06696f * sky.turbidity + 0.37027f;

		if(changed) {
			sky.C[0] *= 0.5f;
			sky.C[1] *= 0.5f;
			sky.C[2] *= 0.5f;

			sky.D[0] *= 4.5f;
			sky.D[1] *= 4.5f;
			sky.D[2] *= 4.5f;
		}
	}

	void GrassDemo::updateZenithColor()
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

	void GrassDemo::initializeRendering()
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


		// grass
		grassTexture.loadTexture("Data/grassPack.dds");
		decalTexture.loadTexture("Data/grasslayer.dds");
		noiseTexture.loadTexture("Data/noise.bmp");
		grassShader.loadShader("Shader/grass2.vert","Shader/grass2.frag");

		grassShader.bindShader();
		grassShader.setUniformTexture("grassMap",0);
		grassShader.setUniformTexture("noiseMap",1);
		grassShader.setUniformf("time",0.0f);
		grassShader.setUniform3f("eyePos",camera.position[0],camera.position[1],camera.position[2]);
		grassShader.unbindShader();

		initLandscape();
		initGrass(0.42f);
	}

	void GrassDemo::renderSky()
	{
		skyShader->bindShader();

		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glTranslatef(0.0f,-1.0f,0.0f);
		glScalef(10.0f,10.0f,10.0f);
		glRotatef(camera.rotation[0],1.0,0.0,0.0); 
		glRotatef(camera.rotation[1],0.0,1.0,0.0);

		skyGrid->render();
	
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		
		skyShader->unbindShader();
	}

	void GrassDemo::renderGround()
	{
		glPushMatrix();

		glRotatef(270,1.0f,0.0f,0.0f);

		// teren
		glBindTexture ( GL_TEXTURE_2D, decalTexture.texture);
		glEnable      ( GL_TEXTURE_2D );
	
		
	
		for ( int i = 0; i < N1 - 1; i++ )
			for ( int j = 0; j < N2 - 1; j++ )
			{
			  glBegin       ( GL_TRIANGLES );
				glTexCoord2f ( 0, 0 );
				glVertex3fv  ( vertex [i][j] );
				glTexCoord2f ( 1, 0 );
				glVertex3fv  ( vertex [i+1][j] );
				glTexCoord2f ( 1, 1 );
				glVertex3fv  ( vertex [i+1][j+1] );
	
				glTexCoord2f ( 0, 1 );
				glVertex3fv  ( vertex [i][j+1] );
				glTexCoord2f ( 1, 1 );
				glVertex3fv  ( vertex [i+1][j+1] );
				glTexCoord2f ( 0, 0 );
				glVertex3fv  ( vertex [i][j] );
			  glEnd ();
			}

		glPopMatrix();

		// draw grass
		glPushMatrix();
		glRotatef(270,1.0f,0.0f,0.0f);
		glActiveTextureARB ( GL_TEXTURE1_ARB );
		glBindTexture      ( GL_TEXTURE_2D, noiseTexture.texture );
		glActiveTextureARB ( GL_TEXTURE0_ARB );
		glBindTexture      ( GL_TEXTURE_2D, grassTexture.texture );
		glEnable           ( GL_TEXTURE_2D );
		glEnable           ( GL_ALPHA_TEST );
		glAlphaFunc        ( GL_GEQUAL, 0.1 );
	
		grassShader.bindShader();

		for ( list<GrassObject *> :: iterator it = grass.begin (); it != grass.end (); ++it )
		{
			GrassObject * object = *it;
		
			glBegin            ( GL_QUADS );	
			glMultiTexCoord3fv ( GL_TEXTURE1_ARB, object -> point );
	
			for ( int k = 0; k < 12; k++ )
			{
				glTexCoord3fv ( object -> vertex [k].tex );
				glVertex3fv   ( object -> vertex [k].pos );
			}
	
			glEnd ();
		}

		grassShader.unbindShader();
	
		glDisable  ( GL_ALPHA_TEST );
		glPopMatrix();
	}

	void GrassDemo::renderHud()
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
