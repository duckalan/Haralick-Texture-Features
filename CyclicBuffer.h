#pragma once
#include "ImageSizes.h"

class CyclicBuffer
{
public:
	enum class Channel : u64
	{
		Blue, Green, Red
	};


	CyclicBuffer(
		const KernelSizes& kernelSizes,
		const ImageSizes& imageSizes,
		std::ifstream& input
	);

	void PrepareNextRow(
		u64 currentImageY,
		std::ifstream& input
	);

	u64 CalculateBufferRowIndex(u64 rowIndex) const noexcept
	{
		return (rowIndex + firstRowIndex_) % kernelSizes_.sideSize;
	}

	// Обращение побайтовое к первому каналу
	u8 operator()(u64 x, u64 y) const
	{
		return extendedRows_[y * imageSizes_.ExtendedWidthBytes() + x * 3];
	}

	u8& operator()(u64 x, u64 y)
	{
		return extendedRows_[y * imageSizes_.ExtendedWidthBytes() + x * 3];
	}

	/*u8& operator()(u64 x, u64 y, Channel channel)
	{
		return extendedRows_[y * imageSizes_.ExtendedWidthBytes() + x * 3 + static_cast<u64>(channel)];
	}*/

private:
	u64 firstRowIndex_;
	ImageSizes imageSizes_;
	KernelSizes kernelSizes_;
	std::vector<u8> extendedRows_;

	void MirrorSideEdges(u64 rowIndex);
};