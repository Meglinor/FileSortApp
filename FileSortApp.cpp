#include <iostream>
#include <chrono>
#include "FileSort.h"

class SimpleTimer {
public:
	SimpleTimer() {
		start = std::chrono::high_resolution_clock::now();
	}

	~SimpleTimer() {
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		cout << "Estimated time: " << duration.count() << "\n";
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
};


std::string SplitFilename(const std::string& str) 
{
	size_t found;
	found = str.find_last_of("/\\");
	return str.substr(0, found + 1);
}

int main(int argc, char* argv[])
{
// namespace for SimpleTimer	
	{
		SimpleTimer simpleTimer;
		FileSort fileSort;

		std::string input = "input";
		std::string output = "output";
		if (argc >= 3)
		{
			input = std::string(argv[1]);
			output = std::string(argv[2]);
		}

		fileSort.startSort(SplitFilename(std::string(argv[0])), input, output);

		std::cout << "Operation complete!\n";
	}
	std::cout << "Press any key to continue . . .\n";
	std::cin.get();
}
