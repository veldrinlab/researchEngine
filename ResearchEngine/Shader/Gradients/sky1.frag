//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

varying	vec3 v;

// Using Gradient transfer function test



void main ()
{
	vec4 skyHorizon = vec4(155.0f/255.0f,55.0f/255.0f,25.0f/255.0f, 1.0f);
	vec4 skyZenith = vec4(43.0f/255.0f,34.0f/255.0f,56.0f/255.0f, 1.0f);
	
	vec4 checkPoint1 = vec4(212.0f/255.0f,110.0f/255.0f,35.0f/255.0f,1.0f);
	vec4 checkPoint2 = vec4(194.0f/255.0f,146.0f/255.0f,98.0f/255.0f,1.0f);
	vec4 checkPoint3 = vec4(130.0f/255.0f,111.0f/255.0f,112.0f/255.0f,1.0f);	
	
	float checkPoint1Offset = 0.14f;
	float checkPoint2Offset = 0.29f;
	float checkPoint3Offset = 0.53f;

	vec3 pointOnSphere = normalize(v);
    float point = pointOnSphere.y;
	
	if(point < 0.0f)
		point = 0.0f;

	float pos;
	float transfer;

	float a = 0.5f;
	float b = 5.0f;

	if(point > checkPoint3Offset)
	{
		pos = 1.0f/(1.0-checkPoint3Offset)*(point-checkPoint3Offset);
		transfer = (1-a)*pos + a*pow(1+exp(b-2*b*pos),-1);
		gl_FragColor = mix(checkPoint3,skyZenith,transfer);
	
		//	gl_FragColor = mix(checkPoint3,skyZenith,1.0f/(1.0-checkPoint3Offset)*(point-checkPoint3Offset));
	}
	else if(point > checkPoint2Offset && point < checkPoint3Offset)
	{
		pos = 1.0f/(checkPoint3Offset-checkPoint2Offset)*(point-checkPoint2Offset);
		transfer = (1-a)*pos + a*pow(1+exp(b-2*b*pos),-1);
		gl_FragColor = mix(checkPoint2,checkPoint3,transfer);	
		//gl_FragColor = mix(checkPoint2,checkPoint3,1.0f/(checkPoint3Offset-checkPoint2Offset)*(point-checkPoint2Offset));	
	}
	else if(point > checkPoint1Offset && point < checkPoint2Offset)
	{
		pos = 1.0f/(checkPoint2Offset-checkPoint1Offset)*(point-checkPoint1Offset);
		transfer = (1-a)*pos + a*pow(1+exp(b-2*b*pos),-1);
		gl_FragColor = mix(checkPoint1,checkPoint2,transfer);
		//gl_FragColor = mix(checkPoint1,checkPoint2,1.0f/(checkPoint2Offset-checkPoint1Offset)*(point-checkPoint1Offset));
	}
	else if(point < checkPoint1Offset)
	{
		pos = point/checkPoint1Offset;
		transfer = (1-a)*pos + a*pow(1+exp(b-2*b*pos),-1);
		gl_FragColor = mix(skyHorizon,checkPoint1,transfer);
		//gl_FragColor = mix(skyHorizon,checkPoint1,point/checkPoint1Offset);
	}
}
