//
// Grass vertex shader
//

uniform float time;
uniform	vec3  eyePos;

void main(void)
{
	float	d          = sqrt ( distance ( gl_Vertex.xyz, eyePos ) );
	float	alphaScale = 0.9 / ( 1.0 + d ) + 0.1;
	vec3	p          = vec3 ( gl_ModelViewMatrix * gl_Vertex );	// transformed point to world space
	

	gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord [0] = vec4 ( gl_MultiTexCoord0.x, (1.0 - gl_MultiTexCoord0.y), gl_MultiTexCoord0.zw );
	gl_TexCoord [1] = gl_MultiTexCoord1;							// ref point
	gl_FrontColor   = gl_Color  * vec4 ( 1.0, 1.0, 1.0, alphaScale );
	
	if ( gl_TexCoord [0].y < 0.5 )
	{	
		float	angle1 = time*10.0f + 0.3 * gl_TexCoord [1].x - 0.221 * gl_TexCoord [1].z;
		float	angle2 = 1.31415 * time*10.0f + 0.3 * gl_TexCoord [1].y - gl_TexCoord [1].z * gl_TexCoord [1].x;
		
		gl_Position += 0.05*vec4 ( sin ( angle1 ), cos ( angle2 ), 0, 0 );
	}
}
