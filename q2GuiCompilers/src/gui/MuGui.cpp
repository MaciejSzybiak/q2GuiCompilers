#include "Core.h"
#include "microui.h"
#include "MuGui.h"

#define WINDOW_BORDER 2

namespace Q2Compilers
{

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
			if (buttonMap.find(btn) != buttonMap.end())
			{
				mu_input_mousedown(_context, event->GetX(), event->GetY(), buttonMap.at(btn));
			}
			break;
		}
		case EventType::MouseButtonReleased:
		{
			auto event = std::dynamic_pointer_cast<MouseButtonReleasedEvent>(e);
			int btn = event->GetButton();
			if (buttonMap.find(btn) != buttonMap.end())
			{
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

			if (keyMap.find(key) != keyMap.end())
			{
				mu_input_keydown(_context, keyMap.at(key));
			}
			break;
		}
		case EventType::KeyReleased:
		{
			auto event = std::dynamic_pointer_cast<KeyReleasedEvent>(e);
			int key = event->GetKey();
			if (keyMap.find(key) != keyMap.end())
			{
				mu_input_keyup(_context, keyMap.at(key));
			}
			break;
		}
		case EventType::TextInput:
		{
			//microui expects a null terminated string
			static char keyChar[2] = { '\0', '\0' };
			auto event = std::dynamic_pointer_cast<TextInputEvent>(e);
			int key = event->GetKey();
			keyChar[0] = (char)key;
			mu_input_text(_context, keyChar);
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
		static int widths[] = { -1 };

		if (mu_begin_window_ex(_context, _name.c_str(), mu_rect(0, 0, 0, 0),
			MU_OPT_NORESIZE))
		{
			mu_layout_row(_context, 1, widths, -200);
			mu_begin_panel(_context, "Main panel");

			if (mu_header(_context, "Profiles"))
			{
				DrawProfilesPanel(data, win);
			}
			if (mu_header(_context, "Map"))
			{
				DrawMapPanel(data);
			}
			if (mu_header(_context, "Game"))
			{
				DrawGamePanel(data);
			}
			if (mu_header(_context, "BSP stage"))
			{
				DrawQradPanel(data);
			}
			if (mu_header(_context, "VIS stage"))
			{
				DrawVisPanel(data);
			}
			if (mu_header(_context, "Radiosity"))
			{

			}
			mu_end_panel(_context);

			mu_layout_row(_context, 1, widths, -25);
			DrawConsolePanel();

			//compile button
			mu_layout_row(_context, 1, widths, -1);
			if (mu_button(_context, data->isCompiling ? "Cancel" : "Compile!"))
			{
				if (!data->isCompiling)
				{
					data->compile = true;
					data->isCompiling = true;
				}
				else
				{
					data->isCompiling = false; //abort
				}
			}

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

	int MuGui::DrawIntSlider(int* value, float low, float high)
	{
		static float tmp;
		mu_push_id(_context, &value, sizeof(value));
		tmp = (float)*value;
		int res = mu_slider_ex(_context, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
		*value = (int)tmp;
		mu_pop_id(_context);
		return res;
	}

	void MuGui::DrawSliderWithLabel(const char* label, int* value, float low, float high)
	{
		static int l[] = { 0, -1 };
		const int widths[] = { -1 };
		mu_layout_row(_context, 1, widths, 30);

		mu_begin_panel_ex(_context, label, 0);

		l[0] = mu_get_current_container(_context)->content_size.x / 2;
		mu_layout_row(_context, 2, l, 0);

		mu_label(_context, label);
		DrawIntSlider(value, low, high);

		mu_end_panel(_context);
	}

	void MuGui::DrawSliderWithLabel(const char* label, float* value, float low, float high)
	{
		static int l[] = { 0, -1 };
		const int widths[] = { -1 };
		mu_layout_row(_context, 1, widths, 30);

		mu_begin_panel_ex(_context, label, 0);

		l[0] = mu_get_current_container(_context)->content_size.x / 2;
		mu_layout_row(_context, 2, l, 0);

		mu_label(_context, label);
		mu_slider(_context, value, low, high);

		mu_end_panel(_context);
	}

	void MuGui::DrawConsolePanel()
	{
		const int widths[] = { -1 };
		static int lastLength = 0;

		mu_begin_panel(_context, "console");

		mu_Container* panel = mu_get_current_container(_context);
		mu_layout_row(_context, 1, widths, -1);
		std::string& log = Log::GetLogString();
		mu_text(_context, log.c_str());

		mu_end_panel(_context);
		if (lastLength != log.length())
		{
			panel->scroll.y = panel->content_size.y;
			lastLength = (int)log.length();
		}
	}

	void MuGui::DrawProfilesPanel(MuGuiData* data, mu_Container* window)
	{
		int l[] = { window->content_size.x / 2, -1 };
		mu_layout_row(_context, 2, l, 0);

		if (mu_button(_context, "Load"))
		{
			mu_open_popup(_context, "Load profile");
			data->updateProfileList = true;
		}
		if (mu_button(_context, "Save"))
		{
			mu_open_popup(_context, "Save profile");
			data->updateProfileList = true;
		}
		DrawProfilePopup(true, data, window->content_size.x);
		DrawProfilePopup(false, data, window->content_size.x);
	}

	void MuGui::DrawVisPanel(MuGuiData* data)
	{
		CData* d = data->data;
		int enabled;
		mu_checkbox(_context, "Enable stage", &enabled); //TODO: add storable variable

		//slider
		DrawSliderWithLabel("Threads:", &d->qvis_threads, 0, 8);

		int l[] = { mu_get_current_container(_context)->content_size.x / 2, -1 };
		mu_layout_row(_context, 2, l, 0);

		//toggles
		mu_checkbox(_context, "Verbose output", &d->qvis_qvis_verbose);
		mu_checkbox(_context, "Fast", &d->qvis_fastvis);
		mu_checkbox(_context, "No sorting", &d->qvis_nosort);
		mu_checkbox(_context, "Cull error", &d->qvis_cullerror);
	}

	void MuGui::DrawQradPanel(MuGuiData* data)
	{
		CData* d = data->data;
		int enabled;
		mu_checkbox(_context, "Enable stage", &enabled); //TODO: add storable variable

		int l[] = { mu_get_current_container(_context)->content_size.x / 2, 0 };
		mu_layout_row(_context, 2, l, 0);

		mu_checkbox(_context, "Verbose output", &d->qbsp_verbose);
		mu_checkbox(_context, "GL View", &d->qbsp_glView);
		mu_checkbox(_context, "No welding", &d->qbsp_noweld);
		mu_checkbox(_context, "No detail", &d->qbsp_nodetail);
		mu_checkbox(_context, "No water", &d->qbsp_nowater);
		mu_checkbox(_context, "Leak test", &d->qbsp_leaktest);
	}

	void MuGui::DrawMapPanel(MuGuiData* data)
	{
		std::string& mapStr = data->mapName;
		mapStr.reserve(32);
		char* mapBuf = &mapStr[0];

		const int l[] = { 100, -1 };
		mu_layout_row(_context, 2, l, 0);

		mu_label(_context, "Map file path");
		mu_textbox(_context, mapBuf, (int)mapStr.capacity());
	}

	void MuGui::DrawGamePanel(MuGuiData* data)
	{
		//directory string
		std::string& dirStr = data->data->q2_directory;
		dirStr.reserve(128);
		char *dirBuf = &dirStr[0];

		//modname
		std::string& modName = data->data->q2_modname;
		modName.reserve(32);
		char *modBuf = &modName[0];

		//executable
		std::string& execStr = data->data->q2_executable;
		execStr.reserve(32);
		char* execBuf = &execStr[0];

		//args
		std::string& argsStr = data->data->q2_args;
		argsStr.reserve(256);
		char* argsBuf = &argsStr[0];

		//layout
		const  int l[] = { 100, -1 };
		mu_layout_row(_context, 2, l, 0);

		//row 1
		mu_label(_context, "Directory");
		mu_textbox(_context, dirBuf, (int)dirStr.capacity());

		//row 2
		mu_label(_context, "Mod directory");
		mu_textbox(_context, modBuf, (int)modName.capacity());

		//row 3
		mu_label(_context, "Executable name");
		mu_textbox(_context, execBuf, (int)execStr.capacity());

		//row 3
		mu_label(_context, "Args");
		mu_textbox(_context, argsBuf, (int)argsStr.capacity());
	}

	void MuGui::DrawProfilePopup(bool isLoadPopup, MuGuiData *data, int windowWidth)
	{
		static char buf[64];
		bool isPopup = isLoadPopup ? mu_begin_popup(_context, "Load profile") :
			mu_begin_popup(_context, "Save profile");

		if (isPopup)
		{
			mu_Container* ct = mu_get_current_container(_context);
			ct->rect.x = (windowWidth / 2) - ct->content_size.x / 2;

			mu_label(_context, isLoadPopup ? "Load profle" : "Save profle");

			for (const auto& value : data->profileFiles)
			{
				if (mu_button(_context, value.c_str()))
				{
					data->profileName = value;
					if (isLoadPopup)
					{
						data->loadProfile = true;
					}
					else
					{
						data->saveProfile = true;
					}
					ct->open = 0;
				}
			}
			if (!isLoadPopup)
			{
				if (mu_header(_context, "New..."))
				{
					bool submitted = false;
					if (mu_textbox(_context, buf, sizeof(buf)) & MU_RES_SUBMIT)
					{
						mu_set_focus(_context, _context->last_id);
						submitted = true;
					}
					if (mu_button(_context, "Save new") || submitted)
					{
						data->saveProfile = true;
						data->profileName = buf;
						mu_Container* ct2 = mu_get_current_container(_context);
						ct2->open = 0;
					}
				}
			}
			mu_end_popup(_context);
		}
	}
}
