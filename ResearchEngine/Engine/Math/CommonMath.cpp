/**
 *  File contains definition of CommonMath class.
 *  @file    CommonMath.cpp
 *  @author  Szymon "Veldrin" Jab³oñski
 *  @date    2011-06-24
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "CommonMath.hpp"

namespace AyumiEngine
{
	namespace AyumiMath
	{
		/**
		 * Definitions of CommonMath static constants.
		 */
		const float CommonMath::PI = 3.1415926f;
		const float CommonMath::HALF_PI = CommonMath::PI / 2.0f;
		const float CommonMath::QUARTER_PI = CommonMath::PI / 4.0f;
		const float CommonMath::TWO_PI = CommonMath::PI * 2.0f;
		const float CommonMath::EPSILON = 1e-6f;
	}
}
