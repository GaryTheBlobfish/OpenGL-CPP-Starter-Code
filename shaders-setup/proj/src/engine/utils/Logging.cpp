#include "Logging.h"

#include <sstream>

void FileLogging::writeToTextFile(std::string s)
{
	/*
		At somepoint, consider using a different time getting system
		because this one requires supressing a warning which cannot
		be good. -_-
	*/


	std::ofstream logFile("logs/latest.log", std::ios_base::app);

	logFile << getTime() << " :                " << s;
	logFile.flush();
	//logFile.close();
}

void FileLogging::writeToTextFileNTime(std::string s)
{
	std::ofstream logFile("logs/latest.log", std::ios_base::app);

	logFile << s;
	logFile.flush();
	//logFile.close();
}

void FileLogging::writePlayTime(std::string start, std::string end)
{
	// FORMAT START

	for (int i = 0; i < 11; i++)
		start.erase(i);

	for (int chars = 0; chars < start.size(); chars++) {
		if (start[chars] == ':')
		{
			start[chars] == ' ';
		}
	}

	std::string hoursS = start.substr(0, start.find(' '));

	start.erase(0);
	start.erase(1);

	std::string minutesS = start.substr(0, start.find(' '));

	start.erase(0);
	start.erase(1);

	std::string secondsS = start.substr(0, start.find(' '));

	start.erase(0);
	start.erase(1);

	int hours, minutes, seconds;

	std::stringstream ss;
	ss << hoursS;
	ss >> hours;
	ss << minutesS;
	ss >> minutes;
	ss << secondsS;
	ss >> seconds;

	// FORMAT END

	for (int i = 0; i < 11; i++)
		end.erase(i);

	for (int chars = 0; chars < end.size(); chars++) {
		if (end[chars] == ':')
		{
			end[chars] == ' ';
		}
	}

	std::string ehoursS = end.substr(0, end.find(' '));

	start.erase(0);
	start.erase(1);

	std::string eminutesS = end.substr(0, end.find(' '));

	start.erase(0);
	start.erase(1);

	std::string esecondsS = end.substr(0, end.find(' '));

	start.erase(0);
	start.erase(1);

	int ehours, eminutes, eseconds;

	ss << ehoursS;
	ss >> ehours;
	ss << eminutesS;
	ss >> eminutes;
	ss << esecondsS;
	ss >> eseconds;

	int playHrs = ehours - hours;
	int playMins = eminutes - minutes;
	int playSecs = eseconds - seconds;

	std::string playtime = "TOTAL PLAY TIME (will not be correct for play time more than 24 hours): ";
	playtime += std::to_string(playHrs);
	playtime += "hrs ";
	playtime += std::to_string(playMins);
	playtime += "mins ";
	playtime += std::to_string(playSecs);
	playtime += "secs\n";

	writeToTextFileNTime(playtime);

}

std::string FileLogging::getTime()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
#pragma warning(suppress : 4996)
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string timeStr(buffer);

	return timeStr;
}

void FileLogging::clearLogFile()
{

	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
#pragma warning(suppress : 4996)
	timeinfo = localtime(&rawtime);


	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string timeStr(buffer);

	std::ofstream clearFile("logs/latest.log");
	clearFile << "LOGGING STARTED AT: " << timeStr << "\n\n";
}