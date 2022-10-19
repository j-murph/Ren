#pragma once

class ITickable
{
public:
	virtual ~ITickable() = default;

	virtual void Tick() = 0;
};