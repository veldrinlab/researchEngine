/*
 * Projekt C-Way
 * Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
 * Szymon Jab�o�ski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef STATICNOISECLOUD_HPP
#define STATICNOISECLOUD_HPP

#include "NoiseCloudType.hpp"

namespace CW
{
	struct StaticNoiseCloud
	{
		int width;
		int height;
		CloudData data;
		NoiseCloudType type;
	};
}
#endif
