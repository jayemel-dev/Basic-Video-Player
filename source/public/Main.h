#pragma once
#include "../../includes/mainincludes.h"
#include "../public/VideoManager.h"

enum class PlayChoice{PICK, START};

class VideoDirectory
{
public:
	VideoDirectory();
	~VideoDirectory();
	inline static const bool VideoChoose(){return Choice == PlayChoice::PICK;}
	void ImportSettingsFromJSON();
	static bool GetVideoChoiceFromUser(Player &player);
	inline const std::string GetVideoDir(){return dir;}
	inline static bool GetValidDir(){return ValidDir;}
	inline static void SetDir(VideoDirectory &obj){std::getline(std::cin, obj.dir);}
	static void AssertDirValid(const std::string &dir);
	static void GetUserInputForDir(VideoDirectory &obj);
	inline FileConverter *GetFileConverter(){return fileconverter;}
	inline VideoManager *GetVideoManager(){return videomanager;}
	void ImportPathFromJSON(const nlohmann::json &jsonFile);
	inline static std::string MakeDirectory(const std::string &path, const std::string &name){return path + '\\' + name; }
	void static CheckDirValid(VideoDirectory &obj);
	static const inline bool WaitForValidInput(const std::string& in) {
		return in != "y" && in != "n";
	}

	static HANDLE hConsole;

private:
	inline static PlayChoice Choice = PlayChoice::START;
	inline static bool ValidDir = true;
	std::string dir;
	FileConverter *fileconverter = nullptr;
	VideoManager *videomanager = nullptr;
};

