#include "pch.h"
#include "MainProgramFunctions.h"

#include "BmpHeader.h"
#include "CyclicBuffer.h"
#include "ImageSizes.h"
#include "KernelSizes.h"

#include "FirstFeature.h"
#include "SecondFeature.h"
#include "ThirdFeature.h"
#include "FourthFeature.h"
#include "FifthFeature.h"
#include "SixthFeature.h"
#include "SeventhFeature.h"
#include "EighthFeature.h"
#include "NinthFeature.h"
#include "TenthFeature.h"
#include "EleventhFeature.h"
#include "TwelfthFeature.h"
#include "ThirteenthFeature.h"

#include "AdjacencyMatrix.h"
#include "HorizontalAdjacency.h"
#include "VerticalAdjacency.h"
#include "MainDiagonalAdjacency.h"
#include "SecondaryDiagonalAdjacency.h"

#include <array>
#include <iostream>

void MainProgramFunctions::InitFeaturesArray(std::array<std::unique_ptr<HaralickFeature>, 4>& features, u64 featureNumber)
{
	for (u64 i = 0; i < 4; i++)
	{
		switch (featureNumber)
		{
		case 1ull:
			features[i] = std::make_unique<FirstFeature>(FirstFeature{});
			break;
		case 2ull:
			features[i] = std::make_unique<SecondFeature>(SecondFeature{});
			break;
		case 3ull:
			features[i] = std::make_unique<ThirdFeature>(ThirdFeature{});
			break;
		case 4ull:
			features[i] = std::make_unique<FourthFeature>(FourthFeature{});
			break;
		case 5ull:
			features[i] = std::make_unique<FifthFeature>(FifthFeature{});
			break;
		case 6ull:
			features[i] = std::make_unique<SixthFeature>(SixthFeature{});
			break;
		case 7ull:
			features[i] = std::make_unique<SeventhFeature>(SeventhFeature{});
			break;
		case 8ull:
			features[i] = std::make_unique<EighthFeature>(EighthFeature{});
			break;
		case 9ull:
			features[i] = std::make_unique<NinthFeature>(NinthFeature{});
			break;
		case 10ull:
			features[i] = std::make_unique<TenthFeature>(TenthFeature{});
			break;
		case 11ull:
			features[i] = std::make_unique<EleventhFeature>(EleventhFeature{});
			break;
		case 12ull:
			features[i] = std::make_unique<TwelfthFeature>(TwelfthFeature{});
			break;
		case 13ull:
			features[i] = std::make_unique<ThirteenthFeature>(ThirteenthFeature{});
			break;
		default:
			break;
		}
	}
}

std::filesystem::path MainProgramFunctions::CreateOutputPath(std::filesystem::path imagePath, u64 featureNumber, u64 kernelSideSize, float amplifyCoef)
{
	std::stringstream ss;
	ss << " ["
		<< HaralickFeature::GetFeatureName(featureNumber)
		<< "; kss "
		<< kernelSideSize
		<< "; amp "
		<< std::fixed << std::setprecision(2) << amplifyCoef
		<< ']';

	return (imagePath.parent_path() / imagePath.stem().concat(ss.str()))
		.concat(imagePath.extension().native());
}

void MainProgramFunctions::GrayscaleImage(const std::filesystem::path& imagePath)
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

void MainProgramFunctions::CalculateHaralickFeature(
	const std::filesystem::path& inputPath,
	u64 featureNumber,
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
	std::ofstream output(
		CreateOutputPath(inputPath, featureNumber, kernelSideSize, amplifyCoef),
		std::ios::binary
	);
	if (!output.is_open())
	{
		std::string errorMessage = "Can't save file";
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

	std::array<std::unique_ptr<HaralickFeature>, 4> features{};
	InitFeaturesArray(features, featureNumber);

	std::unique_ptr<AdjacencyMatrix> adjMatrices[4] {
		std::make_unique<HorizontalAdjacency>(HorizontalAdjacency(kernelSideSize, std::move(features[0]))),
		std::make_unique<VerticalAdjacency>(VerticalAdjacency(kernelSideSize, std::move(features[1]))),
		std::make_unique<MainDiagonalAdjacency>(MainDiagonalAdjacency(kernelSideSize, std::move(features[2]))),
		std::make_unique<SecondaryDiagonalAdjacency>(SecondaryDiagonalAdjacency(kernelSideSize, std::move(features[3]))),
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