#include "filemanager.h"

std::string readFile(std::string path)
{
	std::string line;
	std::string text;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			text += line + '\n';
		}
		file.close();
	}
	return text;
}