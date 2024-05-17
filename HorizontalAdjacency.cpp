#include "pch.h"
#include "HorizontalAdjacency.h"

void HorizontalAdjacency::Init(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 i = 0; i < kernelSideSize_; i++)
	{
		u64 y = buffer.CalculateBufferRowIndex(i);

		for (u64 x = xOffsetPx; x < xOffsetPx + kernelSideSize_ - 1; x++)
		{
			u8 i = buffer(x, y);
			u8 j = buffer(x + 1, y);

			matrix(j, i) += normalisedUnit;
			matrix(i, j) += normalisedUnit;
		}
	}
	feature_->Init(matrix_);
}

void HorizontalAdjacency::RemoveFirstCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_; k++)
	{
		u64 y = buffer.CalculateBufferRowIndex(k);
		u8 i = buffer(xOffsetPx, y);
		u8 j = buffer(xOffsetPx + 1, y);

		feature_->RemoveElement(i, j, matrix(i, j), matrix(j, i), static_cast<float>(GetAdjacentPixelCount()));
		matrix(j, i) -= normalisedUnit;
		matrix(i, j) -= normalisedUnit;
	}
}

void HorizontalAdjacency::AddLastCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	u64 preLastX = xOffsetPx + kernelSideSize_ - 2;
	u64 lastX = preLastX + 1;
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_; k++)
	{
		u64 y = buffer.CalculateBufferRowIndex(k);
		u8 i = buffer(preLastX, y);
		u8 j = buffer(lastX, y);

		matrix(j, i) += normalisedUnit;
		matrix(i, j) += normalisedUnit;
		feature_->AddElement(i, j, matrix(i, j), matrix(j, i), static_cast<float>(GetAdjacentPixelCount()));
	}
}