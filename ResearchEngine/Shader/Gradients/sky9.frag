//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

varying	vec3 v;

void main ()
{
	vec4 skyHorizon = vec4(255.0f/255.0f,249.0f/255.0f,165.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(68.0f/255.0f,106.0f/255.0f,198.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(248.0f/255.0f,178.0f/255.0f,133.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(236.0f/255.0f,168.0f/255.0f,159.0f/255.0f,1.0f);
	vec4 checkPoint3 = vec4(134.0f/255.0f,149.0f/255.0f,218.0f/255.0f,1.0f);	
	
	float checkPoint1Offset = 0.07f;
	float checkPoint2Offset = 0.19f;
	float checkPoint3Offset = 0.42f;

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	if(a < 0.0f)
		a = 0.0f;

	if(a > checkPoint3Offset)
		gl_FragColor = mix(checkPoint3,skyZenith,1.0f/(1.0-checkPoint3Offset)*(a-checkPoint3Offset));	
	else if(a > checkPoint2Offset && a < checkPoint3Offset)
		gl_FragColor = mix(checkPoint2,checkPoint3,1.0f/(checkPoint3Offset-checkPoint2Offset)*(a-checkPoint2Offset));	
	else if(a > checkPoint1Offset && a < checkPoint2Offset)
		gl_FragColor = mix(checkPoint1,checkPoint2,1.0f/(checkPoint2Offset-checkPoint1Offset)*(a-checkPoint1Offset));
	else if(a < checkPoint1Offset)
		gl_FragColor = mix(skyHorizon,checkPoint1,a/checkPoint1Offset);
}
