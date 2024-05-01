#pragma once
#include "HaralickFeature.h"
#include <iostream>
#include <format>
class SecondFeature :
    public HaralickFeature
{
private:
    float sum_{};

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
            sum_ += n * n * elementSum1;
            sum_ += n * n * elementSum2;
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        int n = abs(static_cast<int>(i) - static_cast<int>(j));
        sum_ -= 2 * n * n * normalisedUnit;
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        int n = abs(static_cast<int>(i) - static_cast<int>(j));
        sum_ += 2 * n * n * normalisedUnit;
    }

    float GetValue() const noexcept override
    {
        return sqrt(sum_);
    }

    void Clear() noexcept override
    {
        sum_ = 0;
    }
};

