#pragma once
class Scene;
class Loading;

class SceneManager {
private:
	SceneType crntSceneType{};
	SceneType nextSceneType{};
	std::shared_ptr<Scene> crntScene{};
	std::shared_ptr<Loading> loading{};

	void StartRender(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap);
	void FinishRender(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap);
	void InitScene(SceneType scene);
public:
	void Init(HWND hWnd);

	void LoadScene(SceneType scene);
	void RenderScene(HWND hWnd);
	void AnimateScene();

	void DisConnect();

	const std::shared_ptr<Scene>& GetCurrentScene() const { return crntScene; }
	SceneType GetCurrentSceneType() { return crntSceneType; }

	bool IsLoading();
};
