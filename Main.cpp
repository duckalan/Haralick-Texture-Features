#include "pch.h"
#include "AdjacencyMatrix.h"
#include "BmpHeader.h"
#include "CyclicBuffer.h"
#include "FourthFeature.h"
#include "FirstFeature.h"
#include "ThirdFeature.h"
#include "HorizontalAdjacency.h"
#include "ImageSizes.h"
#include "KernelSizes.h"
#include "MainDiagonalAdjacency.h"
#include "SecondaryDiagonalAdjacency.h"
#include "VerticalAdjacency.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <format>

void GrayscaleImage(const std::filesystem::path& imagePath)
{
	std::filesystem::path outputPath = (
		imagePath.parent_path() / imagePath.stem().concat("_grayscaled")
		).concat(imagePath.extension().native());

	std::ifstream input(imagePath, std::ios_base::binary);
	if (!input.is_open())
	{
		throw std::exception("Can't open image file");
	}
	std::ofstream output(outputPath, std::ios_base::binary);
	if (!output.is_open())
	{
		throw std::exception("Can't open output image file");
	}

	BmpHeader header{};
	input.read((char*)&header, BmpHeader::SizeBytes);

	if (header.dibHeaderSizeBytes != BmpHeader::DibHeaderSizeBytes)
	{
		throw std::exception("Can't work with bmp of this type. Use bmp only with BITMAPINFOHEADER");
	}
	output.write((char*)&header, sizeof(header));

	std::vector<u8> inputRow(header.GetStride());
	std::vector<u8> outputRow(header.GetStride());

	for (u64 y = 0; y < header.imageHeightPx; y++)
	{
		input.read((char*)inputRow.data(), sizeof(u8) * inputRow.size());

		for (u64 x = 0; x < header.imageWidthPx; x++)
		{
			u8 gray = u8(std::clamp(
				inputRow[x * 3] * 0.114f
				+ inputRow[x * 3 + 1] * 0.587f
				+ inputRow[x * 3 + 2] * 0.299f,
				0.f,
				255.f
			) + 0.5f);

			outputRow[x * 3] = gray;
			outputRow[x * 3 + 1] = gray;
			outputRow[x * 3 + 2] = gray;
		}

		output.write((char*)outputRow.data(), sizeof(u8) * outputRow.size());
	}
}

void CalculateHaralickFeature(
	const std::filesystem::path& inputPath,
	const std::filesystem::path& outputPath,
	u64 kernelSideSize,
	float amplifyCoef)
{
	if (kernelSideSize % 2 == 0)
	{
		throw std::exception("Can't work with kernel with side divisible by two");
	}

	std::ifstream input(inputPath, std::ios::binary);
	if (!input.is_open())
	{
		std::string errorMessage = "Can't find or open file with input path: ";
		throw std::invalid_argument(errorMessage + inputPath.string());
	}
	std::ofstream output(outputPath, std::ios::binary);
	if (!output.is_open())
	{
		std::string errorMessage = "Can't save file with output path: ";
		throw std::invalid_argument(errorMessage + outputPath.string());
	}

	BmpHeader header{};
	input.read((char*)&header, sizeof(header));
	if (header.dibHeaderSizeBytes != BmpHeader::DibHeaderSizeBytes)
	{
		throw std::exception("Can't work with bmp of this type. Use bmp only with BITMAPINFOHEADER");
	}

	KernelSizes kernelSizes(kernelSideSize);
	// Проверка на возможность отражения
	if (kernelSizes.radius > header.imageWidthPx ||
		kernelSizes.radius > header.imageHeightPx)
	{
		throw std::exception("Изображение слишком мало");
	}

	output.write((char*)&header, sizeof(header));

	CyclicBuffer cyclicBuffer(
		kernelSizes,
		ImageSizes(header, kernelSizes),
		input
	);
	std::vector<u8> outputRow(header.GetStride());
	std::unique_ptr<AdjacencyMatrix> adjMatrices[4]
	{
		std::make_unique<HorizontalAdjacency>(
			HorizontalAdjacency{kernelSideSize, std::make_unique<ThirdFeature>(ThirdFeature{})}
		),
		std::make_unique<VerticalAdjacency>(
			VerticalAdjacency{kernelSideSize, std::make_unique<ThirdFeature>(ThirdFeature{})}
		),
		std::make_unique<MainDiagonalAdjacency>(
			MainDiagonalAdjacency{kernelSideSize, std::make_unique<ThirdFeature>(ThirdFeature{})}
		),
		std::make_unique<SecondaryDiagonalAdjacency>(
			SecondaryDiagonalAdjacency{kernelSideSize, std::make_unique<ThirdFeature>(ThirdFeature{})}
		),
	};

	for (u64 y = kernelSizes.radius;
		y < header.imageHeightPx + kernelSizes.radius;
		y++)
	{
		if (y % 100 == 0)
		{
			std::cout << std::format(
				"Current progress: {:3.2f}%\r",
				static_cast<float>(y) / (header.imageHeightPx - 1) * 100.f
			);
		}

		// Начальный расчёт матриц и статистики
		for (auto& adjMat : adjMatrices)
		{
			adjMat->Init(cyclicBuffer, 0);
		}

		u64 xOffsetPx = 0;
		for (u64 x = kernelSizes.radius;
			x < header.imageWidthPx + kernelSizes.radius;
			x++)
		{
			float resultValue = 0;
			for (auto& adjMat : adjMatrices)
			{
				resultValue += adjMat->GetFeatureValue();
			}
			u8 resultColor = (u8)(
				std::clamp(amplifyCoef * (resultValue / 4.f) + 0.5f, 
					0.f, 
					255.f
				));

			// Запись в выходную строку
			outputRow[(x - kernelSizes.radius) * 3] = resultColor;
			outputRow[(x - kernelSizes.radius) * 3 + 1] = resultColor;
			outputRow[(x - kernelSizes.radius) * 3 + 2] = resultColor;

			// Удаление смежностей первого столбца в окне
			for (auto& adjMat : adjMatrices)
			{
				adjMat->RemoveFirstCol(cyclicBuffer, xOffsetPx);
			}
			
			xOffsetPx++;

			if (xOffsetPx < header.imageWidthPx)
			{
				// Добавление смежностей последнего столбца сдвинутого окна
				for (auto& adjMat : adjMatrices)
				{
					adjMat->AddLastCol(cyclicBuffer, xOffsetPx);
				}
			}
		}

		cyclicBuffer.PrepareNextRow(y, input);

		output.write(
			reinterpret_cast<char*>(outputRow.data()),
			sizeof(u8) * outputRow.size()
		);

		// Очистка матриц и статистики
		for (auto& matrix : adjMatrices)
		{
			matrix->Clear();
		}
	}
}

int main()
{
	//GrayscaleImage("C:\\Users\\Duck\\Desktop\\test images\\crop.bmp");
	CalculateHaralickFeature(
		"H:\\ImageTest\\crop_grayscaled.bmp",
		"H:\\ImageTest\\hf3____.bmp",
		5, 25
	);
}