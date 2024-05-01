#pragma once
#include "HaralickFeature.h"

class MainProgramFunctions
{
private:
	static void InitFeaturesArray(
		std::array<std::unique_ptr<HaralickFeature>, 4>& features,
		u64 featureNumber
	);

	static std::filesystem::path CreateOutputPath(
		std::filesystem::path imagePath,
		u64 featureNumber,
		u64 kernelSideSize,
		float amplifyCoef
	);

public:

	static void GrayscaleImage(const std::filesystem::path& imagePath);

	static void CalculateHaralickFeature(
		const std::filesystem::path& inputPath,
		u64 featureNumber,
		u64 kernelSideSize,
		float amplifyCoef);
};

