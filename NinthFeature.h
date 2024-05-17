#pragma once
#include "HaralickFeature.h"
class NinthFeature :
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
				float element = adjMat[i * AdjMatSideLength + j] < std::numeric_limits<float>::epsilon()
					? 0
					: adjMat[i * AdjMatSideLength + j];

				float log = adjMat[i * AdjMatSideLength + j] <= 0 ? 0 : log2(element);

				sum_ += element * log;
			}
		}
	}

	void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1.f / divCoef;
		float g1 = g_ij;
		float g2 = g_ij - normalisedUnit;
		if (i == j)
		{
			g2 -= normalisedUnit;

			if (g2 <= 0)
			{
				sum_ -= g1 * log2(g1);
			}
			else
			{
				sum_ -= g1 * log2(g1) - g2 * log2(g2);
			}
		}
		else
		{
			if (g2 <= 0)
			{
				sum_ -= 2 * (g1 * log2(g1));
			}
			else
			{
				sum_ -= 2 * (g1 * log2(g1) - g2 * log2(g2));
			}
		}
	}

	void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
	{
		float normalisedUnit = 1.f / divCoef;
		float g1 = g_ij;
		float g2 = g_ij - normalisedUnit;
		if (i == j)
		{
			g2 -= normalisedUnit;

			if (g2 <= 0)
			{
				sum_ += g1 * log2(g1);
			}
			else
			{
				sum_ += g1 * log2(g1) - g2 * log2(g2);
			}
		}
		else
		{
			if (g2 <= 0)
			{
				sum_ += 2 * (g1 * log2(g1));
			}
			else
			{
				sum_ += 2 * (g1 * log2(g1) - g2 * log2(g2));
			}
		}
	}

	float GetValue() const noexcept override
	{
		return -sum_;
	}

	void Clear() noexcept override
	{
		sum_ = 0;
	}
};

