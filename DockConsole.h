#ifndef __DOCKCONSOLE_H__
#define __DOCKCONSOLE_H__

#include "Dock.h"

class DockConsole : public Dock
{
	public:
		DockConsole();
		~DockConsole();

		void Clear() { buf.clear(); lineOffsets.clear(); }
		void AddLog(const char* log);
		void Draw();

	private:
		ImGuiTextBuffer     buf;
		ImVector<int>       lineOffsets;
		bool                scrollToBottom = false;
};

#endif __DOCKCONSOLE_H__
