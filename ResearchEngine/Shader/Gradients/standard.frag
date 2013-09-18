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
	vec4 skyTop = vec4(0.157f, 0.431f, 0.620f, 1.0f);
	vec4 skyHorizon = vec4(0.823f, 0.870f, 0.906f, 1.0f);

	vec3 pointOnSphere = normalize(v);
    float a = pointOnSphere.y;
	
	gl_FragColor = mix(skyHorizon, skyTop, a);
}
