#pragma once
#include "HaralickFeature.h"
class ThirteenthFeature final :
    public HaralickFeature
{
private:
	float mean_{};
	float sum1_{};
	float sum2_{};
	float sum3_{};
	float sum4_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			float p_i = 0;
			float subsum24 = 0;

			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				p_i += adjMat[i * AdjMatSideLength + j];
				subsum24 += j * adjMat[i * AdjMatSideLength + j];
			}

			mean_ += i * p_i;
			sum1_ += i * i * i * p_i;
			sum2_ += i * i * subsum24;
			sum3_ += i * i * p_i;
			sum4_ += i * subsum24;
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1 / divCoef;

		mean_ -= (i + j) / divCoef;
		sum1_ -= (i * i * i + j * j * j) / divCoef;
		sum3_ -= (i * i + j * j) / divCoef;
		sum4_ -= i * 2 * j * normalisedUnit;
		sum2_ -= i * i * j * normalisedUnit;
		sum2_ -= j * j * i * normalisedUnit;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1 / divCoef;

		mean_ += (i + j) / divCoef;
		sum1_ += (i * i * i + j * j * j) / divCoef;
		sum3_ += (i * i + j * j) / divCoef;
		sum4_ += i * 2 * j * normalisedUnit;
		sum2_ += i * i * j * normalisedUnit;
		sum2_ += j * j * i * normalisedUnit;
	}

	float GetValue() const noexcept override
	{
		return cbrt(
			2 * sum1_ +
			6 * sum2_ -
			12 * mean_ * (sum3_ + sum4_) +
			16 * mean_ * mean_ * mean_);
	}

	void Clear() noexcept override
	{
		mean_ = 0;
		sum1_ = 0;
		sum2_ = 0;
		sum3_ = 0;
		sum4_ = 0;
	}
};

