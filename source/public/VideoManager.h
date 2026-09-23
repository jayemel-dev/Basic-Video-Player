#pragma once
#include <opencv2/opencv.hpp>
#include "../public/VideoController.h"
#include "../public/AudioManager.h"
#include <Windows.h>

enum class Autoplaystate{True, False};
enum class Shufflestate{True, False};
enum class Fullscreenstate{True, False};

constexpr int DisplayWidth = 1920;
constexpr int DisplayHeight = 1080;

class VideoManager
{
private:
	std::string capDir{};
	std::string VideoPath{};

public:
	VideoManager();
	VideoManager(const std::string dir, const std::string name) : capDir{dir} , VideoPath{name} {}
	long StartVideo(const std::string &dir, const std::string &name, const bool bPlay_next); 
	const std::string GetDir()
	{
		return capDir;
	}
	const std::string GetName()
	{
		return VideoName;
	}


	template<typename T>
	void GetStateFromSettings(T *state, std::string in, const std::string &settingName)
	{
		constexpr std::array valid {
			T::True,
			T::False,
		};

		if constexpr(std::is_enum_v<std::remove_cv_t<T>>){
		std::string_view  t = "true";
		std::string_view  f = "false";

		auto tolower = [](char a, char b){
			return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
		};
	
		if(in.size() == t.size() && std::equal(in.begin(), in.end(), t.begin(),  t.end(), tolower)){
			if (std::find(valid.begin(), valid.end(), *state) == valid.end())
   				return;
			*state = T::True;
			std::cout << settingName << "is on\n";
			return;
		}else if(in.size() == f.size() && std::equal(in.begin(), in.end(), f.begin(), f.end(), tolower)) {
			if (std::find(valid.begin(), valid.end(), *state) == valid.end())
   				return;
			*state = T::False;
			std::cout << settingName << "is off\n";
			return;
		}
		std::cout << "Please type true or false\n";
		}
	}

	template<typename T>
	bool GetSettingState(T *state)
	{
		if constexpr(std::is_enum_v<std::remove_cv_t<T>>){

		switch(*state){
		case T::True:
			return true;
		case T::False:
			return false;
			}
		}
		return false;
	}

	inline const bool GetAutoplay(){return GetSettingState(GetAutoplayState());}
	inline const bool GetShuffle(){return GetSettingState(GetShuffleState());}
	inline const bool GetFullscreen(){return GetSettingState(GetFullscreenState());}

	inline const cv::VideoCapture& GetCap(){return cap;}
	inline void SetPaused(const bool a){paused = a;}

	inline static Autoplaystate* GetAutoplayState(){return &AutoPlay;}
	inline static Shufflestate* GetShuffleState(){return &Shuffle;}
	inline static Fullscreenstate* GetFullscreenState(){return &Fullscreen;};

	void EndVideo();
		
private:
	bool OpenVideo();
	void KeyPressed(const int Key);
	inline const bool ControlKeyPressed(const long &key){ return key == CLOSE_KEY || key == NEXT_KEY || key == PREV_KEY || key == SHUFFLE_KEY;}
	void PlayVideo(long &Key, const bool bPlay_next);
	inline void Create_CV_Window() {cv::namedWindow(VideoName, cv::WINDOW_NORMAL);}
	inline void SetWindowProperties(){setWindowProperty(VideoName, cv::WND_PROP_FULLSCREEN, GetScreenSizeType());}
	inline const cv::WindowFlags GetScreenSizeType(){return *GetFullscreenState() == Fullscreenstate::True? cv::WINDOW_FULLSCREEN : cv::WINDOW_AUTOSIZE;}
	inline void DestroyWindow() {cv::destroyWindow(VideoName);}
	void CreateBlackBorderForVideo(cv::Mat &frame, cv::Mat &background);
	void HandleImShow(cv::Mat &frame, cv::Mat &background);
	void SetPositionInVideo(bool forwards, const bool restart);
	bool isWindowOpen = false;
	cv::Mat image_blurred_with_5x5_kernel;

	std::string VideoName{};
	AudioPlayer audioplayer;
	cv::VideoCapture cap;

	inline static Autoplaystate AutoPlay = Autoplaystate::True;
	inline static Shufflestate Shuffle = Shufflestate::True;
	inline static Fullscreenstate Fullscreen  = Fullscreenstate::True;
	bool paused = false;
	int newW;
	int newH;
	static HANDLE hConsole;
};
