#pragma once
#include "AdjacencyMatrix.h"

class MainDiagonalAdjacency :
    public AdjacencyMatrix
{
public:
    MainDiagonalAdjacency(u64 kernelSideSize, std::unique_ptr<HaralickFeature> featurePtr) noexcept
        : AdjacencyMatrix(kernelSideSize, std::move(featurePtr)) {}

    void Init(
        const CyclicBuffer& buffer,
        u64 xOffsetPx
    ) override;

    void RemoveFirstCol(
        const CyclicBuffer& buffer,
        u64 xOffsetPx
    ) override;

    void AddLastCol(
        const CyclicBuffer& buffer,
        u64 xOffsetPx
    ) override;

    u64 GetAdjacentPixelCount() const noexcept override
    {
        return (kernelSideSize_ - 1) * (kernelSideSize_ - 1) * 2;
    }
};

