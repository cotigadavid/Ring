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
	p_Window = std::unique_ptr<Window>(Window::Create());
	p_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	client = std::make_shared<CustomClient>();
	m_ClientName = "DefaultName";

	p_ImguiLayer = new ImGuiLayer();
	p_ImguiLayer->OnAttach();

}

void Application::Run()
{
	while (m_Running)
	{
		glfwGetWindowPos(Application::Get().GetWindow().GetNativeWindow(), &WindowPosX, &WindowPosY);
		glfwGetWindowSize(Application::Get().GetWindow().GetNativeWindow(), &WindowWidth, &WindowHeight);

 		if (client->IsConnected())
		{
			p_ImguiLayer->connected = true;

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
					uint32_t sizeOfSentence;
					uint32_t sizeOfName;
					uint8_t indChar;

					std::string receivedMessage;
					std::string receivedName;

					msg >> sizeOfSentence;
					
					while (sizeOfSentence)
					{
						sizeOfSentence--;
						msg >> indChar;
						receivedMessage += indChar;
					}
					
					msg >> sizeOfName;

					while (sizeOfName)
					{
						sizeOfName--;
						msg >> indChar;
						receivedName += indChar;
					}

					std::reverse(receivedMessage.begin(), receivedMessage.end());
					std::reverse(receivedName.begin(), receivedName.end());

					msg >> clientID;
					std::cout << receivedMessage << " from [" << receivedName << "]\n";
					p_ImguiLayer->AddMessage(receivedName + ": " + receivedMessage);
					
				}
				break;
				
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
		}

		p_ImguiLayer->Begin();
		p_ImguiLayer->OnImGuiRender();
		p_ImguiLayer->End();

		p_Window->OnUpdate();

		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::Enter)
	{
		p_ImguiLayer->OnKeyPressed(e);
	}
	return true;
}

void Application::Connect(const char* ipAddress, int portNr)
{
	client->Connect(ipAddress, portNr);
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
		RING_TRACE("Minimized");
		return false;
	}

	float width = (float)e.GetWidth();
	float height = (float)e.GetHeight();

	glViewport(0, 0, width, height);

	return false;
}

Application::~Application()
{

}