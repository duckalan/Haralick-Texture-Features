#include "pch.h"
#include "SecondaryDiagonalAdjacency.h"

void SecondaryDiagonalAdjacency::Init(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 i = 0; i < kernelSideSize_ - 1; i++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(i);
		u64 y2 = buffer.CalculateBufferRowIndex(i + 1);

		for (u64 x = xOffsetPx + 1; x < xOffsetPx + kernelSideSize_; x++)
		{
			u8 i = buffer(x, y1);
			u8 j = buffer(x - 1, y2);

			matrix(j, i) += normalisedUnit;
			matrix(i, j) += normalisedUnit;
		}
	}
	feature_->Init(matrix_);
}

void SecondaryDiagonalAdjacency::RemoveFirstCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_ - 1; k++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(k);
		u64 y2 = buffer.CalculateBufferRowIndex(k + 1);
		u8 i = buffer(xOffsetPx + 1, y1);
		u8 j = buffer(xOffsetPx, y2);

		feature_->RemoveElement(j, i, matrix(i, j), matrix(j, i), GetAdjacentPixelCount());
		matrix(j, i) -= normalisedUnit;
		matrix(i, j) -= normalisedUnit;
	}
}

void SecondaryDiagonalAdjacency::AddLastCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	u64 lastX = xOffsetPx + kernelSideSize_ - 1;
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_ - 1; k++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(k);
		u64 y2 = buffer.CalculateBufferRowIndex(k + 1);
		u8 i = buffer(lastX, y1);
		u8 j = buffer(lastX - 1, y2);

		matrix(j, i) += normalisedUnit;
		matrix(i, j) += normalisedUnit;
		feature_->AddElement(j, i, matrix(i, j), matrix(j, i), GetAdjacentPixelCount());
	}
}