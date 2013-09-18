/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "CloudFactory.hpp"

namespace CW
{
	CloudFactory::CloudFactory()
	{
		cover = 40.0f;
		sharpness = 0.95f;
	}

	CloudFactory::~CloudFactory()
	{

	}

	void CloudFactory::createStaticNoiseCloud(StaticNoiseCloud* noiseCloud)
	{
		noiseCloud->data = new float[noiseCloud->width*noiseCloud->height];
	
		if(noiseCloud->type == CLASSIC)
		{
			for(int x = 0; x < noiseCloud->width; ++x) 
				for(int y = 0; y < noiseCloud->height; ++y)
					noiseCloud->data[x*noiseCloud->width+y] = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;
		}
		else if(noiseCloud->type == DIFF)
		{
			for(int x = 0; x < noiseCloud->width; ++x) 
				for(int y = 0; y < noiseCloud->height; ++y)
					noiseCloud->data[x*noiseCloud->width+y] = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;

			textureFactory.getNoiseGenerator()->chnageSeed();
			
			for(int x = 0; x < noiseCloud->width; ++x)
				for(int y = 0; y < noiseCloud->height; ++y)
				{
					float value = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;
					value -= noiseCloud->data[x*noiseCloud->width+y];
				
					if(value > 255.0f)
						value = 255.0f;
					else if(value < 0.0f)
						value = 0.0f;

					noiseCloud->data[x*noiseCloud->width+y] = value;
				}
		}
		else if(noiseCloud->type == EXPOTENTIAL)
		{
			noiseCloud->data = new float[noiseCloud->width*noiseCloud->height];
			float* block = new float[noiseCloud->width*noiseCloud->height/64];

			textureFactory.setNoise(noiseCloud->width,noiseCloud->height,block);
			textureFactory.overlapOctaves(noiseCloud->width,noiseCloud->height,block,noiseCloud->data);
			textureFactory.expotentialFilter(noiseCloud->width,noiseCloud->height,noiseCloud->data,cover,sharpness);
		}
	}

	void CloudFactory::createDynamicNoiseCloud(DynamicNoiseCloud* noiseCloud, const int statesAmount)
	{
		for(int i = 0; i < statesAmount+1; ++i)
		{
			CloudData cloudData = nullptr;

			if(noiseCloud->type == EXPOTENTIAL)
				cloudData = new float[noiseCloud->width*noiseCloud->height/64];
			else
				cloudData = new float[noiseCloud->width*noiseCloud->height];

			noiseCloud->statesAmount = statesAmount+1;
			noiseCloud->data.push_back(cloudData);
		}

		if(noiseCloud->type == CLASSIC)
		{
			for(int i = 0; i < statesAmount; ++i)
			{
				for(int x = 0; x < noiseCloud->width; ++x) 
					for(int y = 0; y < noiseCloud->height; ++y)
						noiseCloud->data.at(i)[x*noiseCloud->width+y] = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;
				
				textureFactory.getNoiseGenerator()->chnageSeed();
			}
		}
		else if(noiseCloud->type == DIFF)
		{
			for(int i = 0; i < statesAmount; ++i)
			{
				for(int x = 0; x < noiseCloud->width; ++x) 
					for(int y = 0; y < noiseCloud->height; ++y)
						noiseCloud->data.at(i)[x*noiseCloud->width+y] = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;

			textureFactory.getNoiseGenerator()->chnageSeed();
			
			for(int x = 0; x < noiseCloud->width; ++x)
				for(int y = 0; y < noiseCloud->height; ++y)
				{
					float value = (128.0f*textureFactory.getNoiseGenerator()->perlinNoise2D(x,y))+128.0f;
					value -= noiseCloud->data.at(i)[x*noiseCloud->width+y];
				
					if(value > 255.0f)
						value = 255.0f;
					else if(value < 0.0f)
						value = 0.0f;

					noiseCloud->data.at(i)[x*noiseCloud->width+y] = value;
				}
			}
		}
		else if(noiseCloud->type == EXPOTENTIAL)
		{
			for(int i = 0; i < statesAmount; ++i) 
			{
				textureFactory.getNoiseGenerator()->chnageSeed();
				textureFactory.setNoise(noiseCloud->width,noiseCloud->height,noiseCloud->data.at(i));
			}
		}
	}

	GLuint CloudFactory::createCloudTexture(StaticNoiseCloud* noiseCloud, const int alphaThreshold)
	{
		return textureFactory.buildTexture(noiseCloud->width,noiseCloud->height,alphaThreshold,noiseCloud->data);
	}

	GLuint CloudFactory::createCloudTexture(DynamicNoiseCloud* noiseCloud, const int begin, const int end, const float a, const int alphaThreshold)
	{
		if(noiseCloud->type == EXPOTENTIAL)
		{
			float* data = new float[noiseCloud->width*noiseCloud->height];
			
			for(int i = 0; i < noiseCloud->width/8; ++i)
				for(int j = 0; j < noiseCloud->height/8; ++j)
					noiseCloud->data.at(end+1)[i*noiseCloud->width/8+j] = (1.0f - a) * noiseCloud->data.at(begin)[i*noiseCloud->width/8+j] + a * noiseCloud->data.at(end)[i*noiseCloud->width/8+j];

			textureFactory.overlapOctaves(noiseCloud->width,noiseCloud->height,noiseCloud->data.at(end+1),data);
			textureFactory.expotentialFilter(noiseCloud->width,noiseCloud->height,data,cover,sharpness);
			
			GLuint texture = textureFactory.buildTexture(noiseCloud->width,noiseCloud->height,alphaThreshold,data);
			delete data;
			return texture;
		}
		else
		{
			for(int i = 0; i < noiseCloud->width; ++i)
				for(int j = 0; j < noiseCloud->height; ++j)
					noiseCloud->data.at(end+1)[i*noiseCloud->width+j] = (1.0f - a) * noiseCloud->data.at(begin)[i*noiseCloud->width+j] + a * noiseCloud->data.at(end)[i*noiseCloud->width+j];
		}

		return textureFactory.buildTexture(noiseCloud->width,noiseCloud->height,alphaThreshold,noiseCloud->data.at(end+1));
	}

	void CloudFactory::configureExpParameters(const float cover, const float sharpness)
	{
		this->cover = cover;
		this->sharpness = sharpness;
	}

	void CloudFactory::configureNoise(const float persistance, const int octaveAmount, const int seed, const float zoom, const float initialFrequency, const float initialAmplitude)
	{
		textureFactory.getNoiseGenerator()->configurePerlinNoise(persistance,octaveAmount,seed,zoom,initialFrequency,initialAmplitude);
	}
}
