#pragma once
#include "HaralickFeature.h"
class FourteenthFeature final :
    public HaralickFeature
{
private:
	float mean_{};
	float sum1_{};
	float sum2_{};
	float sum3_{};
	float sum4_{};
	float sum5_{};
	float sum6_{};
	float sum7_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			float p_i = 0;
			float subsum257 = 0;
			float subsum4 = 0;

			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				p_i += adjMat[i * AdjMatSideLength + j];
				subsum257 += j * adjMat[i * AdjMatSideLength + j];
				subsum4 += j * j * adjMat[i * AdjMatSideLength + j];
			}

			mean_ += i * p_i;
			sum1_ += i * i * i * i * p_i;
			sum2_ += i * i * i * subsum257;
			sum3_ += i * i * i * p_i;
			sum4_ += i * i * subsum4;
			sum5_ += i * i * subsum257;
			sum6_ += i * i * p_i;
			sum7_ += i * subsum257;
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1 / divCoef;

		mean_ -= (i + j) / divCoef;
		sum1_ -= (i * i * i * i + j * j * j * j) / divCoef;
		sum2_ -= i * i * i * j * normalisedUnit;
		sum2_ -= j * j * j * i * normalisedUnit;
		sum3_ -= (i * i * i + j * j * j) / divCoef;
		sum4_ -= i * i * 2 * j * j * normalisedUnit;
		sum5_ -= i * i * j * normalisedUnit;
		sum5_ -= j * j * i * normalisedUnit;
		sum6_ -= (i * i + j * j) / divCoef;
		sum7_ -= i * j * normalisedUnit;
		sum7_ -= j * i * normalisedUnit;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1 / divCoef;

		mean_ += (i + j) / divCoef;
		sum1_ += (i * i * i * i + j * j * j * j) / divCoef;
		sum2_ += i * i * i * j * normalisedUnit;
		sum2_ += j * j * j * i * normalisedUnit;
		sum3_ += (i * i * i + j * j * j) / divCoef;
		sum4_ += i * i * 2 * j * j * normalisedUnit;
		sum5_ += i * i * j * normalisedUnit;
		sum5_ += j * j * i * normalisedUnit;
		sum6_ += (i * i + j * j) / divCoef;
		sum7_ += 2 * i * j * normalisedUnit;
	}

	float GetValue() const noexcept override
	{
		return pow(std::max(0.f,
			2 * sum1_ + 
			8 * sum2_ - 
			16 * mean_ * sum3_ +
			6 * sum4_ -
			48 * mean_ * sum5_ +
			48 * mean_ * mean_ * (sum6_ + sum7_) -
			48 * mean_ * mean_ * mean_ * mean_), 
			1.f / 4
		);
	}

	void Clear() noexcept override
	{
		mean_ = 0;
		sum1_ = 0;
		sum2_ = 0;
		sum3_ = 0;
		sum4_ = 0;
		sum5_ = 0;
		sum6_ = 0;
		sum7_ = 0;
	}
};

