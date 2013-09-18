//
// Atmospheric scattering vertex shader
// Szymon "Veldrin" Jab這雟ki
// 
// Based on Sean O'Neal Model
// Modified for range scale enviroment
//
//
// Projekt C-Way
// Efekty nieba, chmur i s這鎍a w grze 2D z widokiem z boku
// Szymon Jab這雟ki
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform vec3 lightDirection;
uniform vec3 waveLength;
uniform float rayleighConstant;
uniform float mieConstant;
uniform	float rayleighConstantSun;	
uniform	float mieConstantSun;
uniform float skyRadius;
uniform int samples;

varying vec3 cameraDirection;
varying vec4 rayleighColor;
varying vec4 mieColor;

const float scaleDepth = 0.50f;
const float samplesQuantum = 2.0f;
const float cameraOffset = 1.0e-6f;

// Scale function
float scaleFactor(float angle)
{
	float x = 1.0 - angle;
	return scaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main()
{	
	// Calculate scale variables
	float scale = 1.0 / (skyRadius*1.025f - skyRadius);
	float scaleOverScaleDepth = scale/scaleDepth;
	vec3 cameraPosition = vec3(0.0f,skyRadius,0.0f);


	// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 pos = gl_Vertex.xyz / 10.25; // skyDome radius
	pos.y += 10.0;

	
	vec3 ray = pos - cameraPosition;
	float far = length(ray);
	ray /= far;

	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 start = cameraPosition;
	float height = length(start);	
	
	//fajne kolory  jak height *= 0.5;
	
	float depth = exp(scaleOverScaleDepth * (skyRadius - (skyRadius + cameraOffset)));

	
	float startAngle = dot(ray, start) / height;
	
	
	
	float startOffset = depth*scaleFactor(startAngle);
	


	// Initialize the scattering loop variables
	float sampleLength = far / samplesQuantum;
	
	
	float scaledLength = sampleLength * scale;

	vec3 sampleRay = ray * sampleLength;
	vec3 samplePoint = start + (sampleRay * 0.5); // podzielic przez 10 drugi cz這n r闚nania


	// Now loop through the sample rays
	vec3 color = vec3(0.0f, 0.0f, 0.0f); // zmienia bazowy kolor, ladne efekty, np. 1.0, 0.5
	
	for(int i = 0; i < samples; i++)
	{
		float height = length(samplePoint) ;
		float depth = exp(scaleOverScaleDepth * (skyRadius - height));
		float lightAngle = dot(lightDirection, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = (startOffset + depth*(scaleFactor(lightAngle) - scaleFactor(cameraAngle)));
		vec3 attenuate = exp(-scatter * (waveLength * rayleighConstant + mieConstant));
				
		color += attenuate * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	// Finally, scale the Rayleigh and Mie colors and set up the varying variables for the pixel shader
	rayleighColor.xyz = color * waveLength * rayleighConstantSun;
	mieColor.xyz = color * mieConstantSun;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	cameraDirection = cameraPosition - pos;
}
