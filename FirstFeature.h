#pragma once
#include "pch.h"
#include "HaralickFeature.h"
class FirstFeature :
    public HaralickFeature
{
private:
    float sumOfSquaredElements{};

public:
    void Init(const std::vector<float>& adjMat) noexcept override
    {
        for (u64 i = 0; i < AdjMatSideLength; i++)
        {
            for (u64 j = 0; j < AdjMatSideLength; j++)
            {
                sumOfSquaredElements += adjMat[i * AdjMatSideLength + j] * adjMat[i * AdjMatSideLength + j];
            }
        }
    }

    void RemoveElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        sumOfSquaredElements -= 2 * g_ij * normalisedUnit + normalisedUnit * normalisedUnit;
        sumOfSquaredElements -= 2 * g_ji * normalisedUnit + normalisedUnit * normalisedUnit;
    }

    void AddElement(u8 i, u8 j, float g_ij, float g_ji, float divCoef) noexcept override
    {
        float normalisedUnit = 1.f / divCoef;
        float old_g_ij = g_ij - normalisedUnit;
        float old_g_ji = g_ji - normalisedUnit;

        sumOfSquaredElements += 2 * old_g_ij * normalisedUnit + normalisedUnit * normalisedUnit;
        sumOfSquaredElements += 2 * old_g_ji * normalisedUnit + normalisedUnit * normalisedUnit;
    }

    float GetValue() const noexcept override
    {
        return sumOfSquaredElements;
    }

    void Clear() noexcept override
    {
        sumOfSquaredElements = 0;
    }
};

