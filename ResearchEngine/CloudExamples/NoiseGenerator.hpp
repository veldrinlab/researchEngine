/*
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef NOISEGENERATOR_HPP
#define NOISEGENERATOR_HPP

#include <stdlib.h>
#include "InterpolationType.hpp"

// TODO generic Noise Generator - interpolation type
template <typename T = float>
class NoiseGenerator
{
private:
	T persistance;
	int octaveAmount;
	int seed;
	T zoom;
	T frequency;
	T amplitude;	

public:
	/**
	 * Class default constructor. Set noise properties to defaut values.
	 */
	NoiseGenerator()
	{
		persistance = 0.5;
		octaveAmount = 8;
		seed = 42;
		zoom = 32.0f;
		frequency = 1.0;
		amplitude = 1.0;
	}

	~NoiseGenerator() {}
	
	void configurePerlinNoise(const T persistance, const int octaveAmount, const int seed, const T zoom, const T initialFrequency, const T initialAmplitude)
	{
		this->persistance = persistance;
		this->octaveAmount = octaveAmount;
		this->seed = seed;
		this->zoom = zoom;
		this->frequency = initialFrequency;
		this->amplitude = initialAmplitude;
	}

	void chnageSeed()
	{
		seed = rand() % 5000;
	}
	
	T whiteNoise()
	{
		return (rand() % 32768) / 32768.0f;
	}

	T noise1D(const int x)
	{
		int n = (x<<13) ^ x;
		return static_cast<T>(1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f); 
	}

	T noise2D(const int x, const int y)
	{
		int n = x + y * 57 + seed * 131;
		n = (n<<13) ^ n;
		return static_cast<T>(1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);    
	}

	T noise3D(const int x, const int y, const int z)
	{
		int n = x + y*57 + z * 103 + seed * 131;
		n = (n << 13) ^ n;
		return static_cast<T>(1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
	}

	T smoothedNoise1D(const int x)
	{
		return noise1D(x)/2 + noise1D(x-1)/4 + noise1D(x+1)/4;
	}
	
	T smoothedNoise2D(const int x, const int y)
	{
		T corners = (noise2D(x-1, y-1) + noise2D(x+1, y-1) + noise2D(x-1, y+1) + noise2D(x+1, y+1)) / 16;
		T sides = ( noise2D(x-1, y)  + noise2D(x+1, y)  + noise2D(x, y-1) + noise2D(x, y+1)) /  8;
		T center = noise2D(x, y) / 4;
		
		return corners + sides + center;
	}

    T smoothNoise3D(const int x, const int y, const int z)
    {
        T corners0 = (noise3D(x - 1, y - 1, z - 1) + noise3D(x + 1, y - 1, z - 1) + noise3D(x + 1, y + 1, z - 1) + noise3D(x - 1, y + 1, z - 1)) / 16;
        T sides0 = (noise3D(x + 1, y, z - 1) + noise3D(x - 1, y, z - 1) + noise3D(x, y + 1, z - 1) + noise3D(x, y - 1, z - 1)) / 8;
        T center0 = noise3D(x, y, z - 1) / 4;

        T corners = (noise3D(x - 1, y - 1, z) + noise3D(x + 1, y - 1, z) + noise3D(x + 1, y + 1, z) + noise3D(x - 1, y + 1, z)) / 16;
        T sides = (noise3D(x + 1, y, z) + noise3D(x - 1, y, z) + noise3D(x, y + 1, z) + noise3D(x, y - 1, z)) / 8;
        T center = noise3D(x, y, z) / 4;

        T corners1 = (noise3D(x - 1, y - 1, z + 1) + noise3D(x + 1, y - 1, z + 1) + noise3D(x + 1, y + 1, z + 1) + noise3D(x - 1, y + 1, z + 1)) / 16;
        T sides1 = (noise3D(x + 1, y, z + 1) + noise3D(x - 1, y, z + 1) + noise3D(x, y + 1, z + 1) + noise3D(x, y - 1, z + 1)) /8;
        T center1 = noise3D(x, y, z + 1) / 4;

		T level0 = corners0 + sides0 + center0;
		T levelCurrent = corners + sides + center;
		T level1 = corners1 + sides1 + center1;

        return (( level0 / 3) + (levelCurrent / 3) + (level1 / 3));
    }

	T interpolatedNoise1D(const T x)
	{
		int integerX = static_cast<int>(x);
		T fractionalX = x - integerX;

		T v1 = smoothedNoise1D(integerX);
		T v2 = smoothedNoise1D(integerX + 1);

		return linearInterpolation(v1,v2,fractionalX);
	}

	T interpolatedNoise2D(const T x, const T y)
	{
		int integerX = static_cast<int>(x);
		T fractionalX = x - integerX;
		
		int integerY = static_cast<int>(y);
		T fractionalY = y - integerY;

		T v1 = smoothedNoise2D(integerX, integerY);
		T v2 = smoothedNoise2D(integerX + 1, integerY);
		T v3 = smoothedNoise2D(integerX, integerY + 1);
		T v4 = smoothedNoise2D(integerX + 1, integerY + 1);

		T i1 = linearInterpolation(v1, v2, fractionalX);
		T i2 = linearInterpolation(v3, v4, fractionalX);
		
		return linearInterpolation(i1 , i2 , fractionalY);
	}

    T interpolatedNoise3D(const T x, const T y, const T z)
    {
        int integerX = static_cast<int>(x);
        int integerY = static_cast<int>(y);
        int integerZ = static_cast<int>(z);

        T fractionalX = x - integerX;
        T fractionalY = y - integerY;
        T fractionalZ = z - integerZ;

        T v1 = smoothNoise3D(integerX, integerY, integerZ);
        T v2 = smoothNoise3D(integerX + 1, integerY, integerZ);
        T v3 = smoothNoise3D(integerX, integerY + 1, integerZ);
        T v4 = smoothNoise3D(integerX + 1, integerY + 1, integerZ);
        T v5 = smoothNoise3D(integerX, integerY, integerZ + 1);
        T v6 = smoothNoise3D(integerX + 1, integerY, integerZ + 1);
        T v7 = smoothNoise3D(integerX, integerY + 1, integerZ + 1);
        T v8 = smoothNoise3D(integerX + 1, integerY + 1, integerZ + 1);

		T i1 = linearInterpolation(v1, v2, fractionalX);
        T i2 = linearInterpolation(v3, v4, fractionalX);
        T i3 = linearInterpolation(v5, v6, fractionalX);
        T i4 = linearInterpolation(v7, v8, fractionalX);

		T lvl0 = linearInterpolation(i1, i2, fractionalY);
        T lvl1 = linearInterpolation(i3, i4, fractionalY);

        return linearInterpolation(lvl0, lvl1, fractionalZ);
    }

	T perlinNoise1D(const T x)
	{
		T value = 0.0;
		T p = persistance;
		int n = octaveAmount;
		T freq = frequency;
		T ampl = amplitude;	
		T z = zoom;

		for(int i = 0; i < n; ++i)
		{
			value += interpolatedNoise1D(x * freq/z) * ampl;
			ampl *= p;
			freq *= 2.0;
		}

		return value;
	}

	T perlinNoise2D(const int x, const int y)
	{	
		T value = 0.0;
		T p = persistance;
		int n = octaveAmount;
		T freq = frequency;
		T ampl = amplitude;		
		T z = zoom;

		for(int i = 0; i < n; ++i)
		{
			value += interpolatedNoise2D(x * freq/z, y * freq/z) * ampl;
			ampl *= p;
			freq *= 2.0;
		}

		return value;
	}

	T perlinNoise3D(const int x, const int y, const int z)
	{	
		T value = 0.0;
		T p = persistance;
		int n = octaveAmount;
		T freq = frequency;
		T ampl = amplitude;		
		T zoom = this->zoom;

		for(int i = 0; i < n; ++i)
		{
			value += interpolatedNoise3D(x * freq/zoom, y * freq/zoom, z*freq/zoom ) * ampl;
			ampl *= p;
			freq *= 2.0;
		}

		return value;
	}

	T linearInterpolation(const T a, const T b, const T x)
	{
		return static_cast<T>(a*(1-x) + b*x);
	}

	T cosineInterpolation(const T a, const T b, const T x)
	{
		T ft = x * 3.1415927;
		T f = (1 - cos(ft)) * 0.5;
		return static_cast<T>(a*(1-f) + b*f);
	}

	T cubicInterpolation(const T v0, const T v1, const T v2, const T v3, const T x)
	{
		T P = (v3 - v2) - (v0 - v1);
		T Q = (v0 - v1) - P;
		T R = v2 - v0;
		T S = v1;
		
		return static_cast<T>(P*x*x*x + Q*x*x + R*x + S);
	}
};
#endif
