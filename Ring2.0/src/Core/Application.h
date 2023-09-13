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

	inline static Application& Get() { return *s_Instance; }

	void Run();

	void OnEvent(Event& e);

	void Connect(const char* ipAddress, int portNr);

	void Close() { m_Running = false; }

	Window& GetWindow() { return *p_Window; }

	
private:
	bool OnWindowClosed(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);

private:
	ImGuiLayer* p_ImguiLayer;
	bool m_Running;
	std::unique_ptr<Window> p_Window;

	static Application* s_Instance;

public:
	std::shared_ptr<CustomClient> client;
	std::string m_ClientName;

	int WindowPosX, WindowPosY;
	int WindowWidth, WindowHeight;
};