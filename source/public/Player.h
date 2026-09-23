#pragma once
#include "../public/FileLoader.h"
#include "../public/VideoManager.h"
#include <iostream>
#include <windows.h> 

class Player : public FileLoader 
{
private:
	std::string playerDir;
public:
	Player();
	~Player();
	Player(const std::string &dir) : playerDir{dir} {
		Dir = playerDir;
		GetVideoFilesFromDirectory();
		SetVideoCount();
	}
	void printForward();
	bool MakeVideoQueue();
	void ClearList();
	void StartPlaying();
	void SelectVideo();
	static bool AskMakeQueue();
	void MakeQueue();
	inline void SetPlayerDir(const std::string &dir){Dir = playerDir  = dir;}

private:
	void AddNode(const Video &video);
	void MakeListStart(const Video &video);
	void InsertNode(const Video &video);
	void CreateLinkList(const std::vector<Video> &videoList);

	bool Starting = true;
	VideoManager videomanager;
	VideoInQueue *video = nullptr;
	VideoInQueue *head = nullptr;
	VideoInQueue *tail = nullptr;
	static HANDLE hConsole;
};
