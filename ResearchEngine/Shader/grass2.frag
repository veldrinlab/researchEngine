//
// Grass fragment shader
//

uniform sampler2D	grassMap;
uniform	sampler2D	noiseMap;

void main (void)
{
	vec4	grassColor = texture2D ( grassMap, gl_TexCoord [0].xy );
	float	noiseValue = texture2D ( noiseMap, gl_TexCoord [0].xy ).a;
	
	gl_FragColor = grassColor * vec4 ( vec3 ( 2.0 ), 2.0*noiseValue*gl_Color.a );
}
