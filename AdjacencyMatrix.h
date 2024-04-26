#pragma once
#include "CyclicBuffer.h"
#include "KernelSizes.h"
#include "HaralickFeature.h"

class AdjacencyMatrix
{
public:
	static const u64 SideLength = 256;

	virtual void Init(
		const CyclicBuffer& buffer,
		u64 xOffsetPx
	) = 0;

	virtual void RemoveFirstCol(
		const CyclicBuffer& buffer,
		u64 xOffsetPx
	) = 0;

	virtual void AddLastCol(
		const CyclicBuffer& buffer,
		u64 xOffsetPx
	) = 0;

	void Clear() noexcept
	{
		std::fill(matrix_.begin(), matrix_.end(), 0.f);
		feature_->Clear();
	}

	virtual u64 GetAdjacentPixelCount() const noexcept = 0;

	float operator()(u64 x, u64 y) const
	{
		return matrix_[y * SideLength + x];
	}

	float& operator()(u64 x, u64 y)
	{
		return matrix_[y * SideLength + x];
	}

	float GetFeatureValue() const noexcept
	{
		return feature_->GetValue();
	}

protected:
	AdjacencyMatrix(u64 kernelSideSize, std::unique_ptr<HaralickFeature> featurePtr) noexcept : 
		matrix_(SideLength* SideLength),
		kernelSideSize_(kernelSideSize),
		feature_(std::move(featurePtr))
	{}

	u64 kernelSideSize_;
	std::unique_ptr<HaralickFeature> feature_;
	std::vector<float> matrix_;
};