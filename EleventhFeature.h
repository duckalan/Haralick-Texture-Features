#pragma once
#include "HaralickFeature.h"
class EleventhFeature final :
    public HaralickFeature
{
private:
    float sumsForNs_[AdjMatSideLength - 1]{};
    float mainSum_{};

public:
    void Init(const std::vector<float>& adjMat) noexcept override
    {
        for (i64 n = 1; n < AdjMatSideLength - 1; n++)
        {
            float elementSum1 = 0;
            float elementSum2 = 0;
            for (i64 i1 = n, j2 = n; i1 < AdjMatSideLength && j2 < AdjMatSideLength; i1++, j2++)
            {
                elementSum1 += adjMat[i1 * AdjMatSideLength + i1 - n];
                elementSum2 += adjMat[(j2 - n) * AdjMatSideLength + j2];
            }
            if (elementSum1 > 0 || elementSum2 > 0)
            {
                mainSum_ += log2(elementSum1 + elementSum2) * (elementSum1 + elementSum2);
                sumsForNs_[n - 1] += elementSum1 + elementSum2;
            }
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        i32 n = abs(i - j);
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
        i32 n = abs(i - j);
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
        for (size_t i = 0; i < AdjMatSideLength - 1; i++)
        {
            sumsForNs_[i] = 0;
        }
    }
};

