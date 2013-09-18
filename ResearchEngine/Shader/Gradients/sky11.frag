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
	vec4 skyHorizon = vec4(143.0f/255.0f, 132.0f/255.0f, 86.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(25.0f/255.0f, 27.0f/255.0f, 24.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(59.0f/255.0f,86.0f/255.0f,69.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(25.0f/255.0f, 27.0f/255.0f, 24.0f/255.0f, 1.0f);

	float checkPoint1Offset = 0.14f;
	float checkPoint2Offset = 0.69f;

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
