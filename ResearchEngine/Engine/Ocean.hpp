/* 
* Based on Eric Bruneton code for article 
* Real-time Realistic Ocean Lighting using Seamless Transitions from Geometry to BRDF
* Copyright (c) 2009 INRIA
* All rights reserved.
*/

// WAVES SPECTRUM
// using "A unified directional spectrum for long and short wind-driven waves"
// T. Elfouhaily, B. Chapron, K. Katsaros, D. Vandemark
// Journal of Geophysical Research vol 102, p781-796, 1997

#ifndef OCEAN_HPP
#define OCEAN_HPP

#define M_PI 3.14159265
#define IRRADIANCE_UNIT 0
#define INSCATTER_UNIT 1
#define TRANSMITTANCE_UNIT 2
#define SKY_UNIT 3
#define NOISE_UNIT 4
#define SPECTRUM_1_2_UNIT 5
#define SPECTRUM_3_4_UNIT 6
#define SLOPE_VARIANCE_UNIT 7
#define FFT_A_UNIT 8
#define FFT_B_UNIT 9
#define BUTTERFLY_UNIT 10

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>

#include "ResearchEngine.hpp"

#include "vec4.h"
#include "mat4.h"
#include "Program.h"

class Ocean
{
private:
	Program *render;
	Program *sky;
	Program *skymap;
	Program *init;
	Program *variances;
	Program *fftx;
	Program *ffty;

	GLuint skyTex;
	GLuint fbo;
	GLuint vbo;
	GLuint vboIndices;
	GLuint transmittanceTex;
	GLuint irradianceTex;
	GLuint inscatterTex;
	GLuint spectrum12Tex;
	GLuint spectrum34Tex;
	GLuint fftaTex;
	GLuint fftbTex;
	GLuint butterflyTex;
	GLuint fftFbo1;
	GLuint fftFbo2;
	GLuint variancesFbo;
	GLuint slopeVarianceTex;

	unsigned int skyTexSize;
	int width;
	int height;
	vec4f vboParams;
	int vboSize;
	float sunTheta;
	float sunPhi;
	float gridSize;
	float seaColor[4];
	float hdrExposure;
	bool seaContrib;
	bool sunContrib;
	bool skyContrib;
	bool manualFilter;
	bool choppy;
	int N_SLOPE_VARIANCE;
	float GRID1_SIZE;
	float GRID2_SIZE;
	float GRID3_SIZE;
	float GRID4_SIZE;
	float WIND;
	float OMEGA;
	float A;
	float cm;
	float km;
	int PASSES;
	int FFT_SIZE;
	float* spectrum12;
	float* spectrum34;

	float sqr(float x);
	float omega(float k);
	float spectrum(float kx, float ky, bool omnispectrum = false);

	void drawQuad();
	void loadPrograms();
	void generateMesh();

	long lrandom(long *seed);
	float frandom(long *seed);
	float grandom(float mean, float stdDeviation, long *seed);
	void getSpectrumSample(int i, int j, float lengthScale, float kMin, float *result);
	void generateWavesSpectrum();
	float getSlopeVariance(float kx, float ky, float *spectrumSample);
	void computeSlopeVarianceTex(void *unused);
	int bitReverse(int i, int N);
	void computeWeight(int N, int k, float &Wr, float &Wi);
	float* computeButterflyLookupTexture();
	void simulateFFTWaves(float t);

public:

	float cameraHeight;
	float cameraTheta;
	float cameraPhi;

	float cameraX;
	float cameraY;
	float cameraZ;

	Ocean();
	~Ocean();

	void initialize();
	void update(const float elapsedTime);
	void preRender();
	void renderOcean();

	void setSunPosition(const float x, const float y);
};
#endif
