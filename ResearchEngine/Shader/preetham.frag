//
// Atmospheric scattering fragment shader
// Szymon "Veldrin" Jab這雟ki
// 
// Based on Preetham "A Practical Analytic Model for Daylight" Model
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
uniform vec3 baseColor;
uniform vec3 zenith;
uniform vec3 A;
uniform vec3 B;
uniform vec3 C;
uniform vec3 D;
uniform vec3 E;
 
varying vec3 pos; 
 
vec3 xyY_to_XYZ(const vec3 src) 
{ 
	float f = (src.z / src.y); 
	return vec3(src.x * f, src.z, (1.0 - src.x - src.y) * f); 
} 
 
vec3 XYZ_to_RGB(const vec3 src) 
{ 
	const mat3 m = mat3( 
		+3.240479, -0.969256, +0.055648, 
		-1.537150, +1.875991, -0.204043, 
		-0.498535, +0.041556, +1.057311); 
	return m * src; 
} 

//gamma - k靖 mi璠zy targetem i s這鎍em
//theta - kat mi璠zy tagetem i zenitem

void main() 
{
	vec3 target = normalize(pos); 			
	float cos_theta = abs(target.y);
	float cos_gamma = dot(lightDirection, target); 
	float gamma = acos(cos_gamma); 

	vec3 xyY = (1.0 + A*exp(B/cos_theta)) * (1.0 + C*exp(D*gamma) + E*cos_gamma*cos_gamma); 
	vec3 XYZ = xyY_to_XYZ(zenith*xyY);
	vec3 RGB = XYZ_to_RGB(XYZ);
	
	
	 
	gl_FragColor = vec4(RGB, 1.0); 
} 

// fajny fade filmikowy
//vec3 target = normalize(pos); 			
//float cos_theta = abs(target.y);
//float cos_gamma = abs(lightDirection.y);
// dot(lightDirection, target); 
//float gamma = acos(cos_gamma); 