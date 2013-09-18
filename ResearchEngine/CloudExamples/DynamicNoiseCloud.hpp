/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef DYNAMICNOISECLOUD_HPP
#define DYNAMICNOISECLOUD_HPP

#include <vector>

#include "NoiseCloudType.hpp"

namespace CW
{
	struct DynamicNoiseCloud
	{
		int width;
		int height;
		int statesAmount;
		std::vector<CloudData> data;
		NoiseCloudType type;
	};
}
#endif
