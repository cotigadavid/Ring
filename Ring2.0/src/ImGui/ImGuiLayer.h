#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/Timestep.h"

#include "Network/CustomClient.h"

class Application;

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnEvent(Event& e);
	void OnUpdate(Timestep ts);
	void OnAttach();
	void OnDetach();
	void OnImGuiRender();

	bool OnKeyPressed(KeyPressedEvent& e);

	void Begin();
	void End();

	void AddMessage(std::string message);

	void SendOneMessage();

	void StartingWindow();
	void TypingBar();
	void CustomiseWindow();

	void PrintMessage(const std::string& message);

	void SetDarkThemeColors();

private:
	Application& app;

	std::vector<std::string> Messages;
	
	int NextMessageY = 5;
	char str[1000] = "";
	char name[100] = "Default Name";

	bool setFocus = true;
	bool FlushMessage = false;
	bool scroll = false;
	
	std::shared_ptr<CustomClient> client;

	char ipAddress[15] = "127.0.0.1";
	int portNr = 60000;

public:
	bool connected = false;
};