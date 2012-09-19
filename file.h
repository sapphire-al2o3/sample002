#ifndef _FILE_H
#define _FILE_H

#include <windows.h>
#include <stdio.h>

class File {
private:
	HANDLE handle;
public:
	bool Open(const char *fileName) {
		handle = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		return handle != INVALID_HANDLE_VALUE;
	}
	
	int GetSize() {
		return GetFileSize(handle, 0);
	}
	
	int Read(void *buffer, int size) {
		unsigned long readSize;
		ReadFile(handle, buffer, size, &readSize, 0);
		return readSize;
	}
	
	void Close() {
		if(!CloseHandle(handle)) {
			// error
		}
		handle = 0;
	}
};

class FileWriter {
private:
	HANDLE handle;
public:
	bool Open(const char *fileName) {
		handle = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		return handle != INVALID_HANDLE_VALUE;
	}

	int Write(void *buffer, int size) {
		unsigned long writeSize;
		WriteFile(handle, buffer, size, &writeSize, 0);
		return writeSize;
	}

	void Close() {
		if(!CloseHandle(handle)) {
			// error;
		}
		handle = 0;
	}
};

/*
 * input file stream
 */
class FileStream {
private:
	File file;
	unsigned char *buffer;
	unsigned char *memory;
	int size;
public:
	FileStream() : buffer(0), memory(0), size(0) {}

	~FileStream() {
		if(memory) {
			Close();
		}
	}
	
	bool Open(const char *fileName) {
		if(file.Open(fileName)) {
			size = file.GetSize();
			buffer = static_cast<unsigned char *>(HeapAlloc(GetProcessHeap(), 0, size));
			if(buffer) {
				file.Read(buffer, size);
				memory = buffer;
				return true;
			}
		}
		return false;
	}
	
	void Read(void *p, int length) {
		CopyMemory(p, buffer, length);
		buffer += length;
	}
	
	template <typename T>
	void Read(T *p) {
		CopyMemory(p, buffer, sizeof(T));
		buffer += sizeof(T);
	}
	
	void Skip(int length) {
		buffer += length;
	}
	
	void Close() {
		file.Close();
		if(HeapFree(GetProcessHeap(), 0, memory) == 0) {
			// error
		}
		memory = 0;
	}
};

class OutputFileStream {
private:
	FileWriter file;
	unsigned char *buffer;
	unsigned char *memory;
	int size;
public:
	OutputFileStream() : buffer(0), memory(0), size(0) {}
	
	bool Open(const char *fileName, int writeSize) {
		if(file.Open(fileName)) {
			size = writeSize;
			buffer = static_cast<unsigned char *>(HeapAlloc(GetProcessHeap(), 0, size));
			if(buffer) {
				memory = buffer;
				return true;
			}
		}
		return false;
	}

	void Write(const void *p, int length) {
		MoveMemory(buffer, p, length);
		buffer += length;
	}

	void Skip(int length) {
		//while(length--) {
		//	*buffer++ = 0;
		//}
		ZeroMemory(buffer, length);
		buffer += length;
	}

	void Close() {
		file.Write(memory, size);
		file.Close();
		if(HeapFree(GetProcessHeap(), 0, memory) == 0) {
			// error
		}
	}
};

#endif // _FILE_H
