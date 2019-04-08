#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>

namespace clipboard {
	void copy_txt(const std::string& txt)
	{
		std::stringstream cmd;
		cmd << "echo \"" << txt << "\" | pbcopy";

		FILE* pipe = popen(cmd.str().c_str(), "r");
		if (!pipe)
		{
			printf("WARNING: Token couldn't be copied to clipboard. Please copy manually using cmd+c");
			return;
		}

		char buffer[128];
		std::string result = "";

		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
			{
				result += buffer;
			}
		}

		pclose(pipe);
	}
}