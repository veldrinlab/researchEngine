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

#include "Ocean.hpp"

using namespace AyumiEngine::AyumiMath;

Ocean::Ocean()
{
	width = 800;
	height = 480;

	render = nullptr;
	sky = nullptr;
	skymap = nullptr;
	init = nullptr;
	variances = nullptr;
	fftx = nullptr;
	ffty = nullptr;

	skyTexSize = 512;
	vboSize = 0;
	sunTheta = M_PI / 2.0 - 0.05; // 0.5f
	sunPhi = 0.0;
	cameraHeight = 2.0;
	cameraTheta = 0.0;
	cameraPhi = 0.0;
	cameraX = cameraY = cameraZ = 0.0f;
	gridSize = 8.0;
	seaColor[0] = 10.0 / 255.0;
	seaColor[1] = 40.0 / 255.0;
	seaColor[2] = 120.0 / 255.0;
	seaColor[3] = 0.1;

	hdrExposure = 0.4;
	seaContrib = true;
	sunContrib = true;
	skyContrib = true;
	manualFilter = false;
	choppy = true;
	N_SLOPE_VARIANCE = 10; // size of the 3d texture containing precomputed filtered slope variances
	GRID1_SIZE = 5488.0; // size in meters (i.e. in spatial domain) of the first grid
	GRID2_SIZE = 392.0; // size in meters (i.e. in spatial domain) of the second grid
	GRID3_SIZE = 28.0; // size in meters (i.e. in spatial domain) of the third grid
	GRID4_SIZE = 2.0; // size in meters (i.e. in spatial domain) of the fourth grid
	WIND = 5.0; // wind speed in meters per second (at 10m above surface)
	OMEGA = 0.84; // sea state (inverse wave age)
	A = 1.0; // wave amplitude factor (should be one)
	cm = 0.23; // Eq 59
	km = 370.0; // Eq 59

	PASSES = 8; 
	FFT_SIZE = 1 << PASSES;
	spectrum12 = nullptr;
	spectrum34 = nullptr;
}

Ocean::~Ocean()
{

}

void Ocean::initialize()
{
	float *data = new float[16*64*3];
    FILE *f = fopen("data/irradiance.raw", "rb");
    fread(data, 1, 16*64*3*sizeof(float), f);
    fclose(f);
    glActiveTexture(GL_TEXTURE0 + IRRADIANCE_UNIT);
    glGenTextures(1, &irradianceTex);
    glBindTexture(GL_TEXTURE_2D, irradianceTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, 64, 16, 0, GL_RGB, GL_FLOAT, data);
    delete[] data;

    int res = 64;
    int nr = res / 2;
    int nv = res * 2;
    int nb = res / 2;
    int na = 8;
    f = fopen("data/inscatter.raw", "rb");
    data = new float[nr*nv*nb*na*4];
    fread(data, 1, nr*nv*nb*na*4*sizeof(float), f);
    fclose(f);
    glActiveTexture(GL_TEXTURE0 + INSCATTER_UNIT);
    glGenTextures(1, &inscatterTex);
    glBindTexture(GL_TEXTURE_3D, inscatterTex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F_ARB, na*nb, nv, nr, 0, GL_RGBA, GL_FLOAT, data);
    delete[] data;

    data = new float[256*64*3];
    f = fopen("data/transmittance.raw", "rb");
    fread(data, 1, 256*64*3*sizeof(float), f);
    fclose(f);
    glActiveTexture(GL_TEXTURE0 + TRANSMITTANCE_UNIT);
    glGenTextures(1, &transmittanceTex);
    glBindTexture(GL_TEXTURE_2D, transmittanceTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F_ARB, 256, 64, 0, GL_RGB, GL_FLOAT, data);
    delete[] data;

    float maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

    glActiveTexture(GL_TEXTURE0 + SKY_UNIT);
    glGenTextures(1, &skyTex);
    glBindTexture(GL_TEXTURE_2D, skyTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, skyTexSize, skyTexSize, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmapEXT(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0 + SPECTRUM_1_2_UNIT);
    glGenTextures(1, &spectrum12Tex);
    glBindTexture(GL_TEXTURE_2D, spectrum12Tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + SPECTRUM_3_4_UNIT);
    glGenTextures(1, &spectrum34Tex);
    glBindTexture(GL_TEXTURE_2D, spectrum34Tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 0, GL_RGB, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + SLOPE_VARIANCE_UNIT);
    glGenTextures(1, &slopeVarianceTex);
    glBindTexture(GL_TEXTURE_3D, slopeVarianceTex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA16F_ARB, N_SLOPE_VARIANCE, N_SLOPE_VARIANCE, N_SLOPE_VARIANCE, 0, GL_LUMINANCE_ALPHA, GL_FLOAT, NULL);

    glActiveTexture(GL_TEXTURE0 + FFT_A_UNIT);
    glGenTextures(1, &fftaTex);
    glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, fftaTex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 5, 0, GL_RGB, GL_FLOAT, NULL);
    glGenerateMipmapEXT(GL_TEXTURE_2D_ARRAY_EXT);

    glActiveTexture(GL_TEXTURE0 + FFT_B_UNIT);
    glGenTextures(1, &fftbTex);
    glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, fftbTex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 5, 0, GL_RGB, GL_FLOAT, NULL);
    glGenerateMipmapEXT(GL_TEXTURE_2D_ARRAY_EXT);

    glActiveTexture(GL_TEXTURE0 + BUTTERFLY_UNIT);
    glGenTextures(1, &butterflyTex);
    glBindTexture(GL_TEXTURE_2D, butterflyTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    data = computeButterflyLookupTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, FFT_SIZE, PASSES, 0, GL_RGBA, GL_FLOAT, data);
    delete[] data;

	
    generateWavesSpectrum();

    glGenFramebuffersEXT(1, &variancesFbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, variancesFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	
    glGenFramebuffersEXT(1, &fftFbo1);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fftFbo1);
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    GLenum drawBuffers[5] = {
        GL_COLOR_ATTACHMENT0_EXT,
        GL_COLOR_ATTACHMENT1_EXT,
        GL_COLOR_ATTACHMENT2_EXT,
        GL_COLOR_ATTACHMENT3_EXT,
        GL_COLOR_ATTACHMENT4_EXT
    };
    glDrawBuffers(5, drawBuffers);
    for (int i = 0; i < 5; ++i) {
        glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, fftaTex, 0, i);
    }
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    glGenFramebuffersEXT(1, &fftFbo2);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fftFbo2);
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, fftaTex, 0);
    glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, fftbTex, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	
    glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    generateMesh();
    
	loadPrograms();
    computeSlopeVarianceTex(NULL);
}

void Ocean::update(const float elapsedTime)
{
	simulateFFTWaves(elapsedTime);
}

void Ocean::preRender()
{
	vec4f sun = vec4f(sin(sunTheta) * cos(sunPhi), sin(sunTheta) * sin(sunPhi), cos(sunTheta), 0.0);

    glDisable(GL_DEPTH_TEST);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, skyTex, 0);
    glViewport(0, 0, skyTexSize, skyTexSize);
    glUseProgram(skymap->program);
    glUniform3f(glGetUniformLocation(skymap->program, "sunDir"), sun.x, sun.y, sun.z);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(-1, 1);
    glVertex2f(1, 1);
    glEnd();
    glActiveTexture(GL_TEXTURE0 + SKY_UNIT);
    glGenerateMipmapEXT(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
}

void Ocean::renderOcean()
{
	//TODO camera poprawna

	if(vboParams.w != cameraTheta)
        generateMesh();
  
	vec4f sun = vec4f(sin(sunTheta) * cos(sunPhi), sin(sunTheta) * sin(sunPhi), cos(sunTheta), 0.0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glViewport(0, 0, width, height);

    float ch = cameraHeight;

    mat4f view = mat4f(
        0.0, -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -ch,
        -1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    view = mat4f::rotatey(cameraPhi) * view;
    view = mat4f::rotatex(cameraTheta) * view;

    mat4f proj = mat4f::perspectiveProjection(90.0, float(width) / float(height), 0.1, 1000000.0);

    glUseProgram(render->program);
    glUniformMatrix4fv(glGetUniformLocation(render->program, "screenToCamera"), 1, true, proj.inverse().coefficients());
    glUniformMatrix4fv(glGetUniformLocation(render->program, "cameraToWorld"), 1, true, view.inverse().coefficients());
    glUniformMatrix4fv(glGetUniformLocation(render->program, "worldToScreen"), 1, true, (proj * view).coefficients());
    glUniform3f(glGetUniformLocation(render->program, "worldCamera"), 0.0, 0.0, ch);
    glUniform3f(glGetUniformLocation(render->program, "worldSunDir"), sun.x, sun.y, sun.z);
    glUniform1f(glGetUniformLocation(render->program, "hdrExposure"), hdrExposure);
    glUniform3f(glGetUniformLocation(render->program, "seaColor"), seaColor[0] * seaColor[3], seaColor[1] * seaColor[3], seaColor[2] * seaColor[3]);
    glUniform1i(glGetUniformLocation(render->program, "spectrum_1_2_Sampler"), SPECTRUM_1_2_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "spectrum_3_4_Sampler"), SPECTRUM_3_4_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "fftWavesSampler"), FFT_A_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "slopeVarianceSampler"), SLOPE_VARIANCE_UNIT);
    glUniform4f(glGetUniformLocation(render->program, "GRID_SIZES"), GRID1_SIZE, GRID2_SIZE, GRID3_SIZE, GRID4_SIZE);
    glUniform2f(glGetUniformLocation(render->program, "gridSize"), gridSize / float(width), gridSize / float(height));
    glUniform1f(glGetUniformLocation(render->program, "choppy"), choppy);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glVertexPointer(4, GL_FLOAT, 16, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(GL_TRIANGLES, vboSize, GL_UNSIGNED_INT, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Ocean::setSunPosition(const float x, const float y)
{
	sunPhi = x;
	sunTheta = y;
}

float Ocean::sqr(float x)
{
    return x * x;
}

float Ocean::omega(float k)
{
    return sqrt(9.81 * k * (1.0 + sqr(k / km))); // Eq 24
}

// 1/kx and 1/ky in meters
float Ocean::spectrum(float kx, float ky, bool omnispectrum)
{
    float U10 = WIND;
    float Omega = OMEGA;

    // phase speed
    float k = sqrt(kx * kx + ky * ky);
    float c = omega(k) / k;

    // spectral peak
    float kp = 9.81 * sqr(Omega / U10); // after Eq 3
    float cp = omega(kp) / kp;

    // friction velocity
    float z0 = 3.7e-5 * sqr(U10) / 9.81 * pow(U10 / cp, 0.9f); // Eq 66
    float u_star = 0.41 * U10 / log(10.0 / z0); // Eq 60

    float Lpm = exp(- 5.0 / 4.0 * sqr(kp / k)); // after Eq 3
    float gamma = Omega < 1.0 ? 1.7 : 1.7 + 6.0 * log(Omega); // after Eq 3 // log10 or log??
    float sigma = 0.08 * (1.0 + 4.0 / pow(Omega, 3.0f)); // after Eq 3
    float Gamma = exp(-1.0 / (2.0 * sqr(sigma)) * sqr(sqrt(k / kp) - 1.0));
    float Jp = pow(gamma, Gamma); // Eq 3
    float Fp = Lpm * Jp * exp(- Omega / sqrt(10.0) * (sqrt(k / kp) - 1.0)); // Eq 32
    float alphap = 0.006 * sqrt(Omega); // Eq 34
    float Bl = 0.5 * alphap * cp / c * Fp; // Eq 31

    float alpham = 0.01 * (u_star < cm ? 1.0 + log(u_star / cm) : 1.0 + 3.0 * log(u_star / cm)); // Eq 44
    float Fm = exp(-0.25 * sqr(k / km - 1.0)); // Eq 41
    float Bh = 0.5 * alpham * cm / c * Fm * Lpm; // Eq 40 (fixed)

    if (omnispectrum) {
        return A * (Bl + Bh) / (k * sqr(k)); // Eq 30
    }

    float a0 = log(2.0) / 4.0; float ap = 4.0; float am = 0.13 * u_star / cm; // Eq 59
    float Delta = tanh(a0 + ap * pow(c / cp, 2.5f) + am * pow(cm / c, 2.5f)); // Eq 57

    float phi = atan2(ky, kx);

    if (kx < 0.0) {
        return 0.0;
    } else {
        Bl *= 2.0;
        Bh *= 2.0;
    }

    return A * (Bl + Bh) * (1.0 + Delta * cos(2.0 * phi)) / (2.0 * M_PI * sqr(sqr(k))); // Eq 67
}

void Ocean::drawQuad()
{
    glBegin(GL_TRIANGLE_STRIP);
    glVertex4f(-1.0, -1.0, 0.0, 0.0);
    glVertex4f(+1.0, -1.0, 1.0, 0.0);
    glVertex4f(-1.0, +1.0, 0.0, 1.0);
    glVertex4f(+1.0, +1.0, 1.0, 1.0);
    glEnd();
}

// ----------------------------------------------------------------------------
// PROGRAM RELOAD
// ----------------------------------------------------------------------------

void Ocean::loadPrograms()
{
    char* files[2];
    char options[512];
    files[0] = "Shader/atmosphere.glsl";
    files[1] = "Shader/ocean.glsl";
    sprintf(options, "#define %sSEA_CONTRIB\n#define %sSUN_CONTRIB\n#define %sSKY_CONTRIB\n#define %sCLOUDS\n#define %sHARDWARE_ANISTROPIC_FILTERING\n",
        seaContrib ? "" : "NO_", sunContrib ? "" : "NO_", skyContrib ? "" : "NO_", false ? "" : "NO_", manualFilter ? "NO_" : "");

    if (render != NULL) {
        delete render;
    }
	render = new Program(2, files, options);
    glUseProgram(render->program);
    glUniform1i(glGetUniformLocation(render->program, "skyIrradianceSampler"), IRRADIANCE_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "inscatterSampler"), INSCATTER_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "transmittanceSampler"), TRANSMITTANCE_UNIT);
    glUniform1i(glGetUniformLocation(render->program, "skySampler"), SKY_UNIT);

    files[0] = "Shader/atmosphere.glsl";
    files[1] = "Shader/sky.glsl";
    if (sky != NULL) {
        delete sky;
    }
	sky = new Program(2, files, options);
    glUseProgram(sky->program);
    glUniform1i(glGetUniformLocation(sky->program, "skyIrradianceSampler"), IRRADIANCE_UNIT);
    glUniform1i(glGetUniformLocation(sky->program, "inscatterSampler"), INSCATTER_UNIT);
    glUniform1i(glGetUniformLocation(sky->program, "transmittanceSampler"), TRANSMITTANCE_UNIT);
    glUniform1i(glGetUniformLocation(sky->program, "skySampler"), SKY_UNIT);

    files[0] = "Shader/atmosphere.glsl";
    files[1] = "Shader/skymap.glsl";
    if (skymap != NULL) {
        delete skymap;
    }
	skymap = new Program(2, files, options);
    glUseProgram(skymap->program);
    glUniform1i(glGetUniformLocation(skymap->program, "skyIrradianceSampler"), IRRADIANCE_UNIT);
    glUniform1i(glGetUniformLocation(skymap->program, "inscatterSampler"), INSCATTER_UNIT);
    glUniform1i(glGetUniformLocation(skymap->program, "transmittanceSampler"), TRANSMITTANCE_UNIT);
    glUniform1i(glGetUniformLocation(skymap->program, "noiseSampler"), NOISE_UNIT);

    files[0] = "Shader/init.glsl";
    if (init != NULL) {
        delete init;
    }
    init = new Program(1, files);
    glUseProgram(init->program);
    glUniform1i(glGetUniformLocation(init->program, "spectrum_1_2_Sampler"), SPECTRUM_1_2_UNIT);
    glUniform1i(glGetUniformLocation(init->program, "spectrum_3_4_Sampler"), SPECTRUM_3_4_UNIT);

    files[0] = "Shader/variances.glsl";
    if (variances != NULL) {
        delete variances;
    }
    variances = new Program(1, files);
    glUseProgram(variances->program);
    glUniform1f(glGetUniformLocation(variances->program, "N_SLOPE_VARIANCE"), N_SLOPE_VARIANCE);
    glUniform1i(glGetUniformLocation(variances->program, "spectrum_1_2_Sampler"), SPECTRUM_1_2_UNIT);
    glUniform1i(glGetUniformLocation(variances->program, "spectrum_3_4_Sampler"), SPECTRUM_3_4_UNIT);
    glUniform1i(glGetUniformLocation(variances->program, "FFT_SIZE"), FFT_SIZE);

    files[0] = "Shader/fftx.glsl";
    if (fftx != NULL) {
        delete fftx;
    }
    fftx = new Program(1, files);
    glUseProgram(fftx->program);
    glUniform1i(glGetUniformLocation(fftx->program, "butterflySampler"), BUTTERFLY_UNIT);

    files[0] = "Shader/ffty.glsl";
    if (ffty != NULL) {
        delete ffty;
    }
    ffty = new Program(1, files);
    glUseProgram(ffty->program);
    glUniform1i(glGetUniformLocation(ffty->program, "butterflySampler"), BUTTERFLY_UNIT);
}

void Ocean::generateMesh()
{
    float horizon = tan(cameraTheta / 180.0 * M_PI);
    float s = std::min(1.1f, 0.5f + horizon * 0.5f);

    float vmargin = 0.1;
    float hmargin = 0.1;

	int valueA = int(ceil(height * (s + vmargin) / gridSize) + 5) * int(ceil(width * (1.0 + 2.0 * hmargin) / gridSize) + 5);
	int valueB = 6 * int(ceil(height * (s + vmargin) / gridSize) + 4) * int(ceil(width * (1.0 + 2.0 * hmargin) / gridSize) + 4);

	if(valueA < 0 || valueB < 0)
		return;

    if (vboSize != 0) {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vboIndices);
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);



    vboParams = vec4f(width, height, gridSize, cameraTheta);
    vec4f *data = new vec4f[valueA];

    int n = 0;
    int nx = 0;
    for (float j = height * s - 0.1; j > -height * vmargin - gridSize; j -= gridSize) {
        nx = 0;
        for (float i = -width * hmargin; i < width * (1.0 + hmargin) + gridSize; i += gridSize) {
            data[n++] = vec4f(-1.0 + 2.0 * i / width, -1.0 + 2.0 * j / height, 0.0, 1.0);
            nx++;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, n * 16, data, GL_STATIC_DRAW);
    delete[] data;

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ARRAY_BUFFER, vboIndices);

    vboSize = 0;
    GLuint *indices = new GLuint[valueB];

    int nj = 0;
    for (float j = height * s - 0.1; j > -height * vmargin; j -= gridSize) {
        int ni = 0;
        for (float i = -width * hmargin; i < width * (1.0 + hmargin); i += gridSize) {
            indices[vboSize++] = ni + (nj + 1) * nx;
            indices[vboSize++] = (ni + 1) + (nj + 1) * nx;
            indices[vboSize++] = (ni + 1) + nj * nx;
            indices[vboSize++] = (ni + 1) + nj * nx;
            indices[vboSize++] = ni + (nj + 1) * nx;
            indices[vboSize++] = ni + nj * nx;
            ni++;
        }
        nj++;
    }

    glBufferData(GL_ARRAY_BUFFER, vboSize * 4, indices, GL_STATIC_DRAW);
    delete[] indices;
}

long Ocean::lrandom(long *seed)
{
    *seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return *seed;
}

float Ocean::frandom(long *seed)
{
    long r = lrandom(seed) >> (31 - 24);
    return r / (float)(1 << 24);
}

float Ocean::grandom(float mean, float stdDeviation, long *seed)
{
    float x1, x2, w, y1;
    static float y2;
    static int use_last = 0;

    if (use_last) {
        y1 = y2;
        use_last = 0;
    } else {
        do {
            x1 = 2.0f * frandom(seed) - 1.0f;
            x2 = 2.0f * frandom(seed) - 1.0f;
            w  = x1 * x1 + x2 * x2;
        } while (w >= 1.0f);
        w  = sqrt((-2.0f * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }
	return mean + y1 * stdDeviation;
}

void Ocean::getSpectrumSample(int i, int j, float lengthScale, float kMin, float *result)
{
    static long seed = 1234;
    float dk = 2.0 * M_PI / lengthScale;
    float kx = i * dk;
    float ky = j * dk;
    if (abs(kx) < kMin && abs(ky) < kMin) {
        result[0] = 0.0;
        result[1] = 0.0;
    } else {
        float S = spectrum(kx, ky);
        float h = sqrt(S / 2.0) * dk;
        float phi = frandom(&seed) * 2.0 * M_PI;
        result[0] = h * cos(phi);
        result[1] = h * sin(phi);
    }
}

// generates the waves spectrum
void Ocean::generateWavesSpectrum()
{
    if (spectrum12 != NULL) {
        delete[] spectrum12;
        delete[] spectrum34;
    }
    spectrum12 = new float[FFT_SIZE * FFT_SIZE * 4];
    spectrum34 = new float[FFT_SIZE * FFT_SIZE * 4];

    for (int y = 0; y < FFT_SIZE; ++y) {
        for (int x = 0; x < FFT_SIZE; ++x) {
            int offset = 4 * (x + y * FFT_SIZE);
            int i = x >= FFT_SIZE / 2 ? x - FFT_SIZE : x;
            int j = y >= FFT_SIZE / 2 ? y - FFT_SIZE : y;
            getSpectrumSample(i, j, GRID1_SIZE, M_PI / GRID1_SIZE, spectrum12 + offset);
            getSpectrumSample(i, j, GRID2_SIZE, M_PI * FFT_SIZE / GRID1_SIZE, spectrum12 + offset + 2);
            getSpectrumSample(i, j, GRID3_SIZE, M_PI * FFT_SIZE / GRID2_SIZE, spectrum34 + offset);
            getSpectrumSample(i, j, GRID4_SIZE, M_PI * FFT_SIZE / GRID3_SIZE, spectrum34 + offset + 2);
        }
    }

    glActiveTexture(GL_TEXTURE0 + SPECTRUM_1_2_UNIT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 0, GL_RGBA, GL_FLOAT, spectrum12);
    glActiveTexture(GL_TEXTURE0 + SPECTRUM_3_4_UNIT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, FFT_SIZE, FFT_SIZE, 0, GL_RGBA, GL_FLOAT, spectrum34);
}

float Ocean::getSlopeVariance(float kx, float ky, float *spectrumSample)
{
    float kSquare = kx * kx + ky * ky;
    float real = spectrumSample[0];
    float img = spectrumSample[1];
    float hSquare = real * real + img * img;
    return kSquare * hSquare * 2.0;
}

// precomputes filtered slope variances in a 3d texture, based on the wave spectrum
void Ocean::computeSlopeVarianceTex(void *unused)
{
    // slope variance due to all waves, by integrating over the full spectrum
    float theoreticSlopeVariance = 0.0;
    float k = 5e-3;
    while (k < 1e3) {
        float nextK = k * 1.001;
        theoreticSlopeVariance += k * k * spectrum(k, 0, true) * (nextK - k);
        k = nextK;
    }

    // slope variance due to waves, by integrating over the spectrum part
    // that is covered by the four nested grids. This can give a smaller result
    // than the theoretic total slope variance, because the higher frequencies
    // may not be covered by the four nested grid. Hence the difference between
    // the two is added as a "delta" slope variance in the "variances" shader,
    // to be sure not to lose the variance due to missing wave frequencies in
    // the four nested grids
    float totalSlopeVariance = 0.0;
    for (int y = 0; y < FFT_SIZE; ++y) {
        for (int x = 0; x < FFT_SIZE; ++x) {
            int offset = 4 * (x + y * FFT_SIZE);
            float i = 2.0 * M_PI * (x >= FFT_SIZE / 2 ? x - FFT_SIZE : x);
            float j = 2.0 * M_PI * (y >= FFT_SIZE / 2 ? y - FFT_SIZE : y);
            totalSlopeVariance += getSlopeVariance(i / GRID1_SIZE, j / GRID1_SIZE, spectrum12 + offset);
            totalSlopeVariance += getSlopeVariance(i / GRID2_SIZE, j / GRID2_SIZE, spectrum12 + offset + 2);
            totalSlopeVariance += getSlopeVariance(i / GRID3_SIZE, j / GRID3_SIZE, spectrum34 + offset);
            totalSlopeVariance += getSlopeVariance(i / GRID4_SIZE, j / GRID4_SIZE, spectrum34 + offset + 2);
        }
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, variancesFbo);
    glViewport(0, 0, N_SLOPE_VARIANCE, N_SLOPE_VARIANCE);

    glUseProgram(variances->program);
    glUniform4f(glGetUniformLocation(variances->program, "GRID_SIZES"), GRID1_SIZE, GRID2_SIZE, GRID3_SIZE, GRID4_SIZE);
    glUniform1f(glGetUniformLocation(variances->program, "slopeVarianceDelta"), 0.5 * (theoreticSlopeVariance - totalSlopeVariance));

    for (int layer = 0; layer < N_SLOPE_VARIANCE; ++layer) {
        glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_3D, slopeVarianceTex, 0, layer);
        glUniform1f(glGetUniformLocation(variances->program, "c"), layer);
        drawQuad();
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

int Ocean::bitReverse(int i, int N)
{
	int j = i;
	int M = N;
	int Sum = 0;
	int W = 1;
	M = M / 2;
	while (M != 0) {
		j = (i & M) > M - 1;
		Sum += j * W;
		W *= 2;
		M = M / 2;
	}
	return Sum;
}

void Ocean::computeWeight(int N, int k, float &Wr, float &Wi)
{
	Wr = cosl(2.0 * M_PI * k / float(N));
	Wi = sinl(2.0 * M_PI * k / float(N));
}

float* Ocean::computeButterflyLookupTexture()
{
    float *data = new float[FFT_SIZE * PASSES * 4];

	for (int i = 0; i < PASSES; i++) {
		int nBlocks  = (int) powf(2.0, float(PASSES - 1 - i));
		int nHInputs = (int) powf(2.0, float(i));
		for (int j = 0; j < nBlocks; j++) {
			for (int k = 0; k < nHInputs; k++) {
			    int i1, i2, j1, j2;
				if (i == 0) {
					i1 = j * nHInputs * 2 + k;
					i2 = j * nHInputs * 2 + nHInputs + k;
					j1 = bitReverse(i1, FFT_SIZE);
					j2 = bitReverse(i2, FFT_SIZE);
				} else {
					i1 = j * nHInputs * 2 + k;
					i2 = j * nHInputs * 2 + nHInputs + k;
					j1 = i1;
					j2 = i2;
				}

				float wr, wi;
				computeWeight(FFT_SIZE, k * nBlocks, wr, wi);

                int offset1 = 4 * (i1 + i * FFT_SIZE);
                data[offset1 + 0] = (j1 + 0.5) / FFT_SIZE;
                data[offset1 + 1] = (j2 + 0.5) / FFT_SIZE;
                data[offset1 + 2] = wr;
                data[offset1 + 3] = wi;

                int offset2 = 4 * (i2 + i * FFT_SIZE);
                data[offset2 + 0] = (j1 + 0.5) / FFT_SIZE;
                data[offset2 + 1] = (j2 + 0.5) / FFT_SIZE;
                data[offset2 + 2] = -wr;
                data[offset2 + 3] = -wi;
			}
		}
	}

	return data;
}

void Ocean::simulateFFTWaves(float t)
{
    // init
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fftFbo1);
    glViewport(0, 0, FFT_SIZE, FFT_SIZE);
    glUseProgram(init->program);
    glUniform1f(glGetUniformLocation(init->program, "FFT_SIZE"),FFT_SIZE);
    glUniform4f(glGetUniformLocation(init->program, "INVERSE_GRID_SIZES"),
        2.0 * M_PI * FFT_SIZE / GRID1_SIZE,
        2.0 * M_PI * FFT_SIZE / GRID2_SIZE,
        2.0 * M_PI * FFT_SIZE / GRID3_SIZE,
        2.0 * M_PI * FFT_SIZE / GRID4_SIZE);
    glUniform1f(glGetUniformLocation(init->program, "t"), t);
    drawQuad();

    // fft passes
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fftFbo2);
    glUseProgram(fftx->program);
    glUniform1i(glGetUniformLocation(fftx->program, "nLayers"), choppy ? 5 : 3);
    for (int i = 0; i < PASSES; ++i) {
        glUniform1f(glGetUniformLocation(fftx->program, "pass"), float(i + 0.5) / PASSES);
        if (i%2 == 0) {
            glUniform1i(glGetUniformLocation(fftx->program, "imgSampler"), FFT_A_UNIT);
            glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
        } else {
            glUniform1i(glGetUniformLocation(fftx->program, "imgSampler"), FFT_B_UNIT);
            glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
        }
        drawQuad();
    }
    glUseProgram(ffty->program);
    glUniform1i(glGetUniformLocation(ffty->program, "nLayers"), choppy ? 5 : 3);
    for (int i = PASSES; i < 2 * PASSES; ++i) {
        glUniform1f(glGetUniformLocation(ffty->program, "pass"), float(i - PASSES + 0.5) / PASSES);
        if (i%2 == 0) {
            glUniform1i(glGetUniformLocation(ffty->program, "imgSampler"), FFT_A_UNIT);
            glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
        } else {
            glUniform1i(glGetUniformLocation(ffty->program, "imgSampler"), FFT_B_UNIT);
            glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
        }
        drawQuad();
    }

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    glActiveTexture(GL_TEXTURE0 + FFT_A_UNIT);
    glGenerateMipmapEXT(GL_TEXTURE_2D_ARRAY_EXT);
}