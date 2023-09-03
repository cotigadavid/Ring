#pragma once

#include "Window/Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include "Network/CustomClient.h"

#include "ImGui/ImGuiLayer.h"

#include <memory>

class Application
{
public:
	Application();
	~Application();

	void Run();
	void OnEvent(Event& e);

	inline static Application& Get() { return *s_Instance; }

	Window& GetWindow() { return *m_Window; }

	void Close() { m_Running = false; }

private:
	bool OnWindowClosed(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);

private:
	bool m_Running;
	bool m_Minimized;
	static Application* s_Instance;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImguiLayer;

public:
	std::shared_ptr<CustomClient> client;

public:
	int WindowPosX, WindowPosY;
	int WindowWidth, WindowHeight;
};