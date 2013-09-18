/*
 * Projekt C-Way
 * Efekty nieba, chmur i s�o�ca w grze 2D z widokiem z boku
 * Szymon Jab�o�ski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef NOISECLOUDTYPE_HPP
#define NOISECLOUDTYPE_HPP

namespace CW
{
	typedef float* CloudData;

	enum NoiseCloudType
	{
		CLASSIC,
		DIFF,
		EXPOTENTIAL
	};
}
#endif
