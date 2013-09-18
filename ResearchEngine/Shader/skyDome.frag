// SkyDome shader
// Author: Szymon "Veldrin" Jab�o�ski"
// Date: 11.11.2011
//
// Projekt C-Way
// Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
// Szymon Jab�o�ski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform sampler2D ColorMap;

void main()
{
	vec4 texel = texture2D(ColorMap, gl_TexCoord[0].st);
	gl_FragColor = texel;
}