#pragma once
#include "pch.h"

class HaralickFeature
{
public:
	static std::string GetFeatureName(u64 featureNumber)
	{
		if (featureNumber != 0 && featureNumber <= 14)
		{
			std::string hf{ "hf" };
			return hf + std::to_string(featureNumber);
		}
		return "";
	}

	virtual void Init(const std::vector<float>& adjMat) noexcept = 0;

	virtual void RemoveElement(
		u8 i, 
		u8 j, 
		float g_ij,
		float g_ji, 
		float divCoef
	) noexcept = 0;

	virtual void AddElement(
		u8 i, 
		u8 j, 
		float g_ij, 
		float g_ji, 
		float divCoef
	) noexcept = 0;

	inline virtual float GetValue() const noexcept = 0;

	inline virtual void Clear() noexcept = 0;

	// ДОБАВИТЬ UPDATE С СИГНАТУРОЙ ADDELEMENT ПУСТОЙ ПО УМОЛЧАНИЮ
	// ДОБАВИТЬ ISUPDATENEEDED С FALSE ПО УМОЛЧАНИЮ
	// ИСПОЛЬЗОВАТЬ ИХ ДЛЯ 13 И 14 ПРИЗНАКОВ

protected:
	static const u64 AdjMatSideLength = 256;
};