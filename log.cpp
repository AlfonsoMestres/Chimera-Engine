#pragma once
#include "Globals.h"
#include "imgui.h"

ImGuiTextBuffer		Buf;
ImVector<int>       LineOffsets;        // Index to lines offset
bool                ScrollToBottom;

void Clear() { Buf.clear(); }

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	int old_size = Buf.size();

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	Buf.appendf(format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size);
	ScrollToBottom = true;
}
//
//void PrintConsole(const char* title, bool* p_opened = NULL)
//{
//	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
//	ImGui::Begin(title, p_opened);
//	if (ImGui::Button("Clear")) Clear();
//	ImGui::SameLine();
//	bool copy = ImGui::Button("Copy");
//	ImGui::Separator();
//	ImGui::BeginChild("scrolling");
//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
//	if (copy) ImGui::LogToClipboard();
//	ImGui::TextUnformatted(Buf.begin());
//	if (ScrollToBottom)
//		ImGui::SetScrollHere(1.0f);
//	ScrollToBottom = false;
//	ImGui::PopStyleVar();
//	ImGui::EndChild();
//	ImGui::End();
//}