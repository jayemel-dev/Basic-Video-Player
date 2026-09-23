#pragma once
#include <filesystem>
#include <vector>

struct Video {
	std::string name{};
	std::string extention{};
};

class FileLoader
{
protected:
	void SetVideoCount();

	uint16_t VideoCount;	
	std::string Dir;
	std::vector<Video> VideoList;

public:
	FileLoader();
	FileLoader(const std::string &dir) : Dir{dir} {
		GetVideoFilesFromDirectory();
		SetVideoCount();
	}
	inline const std::vector<Video> GetVideoList(){
		return VideoList;
	}
	void GetVideoFilesFromDirectory();
	void SetDir(const std::string &dir);
	inline const std::string MakeVideoString(const Video &video){ return video.name + video.extention; }
	const uint16_t GetListSize();
	const void PrintList();
	
private:
	bool HasVideoExtention(const std::string &extention);
};
