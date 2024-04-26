#include "pch.h"
#include "AdjacencyMatrix.h"


void AdjacencyMatrix::CalculateHorizontalAdjacency(
	const CyclicBuffer& buffer,
	const KernelSizes& kernelSizes,
	u64 xOffsetPx)
{
	auto& matrix = (*this);

	for (u64 i = 0; i < kernelSizes.sideSize; i++)
	{
		u64 currentRowIndex = buffer.CalculateBufferRowIndex(i);

		for (u64 j = xOffsetPx; j < xOffsetPx + kernelSizes.sideSize - 1; j++)
		{
			matrix(buffer(j, currentRowIndex), buffer(j + 1, currentRowIndex))++;
			matrix(buffer(j + 1, currentRowIndex), buffer(j, currentRowIndex))++;
		}
	}

	// Нормировка
	for (u64 i = 0; i < SideLength; i++)
	{
		for (u64 j = 0; j < SideLength; j++)
		{
			matrix(j, i) /= (kernelSizes.sideSize - 1) * kernelSizes.sideSize * 2;
		}
	}
}

void AdjacencyMatrix::CalculateVerticalAdjacency(
	const CyclicBuffer& buffer,
	const KernelSizes& kernelSizes,
	u64 xOffsetPx)
{
	auto& matrix = (*this);

	for (u64 j = xOffsetPx; j < kernelSizes.sideSize + xOffsetPx; j++)
	{
		for (u64 i = 0; i < kernelSizes.sideSize - 1; i++)
		{
			u64 currentRowIndex = buffer.CalculateBufferRowIndex(i);

			matrix(buffer(i, currentRowIndex), buffer(i + 1, currentRowIndex))++;
			matrix(buffer(i + 1, currentRowIndex), buffer(i, currentRowIndex))++;
		}
	}

	// Нормировка
	for (u64 i = 0; i < SideLength; i++)
	{
		for (u64 j = 0; j < SideLength; j++)
		{
			matrix(j, i) /= (kernelSizes.sideSize - 1) * kernelSizes.sideSize * 2;
		}
	}
}
