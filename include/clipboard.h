namespace clipboard {
	void copy_txt(const std::string& txt);
}

#if _WIN32
#include "clipboard_windows.h"
#elif __linux__
#include "clipboard_linux.h"
#elif __APPLE__
#include "clipboard_mac.h"
#else
void clipboard::copy_txt(const std::string& txt) {
	printf("\nWARNING: Missing implementation for the clipboard::copy_txt function on this platform. Couldn't copy the jwt to the clipboard... please copy it manually!");
}
#endif
