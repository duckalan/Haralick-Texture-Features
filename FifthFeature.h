#pragma once
#include "pch.h"
#include "HaralickFeature.h"

class FifthFeature final :
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
                sum_ += adjMat[i * AdjMatSideLength + j] / (1 + (i - j) * (i - j));
            }
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        sum_ -= normalisedUnit / (1 + (i - j) * (i - j));
        sum_ -= normalisedUnit / (1 + (j - i) * (j - i));
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        sum_ += normalisedUnit / (1 + (i - j) * (i - j));
        sum_ += normalisedUnit / (1 + (j - i) * (j - i));
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

