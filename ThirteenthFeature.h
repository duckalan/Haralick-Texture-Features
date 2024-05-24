#pragma once
#include "HaralickFeature.h"
class ThirteenthFeature final :
    public HaralickFeature
{
private:
	float mean_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		float testSum1 = 0;

		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			float rowSum = 0.f;
			float testRowSum = 0.f;
			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				rowSum += adjMat[i * AdjMatSideLength + j];
				testRowSum += adjMat[i * AdjMatSideLength + j] * adjMat[i * AdjMatSideLength + j] * adjMat[i * AdjMatSideLength + j];
			}
			mean_ += i * rowSum;
			testSum1 += i * i * i * testRowSum;
			int a = 5;
		}
		float b = mean_ * mean_ * mean_;

		float testSum2 = 0;

		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				testSum2 += mean_ * adjMat[i * AdjMatSideLength + j];
				if (testSum2 != 0)
				{
					int a = 5;
				}
			}
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ -= (i + j) / divCoef;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ += (i + j) / divCoef;
	}

	float GetValue() const noexcept override
	{
		return 0;
	}

	void Clear() noexcept override
	{
		mean_ = 0;
	}
};

