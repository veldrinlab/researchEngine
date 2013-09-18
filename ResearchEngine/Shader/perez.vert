//
// Atmospheric scattering vertex shader
// Szymon "Veldrin" Jab³oñski
// 
// Based on Perez All-weather described by Alex V. Boreskoff
//
//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform	vec3 lightDirection;
uniform vec3 zenithYxy;
uniform vec3 AYxy;
uniform vec3 BYxy;
uniform vec3 CYxy;
uniform vec3 DYxy;
uniform vec3 EYxy;
uniform	float turbidity;

varying	vec3 colorYxy;

// Perez allweather function
vec3 perezFunc(float cosTheta, float cosGamma)
{
	float gamma = acos(cosGamma);
	float cosGammaSq = cosGamma * cosGamma;

	return vec3((1.0 + AYxy.x * exp(BYxy.x/cosTheta)) * (1.0 + CYxy.x * exp(DYxy.x * gamma) + EYxy.x*cosGammaSq),
	              (1.0 + AYxy.y * exp(BYxy.y/cosTheta)) * (1.0 + CYxy.y * exp(DYxy.y * gamma) + EYxy.y*cosGammaSq),
				  (1.0 + AYxy.z * exp(BYxy.z/cosTheta)) * (1.0 + CYxy.z * exp(DYxy.z * gamma) + EYxy.z*cosGammaSq));
}

vec3 perezSky(float cosTheta, float cosGamma, float cosThetaSun)
{	 
	vec3 skyColor = zenithYxy * perezFunc(cosTheta, cosGamma) / perezFunc(1.0, cosThetaSun);
	skyColor[0] *= smoothstep(0.0, 0.1, cosThetaSun);
	return skyColor;
}

void main()
{
	vec3 v = normalize(gl_Vertex);
	vec3 l = normalize(lightDirection);
	vec3 lv = dot(l,v);

	colorYxy = perezSky(max(v.y,0.0)+0.05, lv, l.y);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
