//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform sampler2D cloudTexture;


varying	vec3 v;

void main ()
{
	// cloud color
	vec4 cloudColor = texture2D(cloudTexture,gl_TexCoord[0].st);

	gl_FragColor = cloudColor;
}