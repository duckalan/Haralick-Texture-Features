#pragma once
#include "pch.h"
#include "HaralickFeature.h"
class ThirdFeature :
	public HaralickFeature
{
private:
	float mean_{};
	float meanOfSquares_{};
	float ijgSum_{};

public:
	void Init(const std::vector<float>& adjMat) noexcept override
	{
		for (u64 i = 0; i < AdjMatSideLength; i++)
		{
			float rowSum = 0.f;
			for (u64 j = 0; j < AdjMatSideLength; j++)
			{
				rowSum += adjMat[i * AdjMatSideLength + j];
				ijgSum_ += i * j * adjMat[i * AdjMatSideLength + j];

			}
			mean_ += i * rowSum;
			meanOfSquares_ += i * i * rowSum;
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ -= (i + j) / divCoef;
		meanOfSquares_ -= (i * i + j * j) / divCoef;
		ijgSum_ -= 2 * i * j * (1 / divCoef);
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		mean_ += (i + j) / divCoef;
		meanOfSquares_ += (i * i + j * j) / divCoef;
		ijgSum_ += 2 * i * j * (1 / divCoef);
	}

	float GetValue() const noexcept override
	{
		float rmsd = sqrt(std::max(0.f, meanOfSquares_ - mean_ * mean_));
		rmsd = rmsd == 0 ? 1 : rmsd;

		return (ijgSum_ - mean_ * mean_) / (rmsd * rmsd);
	}

	void Clear() noexcept override
	{
		mean_ = 0;
		meanOfSquares_ = 0;
		ijgSum_ = 0;
	}
};

