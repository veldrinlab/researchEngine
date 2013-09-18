//
// Atmospheric scattering fragment shader
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

varying	vec3 colorYxy;

vec3 convertToRGB()
{
	vec3 clrYxy = vec3(colorYxy);
												
	clrYxy[0] = 1.0 - exp(-clrYxy[0] / 25.0f);

	float ratio = clrYxy[0] / clrYxy[2];
	
	vec3 XYZ;

	XYZ.x = clrYxy[1] * ratio;
	XYZ.y = clrYxy[0];								
	XYZ.z = ratio - XYZ.x - XYZ.y;					

	const vec3 rCoeffs = vec3(3.240479, -1.53715, -0.49853);
	const vec3 gCoeffs = vec3(-0.969256, 1.875991, 0.041556);
	const vec3 bCoeffs = vec3(0.055684, -0.204043, 1.057311);

	return	vec3(dot(rCoeffs, XYZ), dot(gCoeffs, XYZ), dot(bCoeffs, XYZ));
}

void main()
{
	gl_FragColor = vec4(clamp(convertToRGB(), 0.0, 1.0),1.0);
}

//float sunGlow = sunBrightness * skyLuminance;
//sunGlow = lerp(1.0, sunGlow, 1 / gamma);,