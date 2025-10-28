#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

class DataRecord
{
public:
	int key;
	int pool[49];
};

class Stopwatch {
private:
	chrono::steady_clock::time_point start;
	chrono::steady_clock::time_point end;
public:
	Stopwatch() {
		this->start = chrono::high_resolution_clock::now();
	}
	double elapsedMilliseconds() {
		this->end = chrono::high_resolution_clock::now();
		chrono::duration<double, std::milli> elapsed = end - start;
		//cout << "Time: " << elapsed.count() << " ms\n";
		return elapsed.count();
	}
};

void generateInitialFile(int recordCount) {
	ofstream outputFile("initial.bin", ios::binary | ios::trunc);
	for (int i = 0; i < recordCount; i++)
	{
		DataRecord record;
		record.key = rand();
		outputFile.write((char*)&record, sizeof(DataRecord));
	}
	outputFile.close();
}

void generatePartiallyOrderedFile(int recordCount, float sortedFraction) {
	ofstream outputFile("initial.bin", ios::binary | ios::trunc);
	int sortedCount = static_cast<int>(recordCount * sortedFraction);
	for (int i = 0; i < sortedCount; i++) {
		DataRecord record;
		record.key = i;  // Упорядоченные данные
		outputFile.write(reinterpret_cast<char*>(&record), sizeof(DataRecord));
	}
	for (int i = sortedCount; i < recordCount; i++) {
		DataRecord record;
		record.key = rand();  // Случайные данные
		outputFile.write(reinterpret_cast<char*>(&record), sizeof(DataRecord));
	}
	outputFile.close();
}

void duplicateFile() {
	ifstream inputFile("initial.bin", ios::binary);
	ofstream outputFile("working.bin", ios::binary | ios::trunc);
	DataRecord record;
	while (!inputFile.eof() && inputFile.read((char*)&record, sizeof(DataRecord)))
	{
		outputFile.write((char*)&record, sizeof(DataRecord));
	}
	outputFile.close();
	inputFile.close();
}

int displayFileContent(const char* filename) {
	int count = 0;
	cout << "FILE: " << filename << endl;
	ifstream inputFile(filename, ios::binary);
	DataRecord record;
	while (inputFile.read((char*)&record, sizeof(DataRecord)) && !inputFile.eof()) {
		cout << record.key << endl;
		count++;
	}
	inputFile.close();
	cout << "--------------------------------" << endl;
	return count;
}

double directMerge() {
	Stopwatch* timer = new Stopwatch();
	fstream workingFile("working.bin", ios::binary | ios::in);
	fstream tempFile1, tempFile2;
	DataRecord record1, record2;
	workingFile.seekg(0, ios::end);
	auto totalRecords = workingFile.tellg() / sizeof(DataRecord);
	workingFile.close();
	int count1 = 0, count2 = 0;
	int seriesLength = 1;

	while (seriesLength < totalRecords) {
		workingFile.open("working.bin", ios::binary | ios::in);
		tempFile1.open("temp1.bin", ios::binary | ios::trunc | ios::out);
		tempFile2.open("temp2.bin", ios::binary | ios::trunc | ios::out);
		while (!workingFile.eof()) {
			count1 = 0;
			count2 = 0;
			while (!workingFile.eof() && count1 < seriesLength && workingFile.read((char*)&record1, sizeof(DataRecord))) {
				count1++;
				tempFile1.write((char*)&record1, sizeof(DataRecord));
			}
			while (!workingFile.eof() && count2 < seriesLength && workingFile.read((char*)&record1, sizeof(DataRecord))) {
				count2++;
				tempFile2.write((char*)&record1, sizeof(DataRecord));
			}
		}
		workingFile.close();
		tempFile1.close();
		tempFile2.close();

		workingFile.open("working.bin", ios::binary | ios::trunc | ios::out);
		tempFile1.open("temp1.bin", ios::binary | ios::in);
		tempFile2.open("temp2.bin", ios::binary | ios::in);

		tempFile1.read((char*)&record1, sizeof(DataRecord));
		tempFile2.read((char*)&record2, sizeof(DataRecord));
		while (!tempFile1.eof() && !tempFile2.eof()) {
			count1 = 0;
			count2 = 0;
			while (count1 < seriesLength && count2 < seriesLength && !tempFile1.eof() && !tempFile2.eof()) {
				if (record1.key < record2.key) {
					workingFile.write((char*)&record1, sizeof(DataRecord));
					tempFile1.read((char*)&record1, sizeof(DataRecord));
					count1++;
				}
				else {
					workingFile.write((char*)&record2, sizeof(DataRecord));
					tempFile2.read((char*)&record2, sizeof(DataRecord));
					count2++;
				}
			}
			while (count1 < seriesLength && !tempFile1.eof()) {
				workingFile.write((char*)&record1, sizeof(DataRecord));
				tempFile1.read((char*)&record1, sizeof(DataRecord));
				count1++;
			}
			while (count2 < seriesLength && !tempFile2.eof()) {
				workingFile.write((char*)&record2, sizeof(DataRecord));
				tempFile2.read((char*)&record2, sizeof(DataRecord));
				count2++;
			}
		}
		while (!tempFile1.eof()) {
			workingFile.write((char*)&record1, sizeof(DataRecord));
			tempFile1.read((char*)&record1, sizeof(DataRecord));
		}
		while (!tempFile2.eof()) {
			workingFile.write((char*)&record2, sizeof(DataRecord));
			tempFile2.read((char*)&record2, sizeof(DataRecord));
		}

		workingFile.close();
		tempFile1.close();
		tempFile2.close();
		seriesLength *= 2;
	}
	double res = timer->elapsedMilliseconds();
	delete timer;
	return res;
}

double naturalMerge() {
	Stopwatch* timer = new Stopwatch();
	fstream workingFile, tempFile1, tempFile2;
	DataRecord record1, record2;
	int splitCount = 1;
	while (splitCount > 0) {
		workingFile.open("working.bin", ios::binary | ios::in);
		tempFile1.open("temp1.bin", ios::binary | ios::out | ios::trunc);
		tempFile2.open("temp2.bin", ios::binary | ios::out | ios::trunc);
		int lastKey = INT_MIN;
		bool toggleFile = true;
		splitCount = 0;
		while (!workingFile.eof() && workingFile.read((char*)&record1, sizeof(DataRecord))) {
			if (lastKey > record1.key) {
				toggleFile = !toggleFile;
				splitCount++;
			}
			lastKey = record1.key;
			if (toggleFile) {
				tempFile1.write((char*)&record1, sizeof(DataRecord));
			}
			else {
				tempFile2.write((char*)&record1, sizeof(DataRecord));
			}
		}
		workingFile.close();
		tempFile1.close();
		tempFile2.close();

		workingFile.open("working.bin", ios::binary | ios::out | ios::trunc);
		tempFile1.open("temp1.bin", ios::binary | ios::in);
		tempFile2.open("temp2.bin", ios::binary | ios::in);

		tempFile1.read((char*)&record1, sizeof(DataRecord));
		tempFile2.read((char*)&record2, sizeof(DataRecord));
		bool readFirst = true, readSecond = true;
		while (!tempFile1.eof() && !tempFile2.eof()) {
			if (readFirst && readSecond) {
				if (record1.key < record2.key) {
					workingFile.write((char*)&record1, sizeof(DataRecord));
					int oldKey = record1.key;
					tempFile1.read((char*)&record1, sizeof(DataRecord));
					if (record1.key < oldKey) readFirst = false;
				}
				else {
					workingFile.write((char*)&record2, sizeof(DataRecord));
					int oldKey = record2.key;
					tempFile2.read((char*)&record2, sizeof(DataRecord));
					if (record2.key < oldKey) readSecond = false;
				}
			}
			else if (readFirst) {
				workingFile.write((char*)&record1, sizeof(DataRecord));
				int oldKey = record1.key;
				tempFile1.read((char*)&record1, sizeof(DataRecord));
				if (record1.key < oldKey) readFirst = false;
			}
			else if (readSecond) {
				workingFile.write((char*)&record2, sizeof(DataRecord));
				int oldKey = record2.key;
				tempFile2.read((char*)&record2, sizeof(DataRecord));
				if (record2.key < oldKey) readSecond = false;
			}
			else {
				readFirst = readSecond = true;
			}
		}
		if (!tempFile1.eof()) workingFile.write((char*)&record1, sizeof(DataRecord));
		while (!tempFile1.eof() && tempFile1.read((char*)&record1, sizeof(DataRecord))) {
			workingFile.write((char*)&record1, sizeof(DataRecord));
		}
		if (!tempFile2.eof()) workingFile.write((char*)&record2, sizeof(DataRecord));
		while (!tempFile2.eof() && tempFile2.read((char*)&record2, sizeof(DataRecord))) {
			workingFile.write((char*)&record2, sizeof(DataRecord));
		}
		workingFile.close();
		tempFile1.close();
		tempFile2.close();
	}
	double res = timer->elapsedMilliseconds();
	delete timer;
	return res;
}

int main()
{
	srand(static_cast<unsigned int>(std::time(0)));
	locale::global(std::locale("ru_RU.UTF-8"));
	cout.imbue(std::locale());

	ofstream fout("result.csv", ios::trunc);
	fout << "Размер;Тип данных;Прямое;Естественное\n";

	int size = 10000;
	vector<string> types = { "Случайные", "Частично упорядоченные", "Полностью упорядоченные" };

	for (int i = 0; i < 100; i++) {
		/*// Случайные данные
		generateInitialFile(size);
		duplicateFile();
		double directRandom = directMerge();
		duplicateFile();
		double naturalRandom = naturalMerge();
		fout << size << ";Случайные;" << directRandom << ";" << naturalRandom << "\n";*/
		
		// Частично упорядоченные данные (50% упорядочено)
		generatePartiallyOrderedFile(size, 0.5);
		duplicateFile();
		double directPartial = directMerge();
		duplicateFile();
		double naturalPartial = naturalMerge();
		fout << size << ";Частично упорядоченные;" << directPartial << ";" << naturalPartial << "\n";

		/*// Полностью упорядоченные данные
		generatePartiallyOrderedFile(size, 1.0);
		duplicateFile();
		double directSorted = directMerge();
		duplicateFile();
		double naturalSorted = naturalMerge();
		fout << size << ";Полностью упорядоченные;" << directSorted << ";" << naturalSorted << "\n";*/
	}

	fout.close();
	//int total = displayFileContent("working.bin");
	//cout << "TOTAL: " << total << endl;
	return 0;
}