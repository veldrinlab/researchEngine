/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
 * Szymon Jab�o�ski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef PREETHAMMODEL_HPP
#define PREETHAMMODEL_HPP

struct PreethamModel
{
	float lightDirection[3];
	float zenith[3];
	float A[3];
	float B[3];
	float C[3];
	float D[3];
	float E[3];
	float turbidity;
};
#endif
