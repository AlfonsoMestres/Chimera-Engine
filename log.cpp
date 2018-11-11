#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

void log(const char file[], int line, const char* format, ...) {

	static char tmpStr[4096];
	static char tmpStr2[4096];
	static va_list  ap;

	va_start(ap, format);
	vsprintf_s(tmpStr, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpStr2, 4096, "\n%s(%d) : %s", file, line, tmpStr);
	OutputDebugString(tmpStr2);
	if (App != NULL) {
		App->editor->console->AddLog(tmpStr2);
	}

}
