/*
 * Projekt C-Way
 * Efekt nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef PROCEDURALTEXTUREFACOTRY_HPP
#define PROCEDURALTEXTUREFACTORY_HPP

#include <GL/glew.h>
#include <math.h>

#include "NoiseGenerator.hpp"

//TODO testz pytanie co trwa d³u¿ej - nowa tekstura, czy update tekstury -> zale¿noœci od platformy/obie opcje

//TODO CW texture creation

namespace CW
{
	class ProceduralTextureFactory
	{
	private:
		NoiseGenerator<float> noiseGenerator;

		GLuint createTexture(const int width, const int height, GLenum target, GLint format, GLenum type, unsigned char* data);
	public:
		GLuint createNoiseTexture(const int width, const int height, const int alphaThreshold);
		GLuint createCloudTexture(const int width, const int height, const int alphaThreshold);
		GLuint createDifferenceCloudTexture(const int width, const int height, const int alphaThreshold);
		GLuint createExpotentialCloudTexture(const int width, const int height, const int alphaThreshold, const float cover, const float sharpness);

		void setNoise(const int width, const int height, float* data);
		void overlapOctaves(const int width, const int height, float* block, float  *data);
		float interpolate(const int width, const int height, const float x, const float y, float* data);
		void expotentialFilter(const int width, const int height, float* data, const float cover, const float sharpness);

		GLuint buildTexture(const int width, const int height, const int alphaThreshold, float* data);
		NoiseGenerator<float>* getNoiseGenerator();
	};
}
#endif