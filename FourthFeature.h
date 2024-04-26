#pragma once
#include "pch.h"
#include "HaralickFeature.h"
class FourthFeature :
	public HaralickFeature
{
private:
	float mean_{};
	float meanOfSquares_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			float rowSum = 0.f;
			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				rowSum += adjMat[i * AdjMatSideLength + j];;
			}
			mean_ += i * rowSum;
			meanOfSquares_ += i * i * rowSum;
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ -= (i + j) / divCoef;
		meanOfSquares_ -= (i * i + j * j) / divCoef;
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ += (i + j) / divCoef;
		meanOfSquares_ += (i * i + j * j) / divCoef;
	}

	float GetValue() const noexcept override
	{
		return sqrt(std::max(0.f, meanOfSquares_ - mean_ * mean_));
	}

	void Clear() noexcept override
	{
		mean_ = 0;
		meanOfSquares_ = 0;
	}
};