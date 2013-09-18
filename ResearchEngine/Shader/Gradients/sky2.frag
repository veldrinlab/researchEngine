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
	vec4 skyHorizon = vec4(190.0/255.0f, 241.0f/255.0f, 213.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(1.0/255.0f, 2/255.0f, 23.0/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(78.0f/255.0f,124.0f/255.0f,166.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(18.0f/255.0f,47.0f/255.0f,103.0f/255.0f,1.0f);

	float checkPoint1Offset = 0.37f;
	float checkPoint2Offset = 0.74f;

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
