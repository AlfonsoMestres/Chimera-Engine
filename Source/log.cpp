#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

void log(const char file[], int line, const char* format, ...) {
	assert(format != nullptr);

	static char tmpStr[4096];
	static char tmpStr2[4096];
	static va_list ap = nullptr;

	va_start(ap, format);
	vsprintf_s(tmpStr, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpStr2, 4096, "\n%s(%d) : %s", file, line, tmpStr);
	OutputDebugString(tmpStr2);
	if (App != nullptr) {
		sprintf_s(tmpStr, 4096, "%s \n", tmpStr);
		if (App->editor->console != nullptr && App->editor->console->IsEnabled()) {
			App->editor->console->AddLog(tmpStr);
		}
	}

}

