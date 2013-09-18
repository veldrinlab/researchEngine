/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
*/

#include "SkyExamples/GradientDemo.hpp"
#include "SkyExamples/PerezDemo.hpp"
#include "SkyExamples/PreethamDemo.hpp"
#include "SkyExamples/ONealDemo.hpp"
#include "SkyExamples/SkyDomeDemo.hpp"
#include "CloudExamples/NoiseCloudDemo.hpp"
#include "CloudExamples/ComicCloudDemo.hpp"
#include "SunExamples/SimpleSunDemo.hpp"
#include "SunExamples/GodraysSunDemo.hpp"
#include "SunExamples/LensFlareSunDemo.hpp"
#include "SkyExamples/BrunetonDemo.hpp"

#include "GrassDemo/GrassDemo.hpp"
#include "OceanDemo/OceanDemo.hpp"
#include "OceanDemo/OceanDemo2.hpp"
#include "OceanDemo/OceanDemo3.hpp"

using namespace ResearchEngine;

int main (int argc, char** argv)
{
	int id = 0;

	while(true)
	{
		system("cls");
		id = 0;
		printf("Research Engine 1.0 Szymon Jablonski\n\n");
		
		printf("1. Gradient demo \n");
		printf("2. Perez demo \n");
		printf("3. Preetham demo \n");
		printf("4. O'Neal demo \n");
		printf("5. SkyDome demo \n");
		printf("6. NoiseCloud demo \n");
		printf("7. ComicCloud demo \n");
		printf("8. SimpleSun demo \n");
		printf("9. GodRays demo \n");
		printf("10. LensFlare demo \n");
		printf("11. Bruneton demo \n");
		printf("12. Terrain demo \n");
		printf("13. Ocean demo O'Neil \n");
		printf("14. Ocean demo Preetham \n");
		printf("15. Ocean demo Bruneton \n");
		printf("$> ");
		scanf_s("%d",&id);

		switch(id)
		{
		case 1:
			initializeEngine(argc, argv);
			GradientDemo::getInstance()->initializeDemo();
			GradientDemo::getInstance()->doSomeMagic();
			GradientDemo::getInstance()->killInstance();
			break;
		case 2:
			initializeEngine(argc, argv);
			PerezDemo::getInstance()->initializeDemo();
			PerezDemo::getInstance()->doSomeMagic();
			PerezDemo::getInstance()->killInstance();
			break;
		case 3:
			initializeEngine(argc, argv);
			PreethamDemo::getInstance()->initializeDemo();
			PreethamDemo::getInstance()->doSomeMagic();
			PreethamDemo::getInstance()->killInstance();
			break;
		case 4:
			initializeEngine(argc, argv);
			ONealDemo::getInstance()->initializeDemo();
			ONealDemo::getInstance()->doSomeMagic();
			ONealDemo::getInstance()->killInstance();
			break;
		case 5:
			initializeEngine(argc, argv);
			SkyDomeDemo::getInstance()->initializeDemo();
			SkyDomeDemo::getInstance()->doSomeMagic();
			SkyDomeDemo::getInstance()->killInstance();
			break;
		case 6:
			initializeEngine(argc, argv);
			NoiseCloudDemo::getInstance()->initializeDemo();
			NoiseCloudDemo::getInstance()->doSomeMagic();
			NoiseCloudDemo::getInstance()->killInstance();
			break;
		case 7:
			initializeEngine(argc, argv);
			ComicCloudDemo::getInstance()->initializeDemo();
			ComicCloudDemo::getInstance()->doSomeMagic();
			ComicCloudDemo::getInstance()->killInstance();
			break;
		case 8:
			initializeEngine(argc, argv);
			SimpleSunDemo::getInstance()->initializeDemo();
			SimpleSunDemo::getInstance()->doSomeMagic();
			SimpleSunDemo::getInstance()->killInstance();
			break;
		case 9:
			initializeEngine(argc, argv);
			GodraysSunDemo::getInstance()->initializeDemo();
			GodraysSunDemo::getInstance()->doSomeMagic();
			GodraysSunDemo::getInstance()->killInstance();
			break;
		case 10:
			initializeEngine(argc, argv);
			LensFlareSunDemo::getInstance()->initializeDemo();
			LensFlareSunDemo::getInstance()->doSomeMagic();
			LensFlareSunDemo::getInstance()->killInstance();
			break;
		case 11:
			initializeEngine(argc, argv);
			BrunetonDemo::getInstance()->initializeDemo();
			BrunetonDemo::getInstance()->doSomeMagic();
			BrunetonDemo::getInstance()->killInstance();
			break;
		case 12:
			initializeEngine(argc, argv);
			GrassDemo::getInstance()->initializeDemo();
			GrassDemo::getInstance()->doSomeMagic();
			GrassDemo::getInstance()->killInstance();
			break;
		case 13:
			initializeEngine(argc, argv);
			OceanDemo::getInstance()->initializeDemo();
			OceanDemo::getInstance()->doSomeMagic();
			OceanDemo::getInstance()->killInstance();
			break;
		case 14:
			initializeEngine(argc, argv);
			OceanDemo2::getInstance()->initializeDemo();
			OceanDemo2::getInstance()->doSomeMagic();
			OceanDemo2::getInstance()->killInstance();
			break;
		case 15:
			initializeEngine(argc, argv);
			OceanDemo3::getInstance()->initializeDemo();
			OceanDemo3::getInstance()->doSomeMagic();
			OceanDemo3::getInstance()->killInstance();
			break;
		default:
			exit(0);
		}
	}
	return 0;
}
