#pragma once

#include "OpenGL/OpenGLContext.h"
#include "Events/Event.h"

#include "GLFW/glfw3.h"

#include <string>
#include <functional>

using EventCallbackFn = std::function<void(Event&)>;

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "Ring2.0", unsigned int width = 1000, unsigned int height = 562)
	{
		Title = title;
		Width = width;
		Height = height;
	}
};

struct WindowData
{
	std::string Title;
	unsigned int Width, Height;
	int posX, posY;
	bool VSync;

	EventCallbackFn EventCallback;
};

class Window
{
public:

	Window(const WindowProps& props);
	~Window();

	void OnUpdate();
	unsigned int GetWidth() const { return m_Data.Width; }
	unsigned int GetHeight() const { return m_Data.Height; }

	void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

	static Window* Create(const WindowProps& props = WindowProps());

	inline GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
	void Init(const WindowProps& props);
	void Shutdown();

	OpenGLContext* m_Context;
	GLFWwindow* m_Window;
	WindowData m_Data;
};