#pragma once
#include "HaralickFeature.h"
class EighthFeature final :
	public HaralickFeature
{
private:
    float sumsForNs_[AdjMatSideLength * 2 - 1]{};
    float mainSum_{};

public:
    void Init(const std::vector<float>& adjMat) noexcept override
    {
        for (i64 n = 1; n < AdjMatSideLength; n++)
        {
            i64 i = 0;
            i64 j = n;
            float elementSum = 0;

            while (i <= n)
            {
                elementSum += adjMat[i * AdjMatSideLength + j];
                i++;
                j--;
            }

            if (elementSum > 0)
            {
                mainSum_ += log2(elementSum) * elementSum;
                sumsForNs_[n - 1] = elementSum;
            }
        }

        i64 k = 1;
        for (i64 n = AdjMatSideLength; n < AdjMatSideLength * 2; n++)
        {
            i64 i = k;
            i64 j = AdjMatSideLength - 1;
            float elementSum = 0;

            while (i < AdjMatSideLength)
            {
                elementSum += adjMat[i * AdjMatSideLength + j];
                i++;
                j--;
            }

            if (elementSum > 0)
            {
                mainSum_ += log2(elementSum) * elementSum;
                sumsForNs_[n - 1] = elementSum;
            }
            k++;
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        i32 n = i + j;
        if (n != 0)
        {
            float normalisedUnit = 1.f / divCoef;
            float oldSum = sumsForNs_[n - 1];
            sumsForNs_[n - 1] -= 2 * normalisedUnit;

            if (sumsForNs_[n - 1] <= 0)
            {
                mainSum_ -= log2(oldSum) * oldSum;
            }
            else
            {
                mainSum_ -= log2(oldSum) * oldSum - log2(sumsForNs_[n - 1]) * sumsForNs_[n - 1];
            }
        }
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        i32 n = i + j;
        if (n != 0)
        {
            float normalisedUnit = 1.f / divCoef;

            float oldSum = sumsForNs_[n - 1];
            sumsForNs_[n - 1] += 2 * normalisedUnit;

            if (oldSum <= 0)
            {
                mainSum_ += log2(sumsForNs_[n - 1]) * sumsForNs_[n - 1];
            }
            else
            {
                mainSum_ += log2(sumsForNs_[n - 1]) * sumsForNs_[n - 1] - log2(oldSum) * oldSum;
            }
        }
    }

	float GetValue() const noexcept override
	{
		return -mainSum_;
	}

	void Clear() noexcept override
	{
		mainSum_ = 0;
        for (size_t i = 0; i < AdjMatSideLength * 2 - 1; i++)
        {
            sumsForNs_[i] = 0;
        }
	}
};

