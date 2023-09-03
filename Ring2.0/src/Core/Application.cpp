#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Log.h"
#include "Input.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;

	m_Running = true;
	m_Minimized = false;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	client = std::make_shared<CustomClient>();
	client->Connect("127.0.0.1", 60000);

	m_ImguiLayer = new ImGuiLayer();
	m_ImguiLayer->OnAttach();

}
bool once = true;
void Application::Run()
{
	while (m_Running)
	{
		glfwGetWindowPos(Application::Get().GetWindow().GetNativeWindow(), &WindowPosX, &WindowPosY);
		glfwGetWindowSize(Application::Get().GetWindow().GetNativeWindow(), &WindowWidth, &WindowHeight);

		if (client->IsConnected())
		{
			if (!client->Incoming().empty())
			{
				auto msg = client->Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAccept:
					std::cout << "Server Accepted Connection\n";
					break;
				case CustomMsgTypes::ServerPing:
				{
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				}
				break;
				case CustomMsgTypes::ServerMessage:
				{
					uint32_t clientID;
					uint32_t sizeofsentence;
					uint8_t a;

					std::string s;

					msg >> sizeofsentence;
					
					while (sizeofsentence)
					{
						sizeofsentence--;
						msg >> a;
						s += a;
					}
					
					std::reverse(s.begin(), s.end());

					msg >> clientID;
					std::cout << s << " from [" << clientID << "]\n";
					m_ImguiLayer->AddMessage("anoth: " + s);
					//std::cout << a << " from [" << clientID << "]\n";
				}
				break;
				
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
		}

		m_ImguiLayer->Begin();
		m_ImguiLayer->OnImGuiRender();
		m_ImguiLayer->End();

		m_Window->OnUpdate();

		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::Enter)
	{
		//client->MessageAll();
		m_ImguiLayer->OnKeyPressed(e);
		
	}
	return true;
}

bool Application::OnWindowClosed(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		RING_ERROR("Minimized");
		return false;
	}

	m_Minimized = false;

	float width = (float)e.GetWidth();
	float height = (float)e.GetHeight();

	/*glViewport(0, 0, width, height);

	float aspectRatio = width / height;
	m_ProjectionData.SetProjection(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f);*/

	return false;
}

Application::~Application()
{

}