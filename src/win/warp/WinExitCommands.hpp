#ifndef WIN_EXIT_COMMANDS_HPP
#define WIN_EXIT_COMMANDS_HPP

	#include <iostream>
	#include <Windows.h>

	char* tohex(int value)
	{
		char str[256];
		sprintf(str, "%x", value);
		std::clog << str << std::endl;
		return str;
	}

	class KeyDownCommand : public IExitCommand
	{
	public:

		static int type() { return KEY_DOWN; };

		void Execute(const Message& message)
		{
			tohex(message.key_code);
			INPUT buffer;
			buffer.type = INPUT_KEYBOARD;
			buffer.ki.wVk = VkKeyScan('a');

			//VK_CANCEL

			buffer.ki.dwExtraInfo = 0;
			buffer.ki.dwFlags = 0;
			buffer.ki.time = 0;
			buffer.ki.wScan = 0;

			SendInput(1, &buffer, sizeof(INPUT));
		};

	};

	class KeyUpCommand : public IExitCommand
	{
	public:

		static int type() { return KEY_UP; };

		void Execute(const Message& message)
		{
			/*INPUT buffer;
			buffer.type = INPUT_KEYBOARD;
			buffer.ki.wVk = VkKeyScan(message.key_code);
			buffer.ki.dwFlags = KEYEVENTF_KEYUP;

			
			buffer.ki.dwExtraInfo = 0;
			buffer.ki.dwFlags = 0;
			buffer.ki.time = 0;
			buffer.ki.wScan = 0;

			SendInput(1, &buffer, sizeof(INPUT));*/
		};

	};

	class MouseMovedCommand : public IExitCommand
	{

	public:

		static int type() { return MOUSE_MOVE; };

		void Execute(const Message& message)
		{
			INPUT buffer;

			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = message.x;
			buffer.mi.dy = message.y;
			buffer.mi.dwFlags = MOUSEEVENTF_MOVE;

			SendInput(1, &buffer, sizeof(INPUT));
		};

	};

	class LeftDownCommand : public IExitCommand
	{

	public:

		static int type() { return LEFT_DOWN; };

		void Execute(const Message& message)
		{
			INPUT buffer;

			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = message.x;
			buffer.mi.dy = message.y;
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;

			SendInput(1, &buffer, sizeof(INPUT));
		};
	};

	class LeftUpCommand : public IExitCommand
	{

	public:

		static int type() { return LEFT_UP; };

		void Execute(const Message& message)
		{
			INPUT buffer;

			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = message.x;
			buffer.mi.dy = message.y;
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;

			SendInput(1, &buffer, sizeof(INPUT));
		};
	};

	class RightDownCommand : public IExitCommand
	{

	public:

		static int type() { return RIGHT_DOWN; };

		void Execute(const Message& message)
		{
			INPUT buffer;

			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = message.x;
			buffer.mi.dy = message.y;
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;

			SendInput(1, &buffer, sizeof(INPUT));
		};
	};

	class RightUpCommand : public IExitCommand
	{

	public:

		static int type() { return RIGHT_UP; };

		void Execute(const Message& message)
		{
			INPUT buffer;

			buffer.type = INPUT_MOUSE;
			buffer.mi.dx = message.x;
			buffer.mi.dy = message.y;
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;

			SendInput(1, &buffer, sizeof(INPUT));
		};
	};

	class LeftDoubleClickCommand : public IExitCommand
	{

	public:

		static int type() { return LEFT_DOUBLE_CLICK; };

		void Execute(const Message& message)
		{
			LeftDownCommand().Execute(message);
			LeftUpCommand().Execute(message);
		};
	};

#endif