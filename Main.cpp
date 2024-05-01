#include "pch.h"
#include "MainProgramFunctions.h"

// 1 - input image file name [string], 
// 2 - feature number [1 --- 14, u64], 
// 3 - kernel side size [3 --- (image size / 2), must be odd, u64], 
// 4 - amplify coefficient for calculation result [float], 
// 5 - perform grayscaling before calculating feature [0 or 1]
int main(int argc, char* argv[])
{
	if (argc == 5)
	{
		MainProgramFunctions::CalculateHaralickFeature(
			argv[1],
			std::stoull(argv[2]),
			std::stoull(argv[3]),
			std::stof(argv[4])
		);
		
	}
	else if (argc == 6)
	{
		if (std::stoi(argv[5]) == '1')
		{
			std::filesystem::path imagePath{ argv[1] };
			std::filesystem::path grayscaledPath = (
				imagePath.parent_path() / imagePath.stem().concat("_grayscaled")
				).concat(imagePath.extension().native());

			MainProgramFunctions::GrayscaleImage(imagePath);
			MainProgramFunctions::CalculateHaralickFeature(
				grayscaledPath,
				std::stoull(argv[2]),
				std::stoull(argv[3]),
				std::stof(argv[4])
			);
			std::filesystem::remove(grayscaledPath);
		}
		else
		{
			MainProgramFunctions::CalculateHaralickFeature(
				argv[1],
				std::stoull(argv[2]),
				std::stoull(argv[3]),
				std::stof(argv[4])
			);
		}
	}
}