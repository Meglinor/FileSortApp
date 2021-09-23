#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <future>

constexpr size_t KBYTE = 1024;
<<<<<<< HEAD
constexpr size_t MAX_MEMORY = KBYTE * KBYTE * 256;
=======
constexpr size_t MAX_MEMORY = KBYTE * KBYTE * 4;
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
using FILETYPE = unsigned int;
using VectorUInt = std::vector<FILETYPE>;

using std::cout;

class FileSort
{
public:
	FileSort();

	void startSort(const std::string& inputPath, const std::string& inputFile, const std::string& outpuFile);

private:
	void sortInput();
<<<<<<< HEAD
	bool sortData(size_t fileIndex, VectorUInt&& data);
	void startMerge(const std::string& outputFile);
=======
	bool sortAndSave(size_t fileIndex, VectorUInt&& data);
	void sortAndMerge(const std::string& outputFile);
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
	bool merge(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile);

	size_t vectorSize_;
	size_t threadsMaxCount_ = 4;
	size_t tempFileCount_ = 0;
	std::string inputPath_;
	std::string inputFile_;
	std::string outputFile_;
	std::list<std::string> filesList_;
};

