//
// Projekt C-Way
// Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
// Szymon Jab�o�ski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//


varying	vec3 v;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

