// SkyDome shader
// Author: Szymon "Veldrin" Jab³oñski"
// Date: 11.11.2011
//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
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