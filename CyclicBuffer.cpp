#include "pch.h"
#include "BmpHeader.h"
#include "CyclicBuffer.h"
#include "ImageSizes.h"
#include "KernelSizes.h"

CyclicBuffer::CyclicBuffer(
	const KernelSizes& kernelSizes,
	const ImageSizes& imageSizes,
	std::ifstream& input) :

	firstRowIndex_(0),
	imageSizes_(imageSizes),
	kernelSizes_(kernelSizes),
	extendedRows_(imageSizes_.ExtendedWidthBytes()* kernelSizes.sideSize)
{
	auto& rows = (*this);
	input.seekg(sizeof(BmpHeader));

	// Формирование первоначального буфера строк с отражением
	// 1 0 0 1 2
	for (u64 bufferY = kernelSizes.radius; bufferY < kernelSizes.sideSize; bufferY++)
	{
		input.read(
			reinterpret_cast<char*>(&rows(kernelSizes.radius, bufferY)),
			imageSizes.WidthBytes()
		);
		input.seekg(imageSizes.paddingByteCount, std::ios_base::cur);

		MirrorSideEdges(bufferY);
	}

	// Отражение
	for (u64 bufferY = 0; bufferY < kernelSizes.radius; bufferY++)
	{
		u64 y = 2 * kernelSizes.radius - 1 - bufferY;

		std::copy(
			&rows(0, y),
			&rows(0, y + 1),
			&rows(0, bufferY)
		);
	}
}

void CyclicBuffer::PrepareNextRow(u64 currentImageY, std::ifstream& input)
{
	auto& rows = (*this);
	// Чтобы при чётных размерах условие отражение не срабатывало 
	// на 1 строку раньше и последняя строка тоже обрабатывалась 
	u64 imageY = kernelSizes_.sideSize % 2 == 0
		? currentImageY - 1
		: currentImageY;

	// Отражение 
	if (imageY >= imageSizes_.heightPx - 1ull)
	{
		u64 yToCopy{};
		if (imageY == imageSizes_.heightPx - 1ull)
		{
			yToCopy = (firstRowIndex_ + kernelSizes_.sideSize - 1)
				% kernelSizes_.sideSize;
		}
		if (yToCopy != firstRowIndex_)
		{
			std::copy(
				&rows(0, yToCopy),
				&rows(0, yToCopy + 1),
				&rows(0, firstRowIndex_)
			);
		}

		yToCopy--;
		if (yToCopy < 0)
		{
			yToCopy = kernelSizes_.sideSize - 1;
		}
	}
	// Запись новой строки
	else
	{
		input.read(
			reinterpret_cast<char*>(&rows(kernelSizes_.radius, firstRowIndex_)),
			imageSizes_.WidthBytes()
		);
		input.seekg(imageSizes_.paddingByteCount, std::ios_base::cur);

		MirrorSideEdges(firstRowIndex_);
	}

	firstRowIndex_ = (firstRowIndex_ + 1) % kernelSizes_.sideSize;
}

void CyclicBuffer::MirrorSideEdges(u64 rowIndex)
{
	u8* row = &(*this)(0, rowIndex);

	// Левый край
	for (u64 x = 0; x < kernelSizes_.radius; x++)
	{
		u64 mirrorX = kernelSizes_.radius * 2 - 1 - x;

		row[x * 3] = row[mirrorX * 3];
		row[x * 3 + 1] = row[mirrorX * 3 + 1];
		row[x * 3 + 2] = row[mirrorX * 3 + 2];
	}

	// Правый край
	for (u64 x = imageSizes_.widthPx + kernelSizes_.radius;
		x < imageSizes_.widthPx + kernelSizes_.radius * 2;
		x++)
	{
		u64 mirrorX = 2 * (imageSizes_.widthPx + kernelSizes_.radius) - x - 1;

		row[x * 3] = row[mirrorX * 3];
		row[x * 3 + 1] = row[mirrorX * 3 + 1];
		row[x * 3 + 2] = row[mirrorX * 3 + 2];
	}
}