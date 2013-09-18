// Atmospheric scattering fragment shader
// Szymon "Veldrin" Jab³oñski
// 
// Based on Sean O'Neal Model
//
//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform vec3 lightDirection;

uniform float scatteringSymmetry;
uniform float scatteringSymmetry2;

varying vec3 cameraDirection;
varying vec4 rayleighColor;
varying vec4 mieColor;

// Mie phase function
float getMiePhase(float lightAngle, float lightAngle2, float scatteringSymmetry, float scatteringSymmetry2)
{
	return 1.5*((1.0-scatteringSymmetry2)/(2.0+scatteringSymmetry2))*(1.0+lightAngle2)/pow(1.0+scatteringSymmetry2 - 2.0*scatteringSymmetry*lightAngle, 1.5);
}

// Rayleigh phase function
float getRayleighPhase(float lightAngle2)
{ 
	return 0.75 * (2.0 + 0.5 * lightAngle2);	
}

void main()
{
	float lightAngle = dot(lightDirection, normalize(cameraDirection));
	
	float lightAngle2 = lightAngle * lightAngle;
	
	float miePhase = getMiePhase(lightAngle,lightAngle2,scatteringSymmetry,scatteringSymmetry2);
	float rayleighPhase = getRayleighPhase(lightAngle2);
	
	gl_FragColor = rayleighPhase*rayleighColor + miePhase* mieColor; //duze slonce bez Secodnar colors
	//gl_FragColor = 1.0-exp(gl_FragColor*-2.0);	//HDR póŸniej doday
	//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
