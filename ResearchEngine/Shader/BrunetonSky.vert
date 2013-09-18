//
// Projekt C-Way
// Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
// Szymon Jab³oñski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//
 
uniform mat4 projectionMtx;
uniform mat4 modelViewMtx;

varying vec3 viewDir;

void main()
{
	viewDir = vec3(modelViewMtx * gl_Vertex);
    gl_Position = vec4(gl_Vertex.xy, 0.9999999, 1.0);
}


 