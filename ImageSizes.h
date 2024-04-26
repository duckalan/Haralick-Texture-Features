#pragma once
#include "pch.h"
#include "BmpHeader.h"
#include "KernelSizes.h"

struct ImageSizes
{
	i32 widthPx;
	i32 extendedWidthPx;
	i32 heightPx;
	i32 paddingByteCount;

	ImageSizes(
		const BmpHeader& header,
		const KernelSizes& kernelSizes) :

		widthPx(header.imageWidthPx),
		extendedWidthPx(header.imageWidthPx + static_cast<i32>(kernelSizes.radius) * 2),
		heightPx(header.imageHeightPx),
		paddingByteCount(header.GetPaddingByteCount())
	{}

	i32 WidthBytes() const noexcept
	{
		return widthPx * 3;
	}

	i32 ExtendedWidthBytes() const noexcept
	{
		return extendedWidthPx * 3;
	}

	i32 HeightBytes() const noexcept
	{
		return heightPx * 3;
	}
};