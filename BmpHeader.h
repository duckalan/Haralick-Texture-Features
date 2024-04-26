#pragma once
#include "pch.h"

#pragma pack(push, 1)
struct BmpHeader
{
	// Для BMP с BITMAPINFOHEADER
	static const i32 SizeBytes = 54;
	// Размер для BITMAPINFOHEADER
	static const i32 DibHeaderSizeBytes = 40;
	static const i32 BytePerPx = 3;

	// BmpFileHeader
	u16 bm;
	u32 fileSizeBytes;
	u16 reserved1;
	u16 reserved2;
	u32 imageOffsetBytes;

	// DibHeader
	u32 dibHeaderSizeBytes;
	i32 imageWidthPx;
	i32 imageHeightPx;
	u16 colorPlanesCount;
	u16 bitPerPixel;
	u32 compressionMethod;
	u32 imageSizeBytes;
	i32 xPixelPerMetre;
	i32 yPixelPerMetre;
	u32 paletteColorsCount;
	u32 importantColorsCount;

	i32 GetStride() const noexcept
	{
		return (imageWidthPx * BytePerPx + 3) & ~3;
	}

	i32 GetPaddingByteCount() const noexcept
	{
		return GetStride() - imageWidthPx * BytePerPx;
	}

	void InitNew(
		i32 newWidthPx,
		i32 newHeightPx,
		BmpHeader& headerToInit
	) const noexcept {
		i32 newStride = (newWidthPx * BytePerPx + 3) & ~3;
		u32 newImageSizeBytes = newHeightPx * newStride;

		headerToInit.bm = bm;
		headerToInit.fileSizeBytes = SizeBytes + newImageSizeBytes;
		headerToInit.reserved1 = reserved1;
		headerToInit.reserved2 = reserved2;
		headerToInit.imageOffsetBytes = imageOffsetBytes;

		headerToInit.dibHeaderSizeBytes = dibHeaderSizeBytes;
		headerToInit.imageWidthPx = newWidthPx;
		headerToInit.imageHeightPx = newHeightPx;
		headerToInit.colorPlanesCount = colorPlanesCount;
		headerToInit.bitPerPixel = bitPerPixel;
		headerToInit.compressionMethod = compressionMethod;
		headerToInit.imageSizeBytes = newImageSizeBytes;
		headerToInit.xPixelPerMetre = xPixelPerMetre;
		headerToInit.yPixelPerMetre = yPixelPerMetre;
		headerToInit.paletteColorsCount = paletteColorsCount;
		headerToInit.importantColorsCount = importantColorsCount;
	}
};
#pragma pack(pop)