
#include "../public/Player.h"
#include <random>
#include <functional> 
#include <limits>

Player::Player()
{
	GetVideoFilesFromDirectory();
}

HANDLE Player::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Player::~Player()
{
	ClearList();
}

void Player::StartPlaying()
{
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\nStarting player\n";
	bool play_next = videomanager.GetAutoplay();
	bool ExitPlayer = false;
	while(true){
		Starting = false;
		if(!video){
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "\nQueued video is null!\n";
			break;
		}

		SetConsoleTextAttribute(hConsole, 10);
		if(video->next) std::cout << "\nNext video in queue: " << video->next->video.name << '\n';
		const std::string name = MakeVideoString(video->video);
		const long Action = videomanager.StartVideo(playerDir, name, play_next);
		bool Handled = VideoController::HandleAction(Action, &video, std::bind(&Player::MakeVideoQueue, this), ExitPlayer);
	
		if(ExitPlayer) return;	

		if(!Handled) VideoController::PlayNext(&video);
	}

	Starting = true;
}


bool Player::MakeVideoQueue() 
{	
	SetConsoleTextAttribute(hConsole, 13);
	if(videomanager.GetShuffle()) std::cout << "\nShuffling queue...\n";

	if(!Starting){
		videomanager.EndVideo();
		ClearList();
	}
	if(GetListSize() <= 0){
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "\nNo videos in folder. Is this the right directory? " << playerDir << "\n";
		return false;
	}
	
	std::random_device rd;
	auto rng = std::default_random_engine {rd()};
	std::vector<Video> ShuffledList = GetVideoList();
	if(videomanager.GetShuffle()) std::shuffle(std::begin(ShuffledList), std::end(ShuffledList), rng);
	auto VideoList = videomanager.GetShuffle()? ShuffledList : GetVideoList();
	CreateLinkList(VideoList);

	video = head;
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "Songs in queue " << GetListSize() << '\n';
	return true;
}

void Player::CreateLinkList(const std::vector<Video>& videoList)
{
	bool first = true;
	SetConsoleTextAttribute(hConsole, 13);
	std::cout << "\nMaking queue...";
	for (const auto& vid : videoList) {
		if (first) {
			MakeListStart(vid);
			first = false;
			continue;
		}
		AddNode(vid);
	}
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "completed\t";
}

void Player::AddNode(const Video &video)
{
	VideoInQueue *node = new VideoInQueue();
	node->video = video;
	node->next = head;
	node->prev = tail;
	tail->next = node;
	head->prev = node;
	tail->last = false;
	tail = node;
	node->last = true;

}
	
void Player::MakeListStart(const Video &video)
{
	VideoInQueue *start = new VideoInQueue();
	start->video = video;
	head = start;
	tail = start;
	start->next = nullptr;
	start->prev = nullptr;
}

void Player::InsertNode(const Video &video)
{
	if (!head) {
		MakeListStart(video);
		this->video = head;
		return;
	}

	VideoInQueue *node = new VideoInQueue();
	node->video = video;
	node->next = head;
	node->prev = tail;
	tail->next = node;
	tail->last = false;
	node->last = true;
	head->prev = node;
	head = node;

	this->video = node;
}

void Player::ClearList()
{
	printForward();
	VideoInQueue* traverser = head;

	while (traverser)
	{
		bool end = traverser->last;

		VideoInQueue* next = traverser->next;	
		delete traverser;
		traverser = next;

		if (end) break;
	}

	this->video = nullptr;
	head = nullptr;
	tail = nullptr;
}

void Player::printForward()
{
	VideoInQueue *traverser = head;
	std::cout << '\n';
	int index = 0;
	do{
		if(!traverser) return;
		index++;
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << index << " " << traverser->video.name << '\n';
		traverser = traverser->next;
	}
	while(traverser != head);
}

void Player::SelectVideo()
{
	int VideoIndex;
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\nEnter video index: ";
	std::cin >> VideoIndex;
	while(VideoIndex < 1 || VideoIndex > VideoCount){
		std::cin >> VideoIndex;	
	}

	const Video video = GetVideoList()[VideoIndex - 1];
	SetConsoleTextAttribute(hConsole, 14);
	std::cout <<"\nVIDEO SELECTED: " << video.name << '\n'; 
	InsertNode(video);
}

bool Player::AskMakeQueue()
{
	SetConsoleTextAttribute(hConsole, 3);
	std::cout << "\n\tWould you like to make a queue? (yes - choose songs : no - pick one song to play and play rest of list): ";
	while(1){

		const std::array<std::string_view, 4> ValidInputs = {"yes", "y", "no", "n"};
	
		std::string in;
		std::cin >> in;

		auto it = std::find(ValidInputs.begin(), ValidInputs.end(), in);

		if(it == ValidInputs.end()) {
			return false;
		}
	
		if(in == ValidInputs[2] || in == ValidInputs[3]){
			return false;
		}

		if(in == ValidInputs[0] || in == ValidInputs[1]) {
			return true;
		}
	}
}

void Player::MakeQueue()
{
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "\nType \"end\" to complete\n";

	std::string response;
	while(response != "end"){
		SelectVideo();
		std::cout << "\npress enter to continue or type \"end\" to finish\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, response);
	}
}


