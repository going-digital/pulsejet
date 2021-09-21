#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#define BHASKARA_I_APPROX

namespace FastSinusoids
{
	void Init();
	#ifdef BHASKARA_I_APPROX
	double Sin(double x);
	inline double Cos(double x)
	{
		return Sin(x - M_PI_2);
	}
	inline float SinF(float x)
	{
		return static_cast<float>(Cos(static_cast<double>(x)));
	}
	inline float CosF(float x)
	{
		return static_cast<float>(Cos(static_cast<double>(x)));
	}
	#else // BHASKARA_I_APPROX
	double Cos(double x);

	inline double Sin(double x)
	{
		return Cos(x - M_PI_2);
	}
	inline float CosF(float x)
	{
		return static_cast<float>(Cos(static_cast<double>(x)));
	}
	inline float SinF(float x)
	{
		return static_cast<float>(Sin(static_cast<double>(x)));
	}
	#endif // BHASKARA_I_APPROX
}
