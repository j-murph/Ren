#pragma once

class ZBuffer
{
private:
	std::unique_ptr<float[]> buffer = std::make_unique<float[]>(width * height);

	int width;
	int height;

public:
	ZBuffer();
	~ZBuffer();

	void Init(int width, int height);

	void SetSize(int width, int height);

	void Clear(float value = 0.0f);

	__forceinline void SetDepth(int x, int y, float depth);
};

void ZBuffer::SetDepth(int x, int y, float depth)
{
	buffer[x + y * height] = depth;
}
