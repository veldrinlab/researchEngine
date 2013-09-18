//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform sampler2DRect frameBuffer;
//uniform float exposure;
//TODO dodac gradient i glow

varying	vec3 v;

// TODO funkcja szukajaca czy kolorowac

bool edgeDetection(vec2 coord)
{
	/*
	1 2 3
	4 c 5
	6 7 8
	*/

	vec4 color = texture2DRect(frameBuffer,coord);
	vec4 c1 = texture2DRect(frameBuffer,coord + vec2(-1,-1));
	vec4 c2 = texture2DRect(frameBuffer,coord + vec2(0,-1));
	vec4 c3 = texture2DRect(frameBuffer,coord + vec2(1,-1));
	vec4 c4 = texture2DRect(frameBuffer,coord + vec2(-1,0));
	vec4 c5 = texture2DRect(frameBuffer,coord + vec2(1,0));
	vec4 c6 = texture2DRect(frameBuffer,coord + vec2(-1,1));
	vec4 c7 = texture2DRect(frameBuffer,coord + vec2(0,1));
	vec4 c8 = texture2DRect(frameBuffer,coord + vec2(1,1));

	if(color.r + color.g + color.b == 3.0f)
		if(c1.r == 0.0f || c2.r == 0.0f || c3.r == 0.0f 
			|| c4.r == 0.0f || c4.r == 0.0f || c5 == 0.0f
			|| c6.r == 0.0f || c7.r == 0.0f || c8 == 0.0f)
			return true;
	
	return false;
}

void main()
{
	vec4 strokeColor = vec4(0.318f, 0.314f, 0.345f,1.0f); //TODO maybe uniform

	// szukamy krawedzi

	if(edgeDetection(gl_TexCoord[0].st))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(-1,-1)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(0,-1)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(1,-1)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(-1,0)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(1,0)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(-1,1)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(0,1)))
		gl_FragColor = strokeColor;
	else if(edgeDetection(gl_TexCoord[0].st + vec2(1,1)))
		gl_FragColor = strokeColor;
	else
	{
		vec4 color = texture2DRect(frameBuffer,gl_TexCoord[0].st);
		if(color.r + color.g + color.b == 0.0f)
			color.a = 0.0f;
		gl_FragColor = color;
	}

}

	//gl_FragColor.a = 0.0f;
	// add gradient overlay
//	vec3 point = normalize(v);
 //   float value = point.y;
//	vec4 top = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//	vec4 bottom = vec4(0.878f, 0.855f, 0.988f, 1.0f);
	//if(color.r + color.g + color.b > 2.8f)
//		color = mix(top, bottom, value);
//	color -= mix(top, bottom, value);
	//gl_FragColor = color;