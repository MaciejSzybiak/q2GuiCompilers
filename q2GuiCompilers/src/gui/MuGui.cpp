#include "Core.h"
#include "MuGui.h"

namespace Q2Compilers {

#pragma warning( push )
#pragma warning( disable : 26812 )
	static std::unordered_map<int, char> buttonMap = {
		{ GLFW_MOUSE_BUTTON_LEFT, MU_MOUSE_LEFT },
		{ GLFW_MOUSE_BUTTON_RIGHT, MU_MOUSE_RIGHT },
		{ GLFW_MOUSE_BUTTON_MIDDLE, MU_MOUSE_MIDDLE },
	};

	static std::unordered_map<int, char> keyMap = {
		{ GLFW_KEY_LEFT_SHIFT, MU_KEY_SHIFT },
		{ GLFW_KEY_RIGHT_SHIFT, MU_KEY_SHIFT },
		{ GLFW_KEY_LEFT_SHIFT, MU_KEY_SHIFT },
		{ GLFW_KEY_RIGHT_SHIFT, MU_KEY_SHIFT },
		{ GLFW_KEY_LEFT_ALT, MU_KEY_ALT },
		{ GLFW_KEY_RIGHT_ALT, MU_KEY_ALT },
		{ GLFW_KEY_ENTER, MU_KEY_RETURN },
		{ GLFW_KEY_BACKSPACE, MU_KEY_BACKSPACE },
	};
#pragma warning( pop )

	TextWidthFunc MuGui::_widthFunc;

	MuGui::MuGui(TextWidthFunc widthFunc, int xSize, int ySize, std::string name)
		: _width(xSize), _height(ySize), _name(name)
	{
		ASSERT(widthFunc, "No width func specified");

		_context = new mu_Context();
		mu_init(_context);

		_widthFunc = widthFunc;

		_context->text_width = TextWidth;
		_context->text_height = TextHeight;
	}

	void MuGui::HandleEvent(std::shared_ptr<Event> e)
	{
		switch (e->GetType())
		{
		case EventType::MouseButtonPressed:
		{
			auto event = std::dynamic_pointer_cast<MouseButtonPressedEvent>(e);
			int btn = event->GetButton();
			if (buttonMap.find(btn) != buttonMap.end()) {
				mu_input_mousedown(_context, event->GetX(), event->GetY(), buttonMap.at(btn));
			}
			//LOG_TRACE("mousedown %d %d", event->GetX(), event->GetY());
			break;
		}
		case EventType::MouseButtonReleased:
		{
			auto event = std::dynamic_pointer_cast<MouseButtonReleasedEvent>(e);
			int btn = event->GetButton();
			if (buttonMap.find(btn) != buttonMap.end()) {
				mu_input_mouseup(_context, event->GetX(), event->GetY(), buttonMap.at(btn));
			}
			break;
		}
		case EventType::MouseScrolled:
		{
			auto event = std::dynamic_pointer_cast<MouseScrolledEvent>(e);
			mu_input_scroll(_context, (int)(event->GetX() * -30), (int)(event->GetY() * -30));
			break;
		}
		case EventType::MouseMoved:
		{
			auto event = std::dynamic_pointer_cast<MouseMovedEvent>(e);
			mu_input_mousemove(_context, event->GetX(), event->GetY());
			break;
		}
		case EventType::KeyPressed:
		{
			auto event = std::dynamic_pointer_cast<KeyPressedEvent>(e);
			int key = event->GetKey();
			if (!(event->IsRepeating()) && keyMap.find(key) != keyMap.end()) {
				mu_input_keydown(_context, keyMap.at(key));
			}
			break;
		}
		case EventType::KeyReleased:
		{
			auto event = std::dynamic_pointer_cast<KeyReleasedEvent>(e);
			int key = event->GetKey();
			if (keyMap.find(key) != keyMap.end()) {
				mu_input_keyup(_context, keyMap.at(key));
			}
			break;
		}
		default:
		{
			LOG_ERROR("Unhandled event type");
			__debugbreak();
			break;
		}
		}
	}

	bool MuGui::MakeCommands(MuGuiData* data)
	{
		mu_begin(_context);
		mu_Container* win = mu_get_container(_context, _name.c_str());
		win->rect = mu_rect(WINDOW_BORDER, WINDOW_BORDER, _width - WINDOW_BORDER * 2, _height - WINDOW_BORDER * 2);
		
		if (mu_begin_window_ex(_context, _name.c_str(), mu_rect(0, 0, 0, 0),
			MU_OPT_NORESIZE)) {
			int widths[] = { -1 };
			mu_layout_row(_context, 1, widths, -1);
			mu_begin_panel(_context, "console");
			mu_Container* panel = mu_get_current_container(_context);
			mu_layout_row(_context, 1, widths, -1);
			mu_text(_context, "This is\na test text.\n\nAnd it works!");
			mu_end_panel(_context);

			mu_end_window(_context);
		}
		else
		{
			//close the window
			return false;
		}
		mu_end(_context);

		return true;
	}
}
