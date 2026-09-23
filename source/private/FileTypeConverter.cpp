
#include "../public/FileTypeConverter.h"
#include <chrono>
#include <thread>
#include <iostream>

#define batchfile_destination "bin\\"
#define audiofile_foldername "audiofiles"

FileConverter::FileConverter()
{

}

FileConverter::~FileConverter()
{
	std::string batchfile = batchfile_destination + std::string(FileName);
	if (DoesFileExist(batchfile.c_str())) remove(batchfile.c_str());
	if(GetClear()) ClearCreatedSoundFiles();
}

void FileConverter::ClearCreatedSoundFiles()
{
	if(DoesFileExist(GetAudioFileDest())) {
		std::uintmax_t removed = fs::remove_all(GetAudioFileDest());	
	}
}

const char* FileConverter::GetAudioFileDest()
{
	str = std::string(batchfile_destination) + std::string(audiofile_foldername);
	return str.c_str();
}

std::string FileConverter::CreateBatFile(const std::string &videofolder)
{
	if (!fs::is_directory("bin")) fs::create_directories("bin");
	std::string batchfile = batchfile_destination + std::string(FileName);
	if(DoesFileExist(batchfile.c_str())) remove(batchfile.c_str());
	fs::path path = fs::current_path();
	std::cout << "path " << path.string();

		std::ofstream BatchFile(batchfile);

		if(!BatchFile){
			std::cout << "Batch file failed to create\n";
			return "\0";
		}

		BatchFile << 
			"@echo off\n"
			"setlocal enabledelayedexpansion\n"
			"\n"
			"set \"folderPath=" + path.string() + "\"\n"
			"\n"
			"if not exist \"%folderPath%\\ffmpeg.exe\" (\n"
			"    echo ffmpeg.exe not found\n"
			"    pause\n"
			"    exit /b\n"
			")\n"
			"\n"
			"if not exist \"%~dp0" + std::string(audiofile_foldername) + "\\\" (\n"
			"    mkdir \"%~dp0" + std::string(audiofile_foldername) + "\\\"\n"
			")\n"
			"\n"
			"for %%A in ("
			"\"" + videofolder + "\\*.mp4\" "
			"\"" + videofolder + "\\*.avi\" "
			"\"" + videofolder + "\\*.mkv\" "
			"\"" + videofolder + "\\*.mov\""
			") do (\n"
			"    \"%folderPath%\\ffmpeg.exe\" -n -i \"%%A\" -vn "
			"-acodec pcm_s16le -ar 44100 -ac 2 "
			"\"%~dp0" + std::string(audiofile_foldername) + "\\%%~nA.wav\"\n"
			")\n";

	return batchfile;
}

bool FileConverter::DoesFileExist(const char* file){
	return fs::exists(file);
}

void FileConverter::OpenBatFile(const char *fileToOpen)
{
	constexpr int maxAttempts = 100;

	for (int i{}; i < maxAttempts; i++) {
		if (DoesFileExist(fileToOpen)) {
			std::system(fileToOpen);
			return;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << "\nERROR timed out waiting for batch file creation: " << fileToOpen << "\n";
}

void FileConverter::CloseBatFile(const char *fileToClose)
{
	remove(fileToClose);
}

const fs::path FileConverter::GetAudioFilePath(const std::string &name)
{
	const char* Audiofile_dest = GetAudioFileDest();
	std::string Audiofile_destStr = std::string(Audiofile_dest);
	fs::path p = name;
	p.replace_extension();
	return fs::path(Audiofile_destStr) / p.string();
}
