/*
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#ifndef ONEALMODEL_HPP
#define ONEALMODEL_HPP

struct ONealModel
{
	float rayleighConstant;
	float mieConstant;
	float sunBrightness;
	float scatteringSymmetry;
	float skyRadius;
	float lightDirection[3];
	float waveLength[3];
	int samples;

};

// future work
class Atmosphere
{
	//maybe base color
	//light direction
	//waveLength
	//skyradius
};

#endif
