/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "CloudParticleEmiter.hpp"
 
using namespace std;
using namespace AyumiEngine::AyumiMath;
using namespace ResearchEngine;

namespace CW
{
	CloudParticleEmiter::CloudParticleEmiter(const int particleAmount, AyumiEngine::AyumiMath::Vector3D origin) : ParticleEmiter(particleAmount, origin)
	{
		spawnAccumulator = 0.5f;
		elipseRadiusOX = 256;
		elipseRadiusOY = 128/2;
		srand(time(0));

		velocity.set(CommonMath::random(-20.0f,0.0f),CommonMath::random(-20.0f,20.0f),0.0f);
	}

	CloudParticleEmiter::~CloudParticleEmiter()
	{
		for(Particles::const_iterator it = particles.begin(); it != particles.end(); ++it)
			delete (*it);
		particles.clear();
	}

	void CloudParticleEmiter::initializeEmiter()
	{		
		for(int i = 0; i < particleAmount*0.5f; ++i)
			particles.push_back(spawnParticle());
	}

	void CloudParticleEmiter::updateEmiter(const float deltaTime)
	{
		spawnAccumulator -= deltaTime;
		origin[0] += velocity[0] * deltaTime;
	
		if(particles.size() < particleAmount && spawnAccumulator < 0.0f)
		{
			spawnAccumulator = 0.5f;
			particles.push_back(spawnParticle());
		}

		for(int i = 0; i < particles.size(); ++i)
		{
			particles[i]->lifeTime -= deltaTime;

			if(particles[i]->lifeTime < 0.0f) 
				particles[i]->lifeTime = 0.0f;

			// poruszanie wszystkiego w lewo
			particles[i]->position[0] += particles[i]->velocity[0] * deltaTime;
			particles[i]->position[1] += particles[i]->velocity[1] * 0.1f*sin(deltaTime);


			//skalowanie w zale¿noœci od lifeTime
			if(!particles[i]->infinity) {
				particles[i]->color[3] = particles[i]->lifeTime;
				if(particles[i]->lifeTime < 1.0f)
					particles[i]->scale[0] = particles[i]->scale[1] = particles[i]->lifeTime;
			}

			// obrót
			particles[i]->rotation[2] -= deltaTime*3.0f;
		}
	}

	void CloudParticleEmiter::renderEmiter()
	{
	
		
		setOrthographicProjection();
		
		for(int i = 0; i < particles.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, particles[i]->texture.texture);
			glDisable(GL_DEPTH_TEST);
			glPushMatrix();
			glLoadIdentity();
			glColor4f(particles[i]->color[0],particles[i]->color[1],particles[i]->color[2],particles[i]->color[3]);
			glTranslatef(particles[i]->position.x(),particles[i]->position.y(),0.0f);
			
			glTranslatef(128.0f,128.0f,0.0f);
			//skalowanie od œrodks oczywiœcie
			//glScalef(particles[i]->scale[0],particles[i]->scale[1],1.0f);
			// obrót
			glRotatef(particles[i]->rotation[2], 0.0f,0.0f,1.0f);
			glTranslatef(-128.0f,-128.0f,0.0f);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.0f);
				glVertex2f(0.0f,0.0f);
				glTexCoord2f(1.0f,0.0f);
				glVertex2f(256.0f,0.0f);
				glTexCoord2f(1.0f,1.0f);
				glVertex2f(256.0f,256.0f);
				glTexCoord2f(0.0f,1.0f);
				glVertex2f(0.0f,256.0f);
			glEnd();
			glPopMatrix();
		}

		
		restorePerspectiveProjection();
	}

	Particle* CloudParticleEmiter::spawnParticle()
	{
		Particle* p = new Particle();
		p->position = origin;

		p->shapeRandomVariable = CommonMath::random(0.0f,1.0f);
		p->position[0] = elipseRadiusOX * cos(p->shapeRandomVariable) + origin[0];
		p->position[1] = elipseRadiusOY * sin(p->shapeRandomVariable) + origin[1];
		p->velocity.set(CommonMath::random(-20.0f,0.0f),CommonMath::random(-20.0f,20.0f),0.0f);
		p->texture = textures[static_cast<int>(CommonMath::random(0.0f,3.0f))];
		p->color.set(1.0f,1.0f,1.0f,1.0f);
		p->scale.set(1.0f,1.0f,1.0f);
		p->lifeTime = p->initLifeTime = CommonMath::random(1.0f,5.0f);

		if(CommonMath::random(0.0f,1.0f) > 0.60f)
			p->infinity = true;
		else
			p->infinity = false;

		return p;
	}
}
