#pragma once
#include "HaralickFeature.h"

class SixthFeature :
    public HaralickFeature
{
private:
    float sum_{};

public:
    void Init(const std::vector<float>& adjMat) noexcept override
    {
        for (i64 n = 2; n < AdjMatSideLength; n++)
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

            sum_ += n * elementSum;
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

            sum_ += n * elementSum;
            k++;
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        i32 n = i + j;
        sum_ -= 2 * n * normalisedUnit;
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        i32 n = i + j;
        sum_ += 2 * n * normalisedUnit;
    }

    float GetValue() const noexcept override
    {
        return sum_;
    }

    void Clear() noexcept override
    {
        sum_ = 0;
    }
};

