
#include "../public/VideoManager.h"
#include "../public/FileTypeConverter.h"
#include <filesystem>
#include <type_traits>
#include "../public/magic_enum/magic_enum.hpp"
#include <algorithm>

constexpr float SKIP_AMOUNT = 10.f;

VideoManager::VideoManager()	
{

}

HANDLE VideoManager::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

long VideoManager::StartVideo(const std::string &dir, const std::string &name, const bool bPlay_next)
{
	long Key = -1;
	SetPaused(false);
	
	audioplayer.StopAudio();

	if(cap.isOpened()) {
		cap.release();
	}

	VideoPath = dir + '\\' + name;
	VideoName = name;

	if(!OpenVideo()) return -1;
	PlayVideo(Key, bPlay_next);
	
	return Key;
}

bool VideoManager::OpenVideo()
{
	const std::filesystem::path p = FileConverter::GetAudioFilePath(VideoName);
	const std::string audioPath = AudioPlayer::GetAudioFileName(p.string());
	
	if(!audioplayer.LoadAudio(audioPath)){
		return false;	
	}
	
	cap.open(VideoPath);

	if(!cap.isOpened()){
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Cannot open video file\n";
		std::cin.get();
		return false;	
	}
	
	SetConsoleTextAttribute(hConsole, 15 + 3 * 16);
	std::cout << "\nNow playing: " << VideoName << '\n';
	Create_CV_Window();
	SetWindowProperties();
	
	isWindowOpen = true;
	
	return true;
}

void VideoManager::PlayVideo(long &Key, const bool bPlay_next)
{
	double fps = cap.get(cv::CAP_PROP_FPS);
	if(fps <= 0) fps = 30;
	auto framePeriod = std::chrono::duration<double>(1.f / fps);
	auto nextTime = std::chrono::steady_clock::now() + framePeriod; 
	cv::Mat frame;
	cv::Mat background(DisplayHeight, DisplayWidth, CV_8UC3, cv::Scalar(0, 0, 0));
	
	audioplayer.PlayAudio();

	while(true){

		bool bSuccess = cap.read(frame);

		if(!bSuccess){
			if(!bPlay_next){
				Key = cv::waitKeyEx(0);
				if(ControlKeyPressed(Key)){
					if(!GetShuffle() || Key != SHUFFLE_KEY){
						EndVideo();
						return;
					}
				}
				continue;
			}
			SetConsoleTextAttribute(hConsole, 18 + 3 * 16);
			std::cout << "Ended " << VideoName << ". playing next video\n";
			break;
		}

		HandleImShow(frame, background);

		auto now = std::chrono::steady_clock::now();
		int waitTime = std::max(1, (int)std::chrono::duration_cast<std::chrono::milliseconds>(nextTime - now).count());   
		Key = cv::waitKeyEx(paused? 0 : waitTime);

		if (Key == RESTART_KEY){
			SetPositionInVideo(false, true);
		}

		if(ControlKeyPressed(Key)){
			EndVideo();
			return;
		}
		if(Key == PAUSE_KEY){
			SetPaused(!paused);
			audioplayer.PauseAudio(paused);
			if(!paused) nextTime = std::chrono::steady_clock::now() + framePeriod; 
		}
		KeyPressed(Key);
		
		nextTime += framePeriod;
	}
	EndVideo();

	return;
}

void VideoManager::EndVideo()
{
	audioplayer.StopAudio();
	
	if(isWindowOpen){
		DestroyWindow();
		isWindowOpen = false;
	}
}

void VideoManager::KeyPressed(const int Key)
{
	if(Key == VOLUME_UP_KEY){
		audioplayer.PassVol(10.f);
		return;
	}
	if(Key == VOLUME_DOWN_KEY){
		audioplayer.PassVol(-10.f);
		return;
	}
	if(Key == SKIP_FORWARD_KEY){
		SetPositionInVideo(true, false);
		return;	
	}
	if(Key == SKIP_BACKWARD_KEY){
		SetPositionInVideo(false,  false);
		return;
	}
	if (Key == MUTE_KEY) {
		audioplayer.MuteVideo();
		return;
	}
}

void VideoManager::CreateBlackBorderForVideo(cv::Mat &frame, cv::Mat &background)
{
	double scale = std::min(
    		(double)DisplayWidth / frame.cols,
   	 	(double)DisplayHeight / frame.rows
	);

	newW = cvRound(frame.cols * scale);
	newH = cvRound(frame.rows * scale);

	cv::Mat resized;
	cv::resize(frame, resized, cv::Size(newW, newH));

	int x = (DisplayWidth - newW) / 2;
	int y = (DisplayHeight - newH) / 2;

	cv::Rect roi(x, y, newW, newH);

	resized.copyTo(background(roi));
}

void VideoManager::HandleImShow(cv::Mat &frame, cv::Mat &background)
{
	CreateBlackBorderForVideo(frame, background);
	GaussianBlur(background, image_blurred_with_5x5_kernel, cv::Size(5, 5), 0);
	cv::imshow(VideoName, image_blurred_with_5x5_kernel);	
}

void VideoManager::SetPositionInVideo(bool forwards, const bool restart)
{
	double current_time = cap.get(cv::CAP_PROP_POS_MSEC);
	const double time_amount = 10000.f;

	if (restart) {
		cap.set(cv::CAP_PROP_POS_MSEC, double(0.f));
		audioplayer.SetMusicTime(true, time_amount, true);
		return;
	}
	
	if(forwards){
		cap.set(cv::CAP_PROP_POS_MSEC, current_time + time_amount);
		audioplayer.SetMusicTime(true, time_amount, false);
		return;
	}
	cap.set(cv::CAP_PROP_POS_MSEC, std::max(double(0.f), current_time - time_amount));
	audioplayer.SetMusicTime(false, time_amount, false);
}
