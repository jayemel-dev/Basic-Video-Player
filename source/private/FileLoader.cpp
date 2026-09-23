
#include "../public/FileLoader.h"
#include <iostream>

#define MP4 ".mp4"
#define AVI ".avi"
#define MKV ".mkv"
#define MOV ".mov"

FileLoader::FileLoader()
{

}

const void FileLoader::PrintList()
{
	std::cout << '\n';
	for(int i{}; i < VideoCount; i++){
		std::cout << i + 1 << ' ' << GetVideoList()[i].name << '\n';
	}
}

void FileLoader::GetVideoFilesFromDirectory()
{
	for(const auto &dir : std::filesystem::directory_iterator(Dir)){
		if(!HasVideoExtention(dir.path().extension().filename().string())) continue;
		Video elem;
		elem.name = dir.path().stem().string();
		elem.extention = dir.path().extension().filename().string();
		VideoList.push_back(elem);	
	}
	VideoCount = VideoList.size();
}

bool FileLoader::HasVideoExtention(const std::string &extention)
{
	return extention == MP4 || extention == AVI || extention == MKV || extention == MOV;
}

void FileLoader::SetDir(const std::string &dir)
{
	Dir = dir;
}

const uint16_t FileLoader::GetListSize()
{
	return VideoCount;
}

void FileLoader::SetVideoCount()
{
	VideoCount = VideoList.size();
}


