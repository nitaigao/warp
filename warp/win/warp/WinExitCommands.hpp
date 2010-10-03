#ifndef WIN_EXIT_COMMANDS_HPP
#define WIN_EXIT_COMMANDS_HPP

	#include <iostream>
	#include <Windows.h>

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
			buffer.mi.mouseData = 0;
			buffer.mi.dwFlags = MOUSEEVENTF_MOVE;
			buffer.mi.time = 0;
			buffer.mi.dwExtraInfo = 0;

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