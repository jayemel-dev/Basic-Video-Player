#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>
#include <algorithm>
#include <Windows.h>

class AudioPlayer
{
public:
	AudioPlayer();
	bool LoadAudio(const std::string &audio);
	void PlayAudio();
	void StopAudio();
	void PauseAudio(const bool paused);
	inline static const std::string GetAudioFileName(const std::string &name){return name + ".wav";}
	void inline SetVol(const float &vol){Volume = std::clamp(vol, 0.0f, 200.0f);}
	inline const float GetVol(){return Volume;}
	void MuteVideo(){if(music.getVolume() == 0.0){SetVol(PrevVolume);} else{SetVol(0.f);} music.setVolume(GetVol());}
	inline void PassVol(const float &in){ChangeVol(in);}
	void SetMusicTime(bool forward, const float time_amount, const bool restart);
private:
	void ChangeVol(const float &newval);

	sf::Music music;
	float Volume;
	float PrevVolume;

	static HANDLE hConsole;
};
