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
	vec4 skyHorizon = vec4(175.0f/255.0f,209.0f/255.0f,210.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(9.0f/255.0f,92.0f/255.0f,172.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(134.0f/255.0f,197.0f/255.0f,213.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(66.0f/255.0f,184.0f/255.0f,240.0f/255.0f,1.0f);
	vec4 checkPoint3 = vec4(39.0f/255.0f,133.0f/255.0f,194.0f/255.0f,1.0f);	
	
	float checkPoint1Offset = 0.08f;
	float checkPoint2Offset = 0.33f;
	float checkPoint3Offset = 0.72f;

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
