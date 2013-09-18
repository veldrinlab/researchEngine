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
	vec4 skyHorizon = vec4(201.0/255.0f, 197.0f/255.0f, 188.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(89.0/255.0f, 102/255.0f, 144.0/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(167.0f/255.0f,174.0f/255.0f,180.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(111.0f/255.0f,127.0f/255.0f,160.0f/255.0f,1.0f);

	float checkPoint1Offset = 0.13f;
	float checkPoint2Offset = 0.55f;

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	if(a < 0.0f)
		a = 0.0f;

	if(a > checkPoint2Offset)
		gl_FragColor = mix(checkPoint2,skyZenith,1.0f/(1.0-checkPoint2Offset)*(a-checkPoint2Offset));	
	else if(a > checkPoint1Offset && a < checkPoint2Offset)
		gl_FragColor = mix(checkPoint1,checkPoint2,1.0f/(checkPoint2Offset-checkPoint1Offset)*(a-checkPoint1Offset));	
	else if(a < checkPoint1Offset)
		gl_FragColor = mix(skyHorizon,checkPoint1,a/checkPoint1Offset);
}
