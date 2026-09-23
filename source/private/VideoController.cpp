
#include "../public/VideoController.h"

VideoController::VideoController()
{

}

HANDLE VideoController::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool VideoController::HandleAction(const long &key, VideoInQueue **video, std::function<void()> MakeQueueFunc, bool &exit)
{
	if(key <= 0) return false;

	switch(key)
	{
		case NEXT_KEY:
			PlayNext(video);
			return true;

		case PREV_KEY:
			PlayPrev(video);
			return true;

		case SHUFFLE_KEY:
			MakeQueueFunc();
			return true;	
		case CLOSE_KEY:
			exit = true;
			return true;
		default:
			return false;
	}
}

void VideoController::PlayNext(VideoInQueue **video)
{
	if(!video || !(*video)->next) {
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\nNext video is null!\n";
		return;
	}
	(*video) = (*video)->next;
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "Playing next video: " << (*video)->video.name;
}

void VideoController::PlayPrev(VideoInQueue **video)
{
	if(!video || !(*video)->prev) {
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\nPrevious video is null!\n";
		return;
	}
	(*video) = (*video)->prev;
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "Playing next video: " << (*video)->video.name;
}
