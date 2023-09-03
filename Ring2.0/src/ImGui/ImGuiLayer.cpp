#include "ImGuiLayer.h"

#include "TempGui/imgui.h"
#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"

#include "Core/Application.h"

#include "Core/Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)

ImGuiLayer::ImGuiLayer()
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::OnAttach()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("../assets/fonts/OpenSans-Bold.ttf", 23.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../assets/fonts/OpenSans-Regular.ttf", 23.0f);

	Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	client = app.client;

	Messages.push_back("fsafaf");
	Messages.push_back("fsafaf");
	Messages.push_back("fsafaf");
	Messages.push_back("fsafaf");
}

void ImGuiLayer::OnEvent(Event& e)
{
	if (m_BlockEvents)
	{
		ImGuiIO& io = ImGui::GetIO();
		e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
}

bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::Enter)
	{
		SendOneMessage();
	}
	return true;
}

void ImGuiLayer::AddMessage(std::string message)
{
	Messages.push_back(message);
}

void ImGuiLayer::SendOneMessage()
{
	if (strlen(str) != 0)
	{
		client->MessageAll(str);
		FlushMessage = true;
		AddMessage(std::string("Me: ") + std::string(str));
		scroll = true;
	}
}

void ImGuiLayer::OnUpdate(Timestep ts)
{
}

void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnImGuiRender()
{
	Application& app = Application::Get();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1000, 517));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });

	ImGui::Begin("Window", nullptr, window_flags);

	//ImGui::InputTextWithHint("", "enter text here", str, IM_ARRAYSIZE(str));
	TypingBar();

	NextMessageY = 5;

	for (unsigned int i = 0; i < Messages.size(); ++i)
		PrintMessage(Messages[i]);

	if (app.WindowWidth)
		ImGui::SetWindowFontScale(app.WindowWidth / 800);

	if (scroll)
	{
		ImGui::SetScrollY(NextMessageY + 10000);
		scroll = false;
	}

	ImGui::End();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();

	if (FlushMessage)
	{
		str[0] = '\0';
		setFocus = true;

		ImGui::SetKeyboardFocusHere();

		FlushMessage = false;
	}
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	/*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}*/
}

void ImGuiLayer::TypingBar()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ;
	//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus /*| ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar*/;

	//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 1.0f, 1.0f, 1.0f, 0.1f });
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.5f);

	Application& app = Application::Get();

	//ImGui::SetNextWindowPos(ImVec2(app.WindowPosX + app.WindowWidth / 3.57, app.WindowPosY + app.WindowHeight - app.WindowHeight / 13));
	ImGui::SetNextWindowPos(ImVec2(0, 515));
	ImGui::SetNextWindowSize(ImVec2(1000, 300));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });

	ImGui::Begin("TypingBar", nullptr, window_flags);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushItemWidth(app.WindowWidth - 100);

	//ImGui::SetCursorPosY(500);
	//ImGui::SetCursorPosX(18);

	//str[0] = '\0';

	if (ImGui::GetTime() > 0.1 && setFocus)
	{
		setFocus = false;
		ImGui::SetKeyboardFocusHere();
	}

	//if (app.WindowWidth)
		//ImGui::SetWindowFontScale(app.WindowWidth / 800 * 1.25);

	//ImGui::SetWindowFontScale(30);
	ImGui::SetCursorPosX(5);
	
	ImGui::InputTextWithHint("", "Message", str, IM_ARRAYSIZE(str));

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopItemWidth();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f });

	ImGui::SameLine();
	if (ImGui::Button("Send", ImVec2(75, 29)))
		SendOneMessage();

	ImGui::SameLine();

	ImGui::PopStyleColor(3);

	ImGui::End();

	ImGui::PopStyleColor();

	//ImGui::PopStyleColor();
	//ImGui::PopStyleVar();
}

void ImGuiLayer::PrintMessage(const std::string& message)
{
	Application& app = Application::Get();

	if (app.WindowWidth)
		ImGui::SetWindowFontScale(app.WindowWidth / 800);

	int NextMessageX;
	int boxWidth, boxHeight;

	ImVec2 a = ImGui::CalcTextSize(message.c_str(), (const char*)0, false, 350);

	boxWidth = a.x;
	boxHeight = a.y;

	NextMessageX = 20;

	//ImGui::SetCursorPos(ImVec2(NextMessageX, NextMessageY));
	//ImGui::Image((void*)TextBackground->GetRendererID(), ImVec2(boxWidth + 10, boxHeight + 10));

	ImGui::SetCursorPos(ImVec2(NextMessageX + 5, NextMessageY + 5));
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 350);
	ImGui::Text(message.c_str(), boxWidth);

	NextMessageY += boxHeight + 5;

	ImGui::SetWindowFontScale(1);
}

void ImGuiLayer::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}