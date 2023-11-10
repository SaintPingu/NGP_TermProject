#pragma once
class Scene;
class Loading;

class SceneManager {
	SINGLETON_PATTERN(SceneManager)

private:
	RECT rectClientWindow{};
	std::shared_ptr<Scene> crntScene{};
	std::shared_ptr<Loading> loading{};

	void StartRender(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap);
	void FinishRender(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap);
public:
	const RECT& GetRectWindow() { return rectClientWindow; }

	void Init(const HWND& hWnd);

	void LoadScene(SceneType scene);
	void RenderScene(const HWND& hWnd);
	void AnimateScene();
};

#define rectWindow SceneManager::Inst()->GetRectWindow()