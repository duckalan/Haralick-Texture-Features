#pragma once
#include "HaralickFeature.h"

class ThirteenthFeature final :
	public HaralickFeature
{
private:
	double mean_{};
	double sumOfSquareSumMulPij_{};
	double sumOfCubeSumMulPij_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			double p_i = 0;

			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				p_i += adjMat[i * AdjMatSideLength + j];
				sumOfSquareSumMulPij_ += (i + j) * (i + j)
					* adjMat[i * AdjMatSideLength + j];
				sumOfCubeSumMulPij_ += (i + j) * (i + j) * (i + j)
					* adjMat[i * AdjMatSideLength + j];
			}

			mean_ += i * p_i;
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		double normalisedUnit = 1.0 / divCoef;
		int64_t ii = i;
		int64_t jj = j;

		mean_ -= (ii + jj) / divCoef;
		sumOfSquareSumMulPij_ -= 2 * (ii + jj) * (ii + jj) * normalisedUnit;
		sumOfCubeSumMulPij_ -= 2 * (ii + jj) * (ii + jj) * (ii + jj) * normalisedUnit;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		double normalisedUnit = 1.0 / divCoef;
		int64_t ii = i;
		int64_t jj = j;

		mean_ += (ii + jj) / divCoef;
		sumOfSquareSumMulPij_ += 2 * (ii + jj) * (ii + jj) * normalisedUnit;
		sumOfCubeSumMulPij_ += 2 * (ii + jj) * (ii + jj) * (ii + jj) * normalisedUnit;
	}

	float GetValue() const noexcept override
	{
		return cbrt(sumOfCubeSumMulPij_
			- 6 * mean_ * sumOfSquareSumMulPij_
			+ 16 * mean_ * mean_ * mean_);
	}

	void Clear() noexcept override
	{
		mean_ = 0;
		sumOfSquareSumMulPij_ = 0;
		sumOfCubeSumMulPij_ = 0;
	}
};
