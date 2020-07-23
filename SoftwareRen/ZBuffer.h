#pragma once

class ZBuffer
{
private:
	float* buffer;

	int width;
	int height;

public:
	ZBuffer();
	~ZBuffer();

	void Init(int width, int height);

	void SetSize(int width, int height);

	void Clear(float value = 0.0f);

	__forceinline void SetDepth(int x, int y, float depth)
	{
		buffer[x + y * height] = depth;
	}
};
