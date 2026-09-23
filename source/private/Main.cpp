
#include "../public/Main.h"
#include <vector>

VideoDirectory::VideoDirectory()
{
	fileconverter = new FileConverter();
	videomanager = new VideoManager();
};

HANDLE VideoDirectory::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

VideoDirectory::~VideoDirectory()
{
	delete fileconverter;
	delete videomanager;
}

void VideoDirectory::ImportSettingsFromJSON()
{
	std::ifstream file("..\\settings.json");

	if (!file) {
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\nFailed to open \'settings.json\'\tCheck if file exists\n";
		return;
	}

	nlohmann::json jsonFile;
	file >> jsonFile;
	ImportPathFromJSON(jsonFile);
	VideoDirectory::AssertDirValid(GetVideoDir());
	if(!ValidDir) std::cout << "The directory you entered \'" << dir << "\' is not valid\n";

	videomanager->GetStateFromSettings(VideoManager::GetAutoplayState(), jsonFile["PlayerSettings"][0]["AutoPlayOn"], "Autoplay ");
	videomanager->GetStateFromSettings(VideoManager::GetShuffleState(), jsonFile["PlayerSettings"][0]["ShuffleOn"], "Shuffle ");
	videomanager->GetStateFromSettings(VideoManager::GetFullscreenState(), jsonFile["PlayerSettings"][0]["FullScreen"], "Fullscreen ");
}

void VideoDirectory::ImportPathFromJSON(const nlohmann::json &jsonFile)
{
	std::string path;
	std::string name;
	fs::path ParentPath;

	if(jsonFile["PlayerSettings"][0]["FolderPath"] == "USE_ROOT") {
		ParentPath = fs::current_path();
		path = ParentPath.parent_path().string();
		name = "Videos";
	}
	else{
		path = jsonFile["PlayerSettings"][0]["FolderPath"].get<std::string>();
		name = jsonFile["PlayerSettings"][0]["FolderName"].get<std::string>();
	}

	dir = MakeDirectory(path, name);
	std::cout << "path is " << dir << '\n';
}

void VideoDirectory::AssertDirValid(const std::string &dir)
{

	ValidDir = fs::exists(dir) && fs::is_directory(dir);
}

int main()
{
	VideoDirectory obj;
	obj.ImportSettingsFromJSON();
	VideoDirectory::CheckDirValid(obj);
	
	std::string BatFile = obj.GetFileConverter()->CreateBatFile(obj.GetVideoDir());	
	obj.GetFileConverter()->OpenBatFile(BatFile.c_str());
	
	Player player(obj.GetVideoDir());
	player.PrintList();

	bool QueueMade = player.MakeVideoQueue();
	
	while(!QueueMade){
		VideoDirectory::GetUserInputForDir(obj);
		std::string BatFile = obj.GetFileConverter()->CreateBatFile(obj.GetVideoDir());	
		obj.GetFileConverter()->OpenBatFile(BatFile.c_str());
		player.SetPlayerDir(obj.GetVideoDir());
		player.GetVideoFilesFromDirectory();
		player.PrintList();
		QueueMade = player.MakeVideoQueue();
	}

	while(true){		
		SetConsoleTextAttribute(VideoDirectory::hConsole, 14);
		std::cout << "\n\tWould you like to choose a video from the list? (Yes - insert song : No - start queue : esc to exit)\n";
		
		if(VideoDirectory::GetVideoChoiceFromUser(player)) break;

		if(VideoDirectory::VideoChoose()){
			if(Player::AskMakeQueue()){
				player.MakeQueue();
			}
			else{
				player.SelectVideo();
			}
		}
		player.StartPlaying();
	}
	SetConsoleTextAttribute(VideoDirectory::hConsole, 12 + 15 * 16);
	std::cout << "\tClear video data from program? (y/n) - (this will mean those videos will need to be loaded again!): ";

	std::string Clear = "\0";
	while(VideoDirectory::WaitForValidInput(Clear)) {
		std::cin >> Clear;
	}
	FileConverter::SetClear(Clear == "y");
	return 1;
}

void VideoDirectory::CheckDirValid(VideoDirectory &obj)
{
	if(!VideoDirectory::GetValidDir()){
		SetConsoleTextAttribute(VideoDirectory::hConsole, 12);
		std::cout <<"\nThe folder name/path in \"settings.json\" is not valid. Please enter the directory: ";
		VideoDirectory::GetUserInputForDir(obj);
	}	
}

bool VideoDirectory::GetVideoChoiceFromUser(Player &player)
{
	const std::array<std::string_view, 5> ValidInputs = {"yes", "y", "no", "n", "esc"};
	
	std::string in;
	std::cin >> in;

	auto it = std::find(ValidInputs.begin(), ValidInputs.end(), in);

	if(it == ValidInputs.end()) {
		Choice =  PlayChoice::START;
		return false;
	}
	
	if(in == ValidInputs[2] || in == ValidInputs[3]){
	       	Choice = PlayChoice::START;
		return false;
	}

	if(in == ValidInputs[0] || in == ValidInputs[1]) {
		Choice = PlayChoice::PICK;
		return false;
	}

	if (in == ValidInputs[4]) return true;
	
	return true;
}

void VideoDirectory::GetUserInputForDir(VideoDirectory &obj)
{
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "Enter directory: ";
	do{
		VideoDirectory::SetDir(obj);
		VideoDirectory::AssertDirValid(obj.GetVideoDir());
	}
	while(!VideoDirectory::GetValidDir());
	std::cout << '\n';
}
