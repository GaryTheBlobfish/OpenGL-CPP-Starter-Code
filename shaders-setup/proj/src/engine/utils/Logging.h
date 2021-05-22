#pragma once
#include <fstream>

class FileLogging {
public:
	static void writeToTextFile(std::string s);
	static void writeToTextFileNTime(std::string s);
	static void writePlayTime(std::string start, std::string end);
	static std::string getTime();
	static void clearLogFile();
};