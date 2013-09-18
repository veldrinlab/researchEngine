/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "ProceduralTextureFactory.hpp"

namespace CW
{
	GLuint ProceduralTextureFactory::createNoiseTexture(const int width, const int height, const int alphaThreshold)
	{
		float* data = new float[width*height];
	
		for(int x = 0; x < width; ++x) 
			for(int y = 0; y < height; ++y)
				data[x*width+y] = (128.0f*noiseGenerator.whiteNoise())+128.0f;
	
		GLubyte* texture = new GLubyte[width*height*4];

		for(int x = 0; x < width; ++x)
			for(int y = 0; y < height; ++y) 
			{
				int pointer = 4*x+width*y*4;
				GLubyte color = static_cast<GLubyte>(data[x*width+y]); 

				texture[pointer] = color;
				texture[pointer+1] = color;
				texture[pointer+2] = color;
				texture[pointer+3] = color;
			
				if(color < alphaThreshold)
					texture[pointer+3] = 0;
			}

			GLuint textureObject = createTexture(width,height,GL_TEXTURE_2D,GL_RGBA,GL_UNSIGNED_BYTE,texture); 

			delete data;
			delete texture;

			return textureObject;
	}

	GLuint ProceduralTextureFactory::createCloudTexture(const int width, const int height, const int alphaThreshold)
	{
		float* data = new float[width*height];
	
		for(int x = 0; x < width; ++x) 
			for(int y = 0; y < height; ++y)
				data[x*width+y] = (128.0f*noiseGenerator.perlinNoise2D(x,y))+128.0f;
	
		GLubyte* texture = new GLubyte[width*height*4];

		for(int x = 0; x < width; ++x)
			for(int y = 0; y < height; ++y) 
			{
				int pointer = 4*x+width*y*4;
				GLubyte color = static_cast<GLubyte>(data[x*width+y]);  

				texture[pointer] = color;
				texture[pointer+1] = color;
				texture[pointer+2] = color;
				texture[pointer+3] = color;

				if(color < alphaThreshold)
					texture[pointer+3] = 0;
			}

			GLuint textureObject = createTexture(width,height,GL_TEXTURE_2D,GL_RGBA,GL_UNSIGNED_BYTE,texture); 

			delete data;
			delete texture;

			return textureObject;
	}

	GLuint ProceduralTextureFactory::createDifferenceCloudTexture(const int width, const int height, const int alphaThreshold)
	{
		float* data = new float[width*height];
	
		for(int x = 0; x < width; ++x) 
			for(int y = 0; y < height; ++y)
				data[x*width+y] = (128.0f*noiseGenerator.perlinNoise2D(x,y))+128.0f;
	
		noiseGenerator.chnageSeed();
	
		for(int x = 0; x < width; ++x) 
			for(int y = 0; y < height; ++y)
			{
					float value = (128.0f*noiseGenerator.perlinNoise2D(x,y))+128.0f;
					value -= data[x*width+y];
				
					if(value > 255.0f)
						value = 255.0f;
					else if(value < 0.0f)
						value = 0.0f;

					data[x*width+y] = value;
			}
	
		GLubyte* texture = new GLubyte[width*height*4];

		for(int x = 0; x < width; ++x)
			for(int y = 0; y < height; ++y) 
			{
				int pointer = 4*x+width*y*4;
				GLubyte color = static_cast<GLubyte>(data[x*width+y]); 

				texture[pointer] = color;
				texture[pointer+1] = color;
				texture[pointer+2] = color;
				texture[pointer+3] = color;
			
				if(color < alphaThreshold)
					texture[pointer+3] = 0;
			}

			GLuint textureObject = createTexture(width,height,GL_TEXTURE_2D,GL_RGBA,GL_UNSIGNED_BYTE,texture); 

			delete data;
			delete texture;

			return textureObject;
	}

	GLuint ProceduralTextureFactory::createExpotentialCloudTexture(const int width, const int height, const int alphaThreshold, const float cover, const float sharpness)
	{
		float* data = new float[width*height];
		float* block = new float[width*height/64];

		setNoise(width,height,block);
		overlapOctaves(width,height,block,data);
		expotentialFilter(width,height,data,cover,sharpness);

		GLubyte* texture = new GLubyte[width*height*4];

		for(int x = 0; x < width; ++x)
			for(int y = 0; y < height; ++y) 
			{
				int pointer = 4*x+width*y*4;
				GLubyte color = static_cast<GLubyte>(data[x*width+y]); 

				texture[pointer] = color;
				texture[pointer+1] = color;
				texture[pointer+2] = color;
				texture[pointer+3] = color;
			
				if(color < alphaThreshold)
					texture[pointer+3] = 0;
			}

			GLuint textureObject = createTexture(width,height,GL_TEXTURE_2D,GL_RGBA,GL_UNSIGNED_BYTE,texture); 

		delete data;
		delete block;
		delete texture;
		return textureObject;
	}

	void ProceduralTextureFactory::setNoise(const int width, const int height, float* data)
	{
		int tempWidth = width/8;
		int tempHeight = height/8;

		float* temp = new float[(tempWidth+2)*(tempHeight+2)];

		for(int x = 1; x < tempWidth+1; ++x) 
			for (int y = 1; y < tempHeight+1; ++y)
				temp[x*(tempWidth+2)+y] = 128.0f + noiseGenerator.noise2D(x,y) * 128.0f;
		
		for(int x = 1; x < tempWidth+1; ++x)
		{
			temp[x] = temp[tempWidth*(tempWidth+2)+x];
			temp[(tempWidth+1)*(tempWidth+2)+x] = temp[tempWidth+2+x];
			temp[x*(tempWidth+2)] = temp[x*(tempWidth+2)+tempWidth];
			temp[x*(tempWidth+2)+tempWidth+1] = temp[x*(tempWidth+2)+1];
		}

		temp[0] = temp[tempWidth*(tempWidth+2)+tempWidth];
		temp[(tempWidth+1)*(tempWidth+2)+tempWidth+1] = temp[tempWidth+2+1];
		temp[tempWidth+1] = temp[tempWidth*(tempWidth+2)+1];
		temp[(tempWidth+1)*(tempWidth+2)] = temp[tempWidth+2+tempWidth];

		for(int x = 1; x < tempWidth+1; ++x)
			for(int y = 1; y < tempHeight+1; ++y)
			{
				float center = temp[x*(tempWidth+2)+y]/4.0f;
				float sides = (temp[(x+1)*(tempWidth+2)+y] + temp[(x-1)*(tempWidth+2)+y] + temp[x*(tempWidth+2)+y+1] + temp[x*(tempWidth+2)+y-1])/8.0f;
				float corners = (temp[(x+1)*(tempWidth+2)+y+1] + temp[(x+1)*(tempWidth+2)+y-1] + temp[(x-1)*(tempWidth+2)+y+1] + temp[(x-1)*(tempWidth+2)+y-1])/16.0f;

				data[((x-1)*(tempWidth)) + (y-1)] = center + sides + corners;
			}

		delete temp;
	}

	void ProceduralTextureFactory::overlapOctaves(const int width, const int height, float* block, float  *data)
	{
		for(int x = 0; x < width * height; ++x)
			data[x] = 0.0f;
		 
		int octaves = 4;
	
		for(int octave = 0; octave < octaves; ++octave)
			for(int x = 0; x < width; ++x)
				for(int y = 0; y < height; ++y)
				{
					float scale = 1 / powf(2,3.0f-octave);
					float noise = interpolate(width,height,x*scale,y*scale,block);
					data[(x*width) + y] += noise / powf(2,static_cast<float>(octave));
				}
	}

	float ProceduralTextureFactory::interpolate(const int width, const int height, const float x, const float y, float* data)
	{
		int tempWidth = width/8;
		int tempHeight = height/8;

		int integerX = static_cast<int>(x);
		int integerY = static_cast<int>(y);
	
		float fractionalX = x - integerX;
		float fractionalY = y - integerY;

		int X0 = integerX % tempWidth;
		int Y0 = integerY % tempHeight;
		int X1 = (integerX + 1) % tempWidth;
		int Y1 = (integerY + 1) % tempHeight;

		float bot = data[X0*tempWidth + Y0] + fractionalX * (data[X1*tempWidth + Y0] - data[X0*tempWidth + Y0]);
		float top = data[X0*tempWidth + Y1] + fractionalX * (data[X1*tempWidth +  Y1] - data[X0*tempWidth + Y1]);

		return (bot + fractionalY * (top - bot));
	}

	void ProceduralTextureFactory::expotentialFilter(const int width, const int height, float* data, const float cover, const float sharpness)
	{
		for(int x = 0; x < width * height; ++x)
		{
			float c = data[x] - (255.0f-cover);
			if(c < 0)
				c = 0.0f;
			data[x] = 255.0f - (static_cast<float>(pow(sharpness, c))*255.0f);
		}
	}

	GLuint ProceduralTextureFactory::buildTexture(const int width, const int height, const int alphaThreshold, float* data)
	{
		GLubyte* texture = new GLubyte[width*height*4];

		for(int x = 0; x < width; ++x)
			for(int y = 0; y < height; ++y) 
			{
				int pointer = 4*x+width*y*4;
				GLubyte color = static_cast<GLubyte>(data[x*width+y]); 

				texture[pointer] = color;
				texture[pointer+1] = color;
				texture[pointer+2] = color;
				texture[pointer+3] = color;
			
				if(color < alphaThreshold)
					texture[pointer+3] = 0;
			}

			GLuint textureObject = createTexture(width,height,GL_TEXTURE_2D,GL_RGBA,GL_UNSIGNED_BYTE,texture); 

			delete texture;
			return textureObject;
	}

	NoiseGenerator<float>* ProceduralTextureFactory::getNoiseGenerator()
	{
		return &noiseGenerator;
	}

	GLuint ProceduralTextureFactory::createTexture(const int width, const int height, GLenum target, GLint format, GLenum type, unsigned char* data)
	{
		GLuint id;
	
		glGenTextures(1, &id); 
		glBindTexture(target, id);

		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
		glTexImage2D(target, 0, format, width, height, 0, format, type, data);

		return id;
	}
}
