#pragma once
#include "Window.h"
#include "ITickable.h"

class RenWindow : public Window, public ITickable
{
private:
	std::unique_ptr<SRGraphicsContext> pSrgc;
	std::unique_ptr<Rasterizer> pRasterizer;
	std::unique_ptr<SceneRenderer> pSceneRenderer;

	Camera mainCamera;

	RenWindow();

	void Render();
	void HandlePaint();
	void HandleWindowResize(LPARAM lParam, WPARAM wParam);
	void HandleKeyDown(WPARAM wParam);
	
	void UpdateCamera();
	void UpdateTitle(int currentFps);
	void ToggleViewMode();

protected:
	virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	static RenWindow* CreateInstance() { return new RenWindow; }

	virtual bool Create(int width, int height, LPCTSTR title) override;

	virtual void Tick() override;
};
