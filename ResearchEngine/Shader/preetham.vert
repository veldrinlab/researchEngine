//
// Atmospheric scattering vertex shader
// Szymon "Veldrin" Jab�o�ski
// 
// Based on Preetham "A Practical Analytic Model for Daylight" Model
//
//
// Projekt C-Way
// Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
// Szymon Jab�o�ski
// pod kierunkiem Tomasza Martyna
// Instytut Informatyki
// Politechnika Warszawska
//

uniform float x;

varying vec3 pos; 
 
void main() 
{ 
	pos = gl_Vertex.xyz;
	
	//pos.y += 0.90f;
//	pos.x += x;


	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 
