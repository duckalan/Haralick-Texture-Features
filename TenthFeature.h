#pragma once
#include "HaralickFeature.h"

class TenthFeature final :
    public HaralickFeature
{
private:
    float sum_{};
    float sumOfSquares_{};

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
            sumOfSquares_ += n * n * elementSum1;
            sumOfSquares_ += n * n * elementSum2;
            sum_ += n * elementSum1;
            sum_ += n * elementSum1;
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        int n = abs(i - j);
        sumOfSquares_ -= 2 * n * n * normalisedUnit;
        sum_ -= 2 * n * normalisedUnit;
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        int n = abs(i - j);
        sumOfSquares_ += 2 * n * n * normalisedUnit;
        sum_ += 2 * n * normalisedUnit;
    }

    float GetValue() const noexcept override
    {
        return sqrt(std::max(0.f, sumOfSquares_ - sum_ * sum_));
    }

    void Clear() noexcept override
    {
        sumOfSquares_ = 0;
        sum_ = 0;
    }
};

