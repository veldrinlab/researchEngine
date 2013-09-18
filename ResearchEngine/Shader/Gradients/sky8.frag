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
	vec4 skyHorizon = vec4(255.0f/255.0f,246.0f/255.0f,145.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(14.0f/255.0f,43.0f/255.0f,75.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(255.0f/255.0f,210.0f/255.0f,32.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(255.0f/255.0f,162.0f/255.0f,4.0f/255.0f,1.0f);
	vec4 checkPoint3 = vec4(203.0f/255.0f,157.0f/255.0f,64.0f/255.0f,1.0f);	
	vec4 checkPoint4 = vec4(50.0f/255.0f,86.0f/255.0f,94.0f/255.0f,1.0f);
	vec4 checkPoint5 = vec4(14.0f/255.0f,43.0f/255.0f,75.0f/255.0f,1.0f);

	float checkPoint1Offset = 0.06f;
	float checkPoint2Offset = 0.09f;
	float checkPoint3Offset = 0.19f;
	float checkPoint4Offset = 0.50f;
	float checkPoint5Offset = 0.90f;

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	if(a < 0.0f)
		a = 0.0f;

	if(a > checkPoint5Offset)
		gl_FragColor = mix(checkPoint5,skyZenith,1.0f/(1.0-checkPoint5Offset)*(a-checkPoint5Offset));
	else if(a > checkPoint4Offset && a < checkPoint5Offset)
		gl_FragColor = mix(checkPoint4,checkPoint5,1.0f/(checkPoint5Offset-checkPoint4Offset)*(a-checkPoint4Offset));	
	else if(a > checkPoint3Offset && a < checkPoint4Offset)
		gl_FragColor = mix(checkPoint3,checkPoint4,1.0f/(checkPoint4Offset-checkPoint3Offset)*(a-checkPoint3Offset));	
	else if(a > checkPoint2Offset && a < checkPoint3Offset)
		gl_FragColor = mix(checkPoint2,checkPoint3,1.0f/(checkPoint3Offset-checkPoint2Offset)*(a-checkPoint2Offset));	
	else if(a > checkPoint1Offset && a < checkPoint2Offset)
		gl_FragColor = mix(checkPoint1,checkPoint2,1.0f/(checkPoint2Offset-checkPoint1Offset)*(a-checkPoint1Offset));
	else if(a < checkPoint1Offset)
		gl_FragColor = mix(skyHorizon,checkPoint1,a/checkPoint1Offset);
}
