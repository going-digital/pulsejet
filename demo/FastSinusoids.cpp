#include "FastSinusoids.hpp"

#include <cstdint>

#ifndef BHASKARA_I_APPROX
static inline constexpr uint32_t fastCosTabLog2Size = 10; // size = 1024
static inline constexpr uint32_t fastCosTabSize = 1 << fastCosTabLog2Size;
static double fastCosTab[fastCosTabSize + 1];
#endif

namespace FastSinusoids
{
	void Init()
	{
		#ifndef BHASKARA_I_APPROX
		for (uint32_t i = 0; i < fastCosTabSize + 1; i++)
		{
			const auto phase = static_cast<double>(i) * M_PI * 2.0 / static_cast<double>(fastCosTabSize);
			fastCosTab[i] = cos(phase);
		}
		#endif // BHASKARA_I_APPROX
	}

	#ifdef BHASKARA_I_APPROX
	double Sin(double x)
	{
		const auto rotations = x * 1.0 / (M_PI * 2.0);
		const auto half_circle = 2 * rotations - floor(2 * rotations);
		const auto parabola = half_circle * (1-half_circle);
		return copysign(
			4 * parabola / (1.25 - parabola),
			0.5 + floor(rotations) - rotations
		);
	}
	#else // BHASKARA_I_APPROX
	double Cos(double x)
	{
		x = fabs(x); // cosine is symmetrical around 0, let's get rid of negative values

		// normalize range from 0..2PI to 1..2
		const auto phaseScale = 1.0 / (M_PI * 2.0);
		const auto phase = 1.0 + x * phaseScale;

		const auto phaseAsInt = *reinterpret_cast<const uint64_t *>(&phase);
		const auto exponent = static_cast<int32_t>(phaseAsInt >> 52) - 1023;

		const auto fractBits = 32 - fastCosTabLog2Size;
		const auto fractScale = 1 << fractBits;
		const auto fractMask = fractScale - 1;

		const auto significand = static_cast<uint32_t>((phaseAsInt << exponent) >> (52 - 32));
		const auto index = significand >> fractBits;
		const auto fract = significand & fractMask;

		const auto left = fastCosTab[index];
		const auto right = fastCosTab[index + 1];

		const auto fractMix = fract * (1.0 / fractScale);
		return left + (right - left) * fractMix;
	}
	#endif // BHASKARA_I_APPROX
}
