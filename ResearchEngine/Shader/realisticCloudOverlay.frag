//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform sampler2D cloudTexture;



varying	vec3 v;

void main ()
{
	// sky 
	vec4 skyTop = vec4(129.0/255.0, 181.0/255.0, 251.0/255.0, 1.0f);
	vec4 skyHorizon = vec4(206.0/255.0, 226.0/255.0, 254.0/255.0,  1.0f);

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	vec4 skyColor = mix(skyHorizon, skyTop, a);	

	
	
	
	// cloud color
	vec4 cloudColor = texture2D(cloudTexture,gl_TexCoord[0].st);
	
	// overlay blending

	vec4 color;

	vec4 lumCoeff = vec4(0.2126,0.7152,0.0722,1.0);

	float luminance = dot(skyColor, lumCoeff);
	if (luminance < 0.45)
		color = 2.0 * cloudColor * skyColor;
	else if (luminance > 0.55)
		color = vec4(1.0,1.0,1.0,1.0) - 2.0 * (vec4(1.0,1.0,1.0,1.0) - cloudColor) * (vec4(1.0,1.0,1.0,1.0) - skyColor);
	else
	{
		vec4 result1 = 2.0 * cloudColor * skyColor;
		vec4 result2 = vec4(1.0,1.0,1.0,1.0) - 2.0 * (vec4(1.0,1.0,1.0,1.0) - cloudColor) * (vec4(1.0,1.0,1.0,1.0) - skyColor);
		color = mix(result1, result2, (luminance - 0.45) * 10.0);
	}

	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}