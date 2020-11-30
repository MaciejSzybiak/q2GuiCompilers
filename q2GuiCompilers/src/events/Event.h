#pragma once

#include "Core.h"

namespace Q2Compilers {

	enum class EventType {
		MouseButtonPressed, MouseButtonReleased,
		MouseScrolled, MouseMoved,
		KeyPressed, KeyReleased
	};

	class Event {
	public:
		virtual EventType GetType() const = 0;
		virtual std::string ToString() const = 0;
	};

	//------
	//EVENTS
	//------

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button, int x, int y) : _button(button), _xPos(x), _yPos(y) {}

		int GetButton() {
			return _button;
		}
		int GetX() {
			return _xPos;
		}
		int GetY() {
			return _yPos;
		}
	protected:
		int _button;
		int _xPos;
		int _yPos;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int btn, int x, int y) : MouseButtonEvent(btn, x, y) {}

		virtual EventType GetType() const override {
			return EventType::MouseButtonPressed;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "MouseButtonPressed: " << _button;
			return stream.str();
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int btn, int x, int y) : MouseButtonEvent(btn, x, y) {}

		virtual EventType GetType() const override {
			return EventType::MouseButtonReleased;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "MouseButtonReleased: " << _button;
			return stream.str();
		}
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(int x, int y) : _xPos(x), _yPos(y) {}

		virtual EventType GetType() const override {
			return EventType::MouseMoved;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "MouseMoved: " << _xPos << " x " << _yPos;
			return stream.str();
		}

		int GetX() {
			return _xPos;
		}
		int GetY() {
			return _yPos;
		}
	private:
		int _xPos;
		int _yPos;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float x, float y) : _xDelta(x), _yDelta(y) {}

		virtual EventType GetType() const override {
			return EventType::MouseScrolled;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "MouseScrolled: " << _xDelta << " x " << _yDelta;
			return stream.str();
		}

		float GetX() {
			return _xDelta;
		}
		float GetY() {
			return _yDelta;
		}
	private:
		float _xDelta;
		float _yDelta;
	};

	class KeyEvent : public Event {
	public:
		KeyEvent(int keyCode) : _keyCode(keyCode) {}
		int GetKey() {
			return _keyCode;
		}
	protected:
		int _keyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, bool repeats) : KeyEvent(keyCode), _repeats(repeats) {}

		virtual EventType GetType() const override {
			return EventType::KeyPressed;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "KeyPressed: " << ((char)_keyCode) << " repeats: " << _repeats;
			return stream.str();
		}

		bool IsRepeating() {
			return _repeats;
		}
	private:
		bool _repeats;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		virtual EventType GetType() const override {
			return EventType::KeyReleased;
		}
		virtual std::string ToString() const override {
			std::stringstream stream;
			stream << "KeyReleased: " << ((char)_keyCode);
			return stream.str();
		}
	};
}
