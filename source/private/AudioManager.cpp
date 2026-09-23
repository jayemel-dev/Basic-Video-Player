
#include "../public/AudioManager.h"

#include <iostream>

AudioPlayer::AudioPlayer()
{
	PrevVolume = 100.f;
}


HANDLE AudioPlayer::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool AudioPlayer::LoadAudio(const std::string &audio)
{
	if(!music.openFromFile(audio)){
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\nAudio failed to load to buffer " << audio << '\n';
		return false;
	}
	return true;
}

void AudioPlayer::PlayAudio()
{
	music.play();
}

void AudioPlayer::StopAudio()
{
	music.stop();
}

void AudioPlayer::PauseAudio(const bool paused)
{
	if(paused) {
		music.pause();
		return;
	}
	music.play();
}

void AudioPlayer::ChangeVol(const float &newval){
	SetVol(music.getVolume() + newval); 
	PrevVolume = GetVol();
	music.setVolume(GetVol());
}

void AudioPlayer::SetMusicTime(bool forward, const float time_amount, const bool restart){
	if (restart) {
		music.setPlayingOffset(sf::Time::Zero);
		return;
	}
	if(forward){
		music.setPlayingOffset(music.getPlayingOffset() + sf::milliseconds(time_amount));
		return;
	}
	if(music.getPlayingOffset() - sf::milliseconds(time_amount) > sf::Time::Zero){
	       	music.setPlayingOffset(music.getPlayingOffset() - sf::milliseconds(time_amount));
		return;
	}
	music.setPlayingOffset(sf::Time::Zero);	
}
