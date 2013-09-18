//
// God Rays shader
//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//


uniform sampler2DRect frame;

uniform float exposure; 
uniform float decay;
uniform float density;
uniform float weight;
uniform float lightPositionOnScreenX;
uniform float lightPositionOnScreenY;

const int NUM_SAMPLES = 100;

void main()
{
	vec4 color = vec4(0);

	vec2 lightPos = vec2(lightPositionOnScreenX,lightPositionOnScreenY);
	
	vec2 deltaTextCoord = vec2(gl_TexCoord[0].st - lightPos.xy);	
	vec2 textCoo = gl_TexCoord[0].st;
	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
	float illuminationDecay = 1.0;
	
	for(int i=0; i < NUM_SAMPLES ; i++)
	{
		textCoo -= deltaTextCoord;
		vec4 sample = texture2DRect(frame,textCoo);
		sample *= illuminationDecay * weight;	
		color += sample;
		illuminationDecay *= decay;
	}	
	color *= exposure;
	color += texture2DRect(frame,gl_TexCoord[0].st);		
	//color.a = 0.9f;
	
	gl_FragColor = color;
}
