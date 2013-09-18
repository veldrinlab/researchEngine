//
// Simple HDR shader
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

void main()
{
	 vec4 color = texture2DRect(frame,gl_TexCoord[0].st);
	 gl_FragColor =  1.0 - exp(color * -exposure);
}
