#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/Timestep.h"

#include "Network/CustomClient.h"

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

	//static float CursorCenteredHorizontally(int size);
	//static float CursorCenteredVertically(int size);

	void AddMessage(std::string message);

	void SendOneMessage();

	void TypingBar();

	void PrintMessage(const std::string& message);

	void BlockEvents(bool block) { m_BlockEvents = block; }

	void SetDarkThemeColors();

private:
	int NextMessageY = 5;
	std::vector<std::string> Messages;
	bool m_BlockEvents = true;
	float m_Time = 0.0f;
	char str[100] = "";
	bool setFocus = true;
	bool FlushMessage = false;
	bool scroll = false;

	std::shared_ptr<CustomClient> client;
};