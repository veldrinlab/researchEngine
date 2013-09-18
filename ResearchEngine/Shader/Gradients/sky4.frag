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
	vec4 skyHorizon = vec4(153.0f/255.0f,218.0f/255.0f,254.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(43.0f/255.0f,34.0f/255.0f,56.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(119.0f/255.0f,200.0f/255.0f,245.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(53.0f/255.0f,164.0f/255.0f,234.0f/255.0f,1.0f);
	vec4 checkPoint3 = vec4(11.0f/255.0f,123.0f/255.0f,200.0f/255.0f,1.0f);	
	
	float checkPoint1Offset = 0.09f;
	float checkPoint2Offset = 0.30f;
	float checkPoint3Offset = 0.56f;

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
