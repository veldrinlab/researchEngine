//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

varying	vec3 v;

void main ()
{
	vec4 skyTop = vec4(0.184f, 0.51f, 0.753f, 1.0f);
	vec4 skyHorizon = vec4(0.427f, 0.745f, 0.871f,  1.0f);

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	// sun glow




	gl_FragColor = mix(skyHorizon, skyTop, a);	
}
