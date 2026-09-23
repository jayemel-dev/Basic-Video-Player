#pragma once
#include "../public/FileLoader.h"
#include <opencv2/opencv.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <Windows.h>

constexpr int CLOSE_KEY = 27;
constexpr int NEXT_KEY = 100;
constexpr int PREV_KEY = 97;
constexpr int SHUFFLE_KEY = 115;
constexpr int RESTART_KEY = 114;
constexpr int PAUSE_KEY = 32;
constexpr int MUTE_KEY = 109;

#define SKIP_FORWARD_KEY 39
#define SKIP_BACKWARD_KEY 37
#define VOLUME_UP_KEY 38
#define VOLUME_DOWN_KEY 40

struct VideoInQueue{
	Video video{};
	VideoInQueue* prev;
	VideoInQueue* next;
	bool last = false;
};

class VideoController
{
public:
	VideoController();
	static bool HandleAction(const long &key, VideoInQueue **video, std::function<void()> MakeQueueFunc, bool &exit);
	static void PlayNext(VideoInQueue **video);

private:
	static void PlayPrev(VideoInQueue **video);

	std::function<void()> RemakeQueue;
	static HANDLE hConsole;
};
