#include "FileSort.h"


FileSort::FileSort()
{
	auto i = std::thread::hardware_concurrency();
	if (i > 0) threadsMaxCount_ = i;
	vectorSize_ = MAX_MEMORY / (threadsMaxCount_ * sizeof(FILETYPE));
}

<<<<<<< HEAD
void FileSort::startSort(const std::string& inputPath, const std::string& inputFile, const std::string& outputFile) {
=======
void FileSort::startSort(const std::string& inputPath, const std::string& inputFile, const std::string& outputFile)
{
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
	inputPath_ = inputPath;
	inputFile_ = inputFile;
	outputFile_ = outputFile;
	sortInput();
}

<<<<<<< HEAD
void FileSort::sortInput() {
	try {
		cout << "Start sorting file - " << inputPath_ + inputFile_ << "\n";
		std::ifstream input;
		input.open(inputPath_ + inputFile_, std::ios::binary);
		if (!input) {
			cout << "Can`t open file - " << inputFile_ << "\n";
			return;
		}

		FILETYPE buffer;
		auto tmpCount = 0;
		size_t readCount = 0;
		bool error = false;
		std::list<std::future<bool>> tasks;
		while (!input.eof()) {
			VectorUInt data;
			data.reserve(vectorSize_);
			while (readCount < vectorSize_ && input.read(reinterpret_cast<char*>(&buffer), sizeof(FILETYPE))) {
				++readCount;
				data.emplace_back(buffer);
			}
			auto ts = tasks.size();
			if (tasks.size() < threadsMaxCount_ -1 ) {
				tasks.emplace_back(std::async(std::launch::async, [&tmpCount, mdata = std::move(data), this]() mutable {
					return sortData(tmpCount++, std::move(mdata)); }));
			}
			else {
				if (!tasks.front().get()) error = true;
				tasks.pop_front();
				tasks.emplace_back(std::async(std::launch::async, [&tmpCount, mdata = std::move(data), this]() mutable {
					return sortData(tmpCount++, std::move(mdata)); }));
			}
			readCount = 0;
		}
		input.close();
		auto tsize = tasks.size();
		cout << "Waiting.";
		for(auto & task : tasks) {
			cout << ".";
			if (!task.get()) error = true;;
		}
		cout << "Complete sorting.\n";
		if (!error)
			startMerge("output");
		else
			cout << "Something went wrong";
	}
	catch (const std::exception& ex) {
		cout << "FileSort::sortInput() - Exception - " << ex.what() << "\n";
		return;
	}
}


bool FileSort::sortData(size_t fileIndex, VectorUInt&& data) {
	const std::string& outputFile = "_" + std::to_string(fileIndex);
	try {
		std::ofstream output;
		cout << "Sorting and saving to " << outputFile << "\n";
		std::sort(data.begin(), data.end());
		filesList_.emplace_back(outputFile);
		output.open(inputPath_ + outputFile, std::ios::binary);
		for_each(data.begin(), data.end(), [&output](FILETYPE i) {output.write(reinterpret_cast<const char*>(&i), sizeof(FILETYPE)); });
		output.close();
		return true;
	}
	catch (const std::exception& ex) {
		cout << "FileSort::sortAndSave - Exception - " << ex.what() << " FileName - " << outputFile << "\n";
		return false;
	}
}

void FileSort::startMerge(const std::string& outputFile) {
	try {
		std::list<std::future<bool>> tasks;
		auto filesCount = filesList_.size();
		size_t iterations = filesList_.size() / 2;
		size_t iteration = 0;
		auto commit = [&tasks]() mutable {	while (tasks.size() > 0) {
			tasks.front().get();
			tasks.pop_front();
			cout << ".";
		} };
		while (filesList_.size() > 2) {
			auto inFileName1 = filesList_.front();
			filesList_.pop_front();
			auto inFileName2 = filesList_.front();
			filesList_.pop_front();
			auto outfileName = inFileName1 + inFileName2;
			filesList_.emplace_back(outfileName);
			cout << "Merge " << inFileName1 << " and " << inFileName2 << " into " << outfileName << "\n";
			tasks.emplace_back(std::async(std::launch::async, [inFileName1, inFileName2, outfileName, this]() mutable {
				return merge(inFileName1, inFileName2, outfileName); }));
			++iteration;
			if (iteration >= iterations) {
				cout << "\nCommit.";
				commit();
				cout << "\n";
				iterations = filesList_.size() / 2;
				iteration = 0;
			}
		}
		cout << "Finalize.";
		auto fs = filesList_.size();
		for_each(tasks.begin(), tasks.end(), [&tasks](auto& task) mutable {	cout << "."; task.get(); tasks.pop_front(); });
		cout << "\n";
=======
void FileSort::sortInput()
{
	cout << "Start sorting file - " << inputPath_ + inputFile_ << "\n";
	std::ifstream input;
	input.open(inputPath_ + inputFile_, std::ios::binary);
	FILETYPE buffer;
	auto tmpCount = 0;
	size_t readCount = 0;
	std::list<std::future<bool>> tasks;
	while (!input.eof())
	{
		VectorUInt data;
		data.reserve(vectorSize_);
		while (readCount < vectorSize_ && input.read(reinterpret_cast<char*>(&buffer), sizeof(FILETYPE)))
		{
			++readCount;
			data.emplace_back(buffer);
		}
		auto ts = tasks.size();
		if (tasks.size() < threadsMaxCount_-1)
		{
			tasks.emplace_back(std::async(std::launch::async, [&tmpCount, mdata = std::move(data), this]() mutable {
				return sortAndSave(tmpCount++, std::move(mdata)); }));
		}
		else
		{
			tasks.front().get();
			tasks.pop_front();
			tasks.emplace_back(std::async(std::launch::async, [&tmpCount, mdata = std::move(data), this]() mutable {
				return sortAndSave(tmpCount++, std::move(mdata)); }));
		}
		readCount = 0;
	} 
	input.close();
	auto tsize = tasks.size();
	cout << "Wating.";
	for_each(tasks.begin(), tasks.end(), [](auto& task) { 
		cout << "."; 
		task.get();});
	cout << "Complete sorting.\n";
	sortAndMerge("output");
}

/*
bool FileSort::sortAndSave(const std::string& outputFile)
{
	std::ofstream output;
	std::sort(data_.begin(), data_.end());
	output.open(outputFile, std::ios::binary);
	if (output)
	{
		for_each(data_.begin(), data_.end(), [&output](FILETYPE i) {output.write(reinterpret_cast<const char*>(&i), sizeof(FILETYPE)); });
		output.close();
	}
	data_.clear();
	return true;
}
*/
bool FileSort::sortAndSave(size_t fileIndex, VectorUInt&& data)
{
	const std::string& outputFile = "_" + std::to_string(fileIndex);
	cout << "Sorting and saving to " << outputFile << "\n";
	std::ofstream output;
	std::sort(data.begin(), data.end());
	filesList_.emplace_back(outputFile);
 	output.open(inputPath_ + outputFile, std::ios::binary);
	for_each(data.begin(), data.end(), [&output](FILETYPE i) {output.write(reinterpret_cast<const char*>(&i), sizeof(FILETYPE)); });
	output.close();
	return true;
}

void FileSort::sortAndMerge(const std::string& outputFile)
{
	std::list<std::future<bool>> tasks;
	auto filesCount = filesList_.size();
	size_t iterations = filesList_.size() / 2;
	size_t iteration = 0;
	auto commit = [&tasks]() mutable {	while (tasks.size() > 0)
	{
		tasks.front().get();
		tasks.pop_front();
		cout << ".";
	} };
	while (filesList_.size() > 2)
	{
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
		auto inFileName1 = filesList_.front();
		filesList_.pop_front();
		auto inFileName2 = filesList_.front();
		filesList_.pop_front();
<<<<<<< HEAD
		if (!merge(inFileName1, inFileName2, outputFile_))
			cout << "Something went wrong.\n";
	}
	catch (const std::exception& ex) {
		cout << "FileSort::startMerge - Exception - " << ex.what() << "\n";
		return;
	}
=======
		auto outfileName = inFileName1 + inFileName2;
		filesList_.emplace_back(outfileName);		
		cout << "Merge " << inFileName1 << " and " << inFileName2 << " into " << outfileName << "\n";
		tasks.emplace_back(std::async(std::launch::async, [inFileName1, inFileName2, outfileName, this]() mutable {			
			return merge(inFileName1, inFileName2, outfileName); }));
		++iteration;
		if (iteration >= iterations)
		{
			cout << "\nCommit.";
			commit();
			cout << "\n";
			iterations = filesList_.size() / 2;
			iteration = 0;
		}
	}
	cout << "Finalize.";
	auto fs = filesList_.size();
	for_each(tasks.begin(), tasks.end(), [&tasks](auto& task) mutable {	cout << "."; task.get(); tasks.pop_front(); });
	cout << "\n";
	auto inFileName1 = filesList_.front();
	filesList_.pop_front();
	auto inFileName2 = filesList_.front();
	filesList_.pop_front();
	merge(inFileName1, inFileName2, outputFile_);
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
}

bool FileSort::merge(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile)
{
<<<<<<< HEAD
	try {
		std::ifstream input1(inputPath_ + inputFile1, std::ios::binary);
		std::ifstream input2(inputPath_ + inputFile2, std::ios::binary);
		std::ofstream output(inputPath_ + outputFile, std::ios::binary);
		FILETYPE buffer1, buffer2;
		bool r1 = false;
		bool r2 = false;
		bool readNext1 = true;
		bool readNext2 = true;
		if (!input1) {
			cout << "Can`t open file - " << inputFile1 << "\n";
			return false;
		}
		if (!input2) {
			cout << "Can`t open file - " << inputFile2 << "\n";
			return false;
		}
		if (!output) {
			cout << "Can`t open file - " << outputFile << "\n";
			return false;
		}

		while (!input1.eof() || !input2.eof())	{
			if (readNext1) r1 = (bool)(input1.read(reinterpret_cast<char*>(&buffer1), sizeof(FILETYPE)));
			if (readNext2) r2 = (bool)(input2.read(reinterpret_cast<char*>(&buffer2), sizeof(FILETYPE)));

			if (r1 && r2) {
				if (buffer1 == buffer2)
				{
					output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
					readNext1 = true;
					output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
					readNext2 = true;
				}
				else
					if (buffer1 < buffer2) {
=======
	std::ifstream input1(inputPath_ + inputFile1, std::ios::binary);
	std::ifstream input2(inputPath_ + inputFile2, std::ios::binary);
	std::ofstream output(inputPath_ + outputFile, std::ios::binary);
	FILETYPE buffer1, buffer2;
	bool r1 = false;
	bool r2 = false;
	bool readNext1 = true;
	bool readNext2 = true;

	if (input1 && input2 && output)
	{
		while (!input1.eof() || !input2.eof())
		{
			if (readNext1) r1 = (bool)(input1.read(reinterpret_cast<char*>(&buffer1), sizeof(FILETYPE)));
			if (readNext2) r2 = (bool)(input2.read(reinterpret_cast<char*>(&buffer2), sizeof(FILETYPE)));

			if (r1 || r2)
			{
				if (r1 && r2)
				{
					if (buffer1 == buffer2)
					{
						output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
						readNext1 = true;
						output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
						readNext2 = true;
					}
					else
					if (buffer1 < buffer2)
					{
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
						output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
						readNext1 = true;
						readNext2 = false;
					}
<<<<<<< HEAD
					else if (buffer1 > buffer2) {
=======
					else
					if (buffer1 > buffer2)
					{
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
						output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
						readNext1 = false;
						readNext2 = true;
					}
<<<<<<< HEAD
			}
			else
				if (r1 && !r2) {
					output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
					readNext1 = true;
					readNext2 = false;
				}
				else  if (!r1 && r2) {
=======
				} else
				if (r1 && !r2)
				{
					output.write(reinterpret_cast<const char*>(&buffer1), sizeof(FILETYPE));
					readNext1 = true;
					readNext2 = false;
				} else
				if (!r1 && r2)
				{
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
					output.write(reinterpret_cast<const char*>(&buffer2), sizeof(FILETYPE));
					readNext1 = false;
					readNext2 = true;
				}
<<<<<<< HEAD
=======
			}
			else break;
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
		}
		input1.close();
		input2.close();
		output.close();
<<<<<<< HEAD
		return true;
	}
	catch (const std::exception& ex) {
		cout << "FileSort::merge - Exception - " << ex.what() << "\n";
		return false;
	}
=======
	}
	return true;
>>>>>>> 96177cc8b3570d4e0d6b5b25bac3d7d28fbf3180
}