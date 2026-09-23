#pragma once
#include <cstdlib>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileConverter
{
public:
	FileConverter();
	~FileConverter();
	static void CloseBatFile(const char *fileToClose);
	static bool DoesFileExist(const char* file);
	std::string CreateBatFile(const std::string &videofolder);
	void OpenBatFile(const char* fileToOpen);
	static const fs::path GetAudioFilePath(const std::string &name);
	static void ClearCreatedSoundFiles();
	static inline bool GetClear(){return clear;}
	static inline void SetClear(bool a){clear = a;}

private:
	static const char* GetAudioFileDest();
	inline static bool clear = true;
	inline static std::string str{};
	inline static const char* FileName = "converter.bat";
};
