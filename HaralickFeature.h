#pragma once
#include "pch.h"

class HaralickFeature
{
public:
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

protected:
	static const u64 AdjMatSideLength = 256;
};