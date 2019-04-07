#include <Windows.h>

namespace clipboard {
	void copy_txt(const std::string& txt) 
	{
		if (OpenClipboard(NULL)) 
		{
			HGLOBAL clipbuffer;
			char* buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, txt.size() + 1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(txt.c_str()));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT, clipbuffer);
			CloseClipboard();
		}
	}
}