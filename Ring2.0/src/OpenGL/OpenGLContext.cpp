#include "OpenGLContext.h"

#include "Core/Log.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "gl/GL.h"

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	RING_ASSERT(windowHandle, "Window handle is null");
}

void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	RING_ASSERT(status, "Failed to intialize Glad");
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
