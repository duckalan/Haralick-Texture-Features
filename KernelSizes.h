#pragma once
#include "pch.h"

struct KernelSizes
{
	u64 sideSize;
	u64 radius;

	KernelSizes(u64 sideSize) noexcept
		: sideSize(sideSize), radius(sideSize / 2)
	{}
};