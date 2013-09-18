/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
 * Szymon Jab�o�ski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef PEREZMODEL_HPP
#define PEREZMODEL_HPP

struct PerezModel
{
	float lightDirection[3];
	float zenithYxy[3];
	float turbidity;
	float AYxy[3];
	float BYxy[3];
	float CYxy[3];
	float DYxy[3];
	float EYxy[3];
};
#endif
