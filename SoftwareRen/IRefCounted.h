#pragma once

class IRefCounted
{
public:
	virtual ~IRefCounted() = default;

	virtual int AddRef() = 0;
	virtual void Release() = 0;
};

class AutoReleaser
{
private:
	IRefCounted* instance;

public:
	explicit AutoReleaser(IRefCounted* instance) : instance(instance)
	{
		instance->AddRef();
	}

	AutoReleaser(const AutoReleaser&) = delete;
	AutoReleaser(AutoReleaser&&) = delete;

	~AutoReleaser()
	{
		instance->Release();
	}
};