#include "ImGuiLayer.h"

#include "TempGui/imgui.h"
#include "TempGui/imgui_impl_glfw.h"
#include "TempGui/imgui_impl_opengl3.h"

#include "Core/Application.h"

#include "Core/Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)

ImGuiLayer::ImGuiLayer() : app(Application::Get())
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

	SetDarkThemeColors();

	io.Fonts->AddFontFromFileTTF("../../../assets/fonts/OpenSans-Bold.ttf", 23.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../../../assets/fonts/OpenSans-Regular.ttf", 23.0f);

	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	client = app.client;
}

void ImGuiLayer::OnEvent(Event& e)
{
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
		client->MessageAll(name, str);
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

	if (app.WindowWidth >= 400 && app.WindowHeight >= 400)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
		
		ImGuiStyle& style = ImGui::GetStyle();
		
		style.Colors[ImGuiCol_WindowBg] = { 0.2f, 0.2f, 0.2f, 1.0f };
		style.Colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.0f };
		style.WindowBorderSize = 0.0f;
		
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(app.WindowWidth, app.WindowHeight - 50));

		ImGui::Begin("Window", nullptr, window_flags);

		if (!connected)
			StartingWindow();
		TypingBar();
		CustomiseWindow();

		NextMessageY = 5;

		for (unsigned int i = 0; i < Messages.size(); ++i)
			PrintMessage(Messages[i]);

		if (scroll)
		{
			ImGui::SetScrollY(NextMessageY + 10000);
			scroll = false;
		}

		ImGui::End();


		if (FlushMessage)
		{
			str[0] = '\0';
			setFocus = true;

			ImGui::SetKeyboardFocusHere();

			FlushMessage = false;
		}
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
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ImGuiLayer::StartingWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = { 0.1f, 0.1f, 0.1f, 1.0f };
	style.Colors[ImGuiCol_FrameBg] = { 0.4f, 0.4f, 0.4f, 1.0f };

	ImGui::SetNextWindowPos(ImVec2((app.WindowWidth - 300) / 2, (app.WindowHeight - 200) / 2));
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImGui::SetNextWindowFocus();


	ImGui::Begin("Starting Window", nullptr, window_flags);

	ImGui::PushItemWidth(285);

	ImGui::InputTextWithHint("##label IP", "IP Adress", ipAddress, IM_ARRAYSIZE(ipAddress));

	ImGui::InputInt("##label port", &portNr, NULL, NULL);

	ImGui::PopItemWidth();

	float size = ImGui::CalcTextSize("Connect").x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;
	
	float off = (avail - size) / 2;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60);

	if (ImGui::Button("Connect"))
	{
		Application& app = Application::Get();
		app.Connect(ipAddress, portNr);
	}

	ImGui::End();
}

void ImGuiLayer::TypingBar()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = { 0.2f, 0.2f, 0.2f, 1.0f };
	style.Colors[ImGuiCol_FrameBg] = { 0.4f, 0.4f, 0.4f, 1.0f };

	ImGui::SetNextWindowPos(ImVec2(0, app.WindowHeight - 50));
	ImGui::SetNextWindowSize(ImVec2(app.WindowWidth, app.WindowHeight * 0.53));
	
	ImGui::Begin("TypingBar", nullptr, window_flags);

	ImGui::PushItemWidth(app.WindowWidth - 115);

	if (ImGui::GetTime() > 0.1 && setFocus)
	{
		setFocus = false;
		ImGui::SetKeyboardFocusHere();
	}

	ImGui::SetCursorPosX(15);
	
	ImGui::InputTextWithHint("", "Message", str, IM_ARRAYSIZE(str));

	ImGui::PopItemWidth();

	ImGui::SameLine();
	if (ImGui::Button("Send", ImVec2(75, 29)))
		SendOneMessage();

	ImGui::End();

}

void ImGuiLayer::CustomiseWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	int windowHeight = app.WindowHeight * 0.44;
	int windowWidth = 180;

	ImGui::SetNextWindowPos(ImVec2(app.WindowWidth - 180 - 14, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::Begin("CustomizeWindow", nullptr, window_flags);

	ImGui::PushItemWidth(180);
	ImGui::InputTextWithHint("", "Type Name", name, IM_ARRAYSIZE(name));

	ImGui::PopItemWidth();
	ImGui::End();

}

void ImGuiLayer::PrintMessage(const std::string& message)
{
	int boxWidth, boxHeight;

	ImVec2 a = ImGui::CalcTextSize(message.c_str(), (const char*)0, false, std::min(350, app.WindowWidth - 50));

	boxWidth = a.x;
	boxHeight = a.y;
	
	ImGui::SetCursorPos(ImVec2(25, NextMessageY + 5));
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + std::min(350, app.WindowWidth - 50));
	ImGui::Text(message.c_str(), boxWidth);

	NextMessageY += boxHeight + 5;

	ImGui::PopTextWrapPos();
}

void ImGuiLayer::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	//Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };
	
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