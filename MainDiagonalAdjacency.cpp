#include "pch.h"
#include "MainDiagonalAdjacency.h"

void MainDiagonalAdjacency::Init(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 i = 0; i < kernelSideSize_ - 1; i++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(i);
		u64 y2 = buffer.CalculateBufferRowIndex(i + 1);

		for (u64 x = xOffsetPx; x < xOffsetPx + kernelSideSize_ - 1; x++)
		{
			u8 i = buffer(x, y1);
			u8 j = buffer(x + 1, y2);

			matrix(j, i) += normalisedUnit;
			matrix(i, j) += normalisedUnit;
		}
	}
	feature_->Init(matrix_);
}

void MainDiagonalAdjacency::RemoveFirstCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_ - 1; k++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(k);
		u64 y2 = buffer.CalculateBufferRowIndex(k + 1);
		u8 i = buffer(xOffsetPx, y1);
		u8 j = buffer(xOffsetPx + 1, y2);

		feature_->RemoveElement(i, j, matrix(i, j), matrix(j, i), static_cast<float>(GetAdjacentPixelCount()));
		matrix(j, i) -= normalisedUnit;
		matrix(i, j) -= normalisedUnit;
	}
}

void MainDiagonalAdjacency::AddLastCol(const CyclicBuffer& buffer, u64 xOffsetPx)
{
	auto& matrix = (*this);
	u64 preLastX = xOffsetPx + kernelSideSize_ - 2;
	u64 lastX = preLastX + 1;
	float normalisedUnit = 1.f / GetAdjacentPixelCount();

	for (u64 k = 0; k < kernelSideSize_ - 1; k++)
	{
		u64 y1 = buffer.CalculateBufferRowIndex(k);
		u64 y2 = buffer.CalculateBufferRowIndex(k + 1);
		u8 i = buffer(preLastX, y1);
		u8 j = buffer(lastX, y2);

		matrix(j, i) += normalisedUnit;
		matrix(i, j) += normalisedUnit;
		feature_->AddElement(i, j, matrix(i, j), matrix(j, i), static_cast<float>(GetAdjacentPixelCount()));
	}
}