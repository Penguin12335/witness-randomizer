#pragma once
#include <vector>
#include <map>
#include <windows.h>

// https://github.com/erayarslan/WriteProcessMemory-Example
// http://stackoverflow.com/q/32798185
// http://stackoverflow.com/q/36018838
// http://stackoverflow.com/q/1387064
class Memory
{
public:
	Memory(const std::string& processName);
	~Memory();

	template<class T>
	std::vector<T> ReadData(const std::vector<int>& offsets, int numItems) {
		std::vector<T> data;
		data.resize(numItems);
		for (int i=0; i<5; i++) {
			if (ReadProcessMemory(_handle, (LPVOID)ComputeOffset(offsets), &data[0], sizeof(T) * numItems, NULL))
			{
				return data;
			}
		}
		ThrowError();
	}

	template <class T>
	void WriteData(const std::vector<int>& offsets, const std::vector<T>& data) {
		for (int i=0; i<5; i++) {
			if (WriteProcessMemory(_handle, (LPVOID)ComputeOffset(offsets), &data[0], sizeof(T) * data.size(), NULL)) {
				return;
			}
		}
		ThrowError();
	}
	
private:
	void ThrowError();

	uintptr_t ComputeOffset(std::vector<int> offsets);
	uintptr_t _baseAddress;
	HANDLE _handle;
};