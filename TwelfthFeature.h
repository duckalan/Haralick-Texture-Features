#pragma once
#include "pch.h"
#include "HaralickFeature.h"

class TwelfthFeature final :
	public HaralickFeature
{
private:
	float sum_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (i64 i = 0; i < AdjMatSideLength; i++)
		{
			for (i64 j = 0; j < AdjMatSideLength; j++)
			{
				sum_ += (i - j) * (i - j) * adjMat[i * AdjMatSideLength + j];
			}
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1.f / divCoef;
		sum_ -= (i - j) * (i - j) * normalisedUnit;
		sum_ -= (j - i) * (j - i) * normalisedUnit;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1.f / divCoef;
		sum_ += (i - j) * (i - j) * normalisedUnit;
		sum_ += (j - i) * (j - i) * normalisedUnit;
	}

	float GetValue() const noexcept override
	{
		return sqrt(std::max(0.f, sum_));
	}

	void Clear() noexcept override
	{
		sum_ = 0;
	}
};

