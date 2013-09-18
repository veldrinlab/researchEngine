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

void main()
{
	// Vertical Gausian Blur

/*
	float rt_w = 800.0f; // render target width
	float rt_h = 600.0f; // render target height
		
	float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
	float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

	vec3 color = vec3(1.0, 0.0, 0.0);
	vec2 uv = gl_TexCoord[0].xy;
	
	color = texture2DRect(frameBuffer, uv).rgb * weight[0];

	for(int i=1; i<3; i++) 
	{
		color += texture2DRect(frameBuffer, uv + vec2(0.0, offset[i])/rt_h).rgb * weight[i];
		color += texture2DRect(frameBuffer, uv - vec2(0.0, offset[i])/rt_h).rgb * weight[i];
	} */
	

	// bez blura test


	// progowanie

	vec4 color = texture2DRect(frameBuffer,gl_TexCoord[0].st);

	float intensity = (color.r + color.g + color.b) / 3.0f;

	if(intensity > 0.1f)
		color = vec4(1.0,1.0,1.0,0.1);
	else
		color = vec4(0.0,0.0,0.0,0.0);


	gl_FragColor = color;
}
