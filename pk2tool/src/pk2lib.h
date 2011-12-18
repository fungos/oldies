#ifdef WIN32

#include <io.h>
#include <windows.h>

extern "C" {
	unsigned int PK2Init(const char *pk2_filename, int *pk2);
	unsigned int PK2OpenFile(int pk2, char *filename);
	unsigned int PK2GetFileSize(int pk2, int file_index);
	unsigned int PK2ReadFile(int pk2, int file_index, unsigned char *buffer, int file_size, int *bytes_read);
	unsigned int PK2WriteFileToFilesystem(int pk2, const char *src_folder, const char *dst_folder, const char *filename, int *size);
	unsigned int PK2ReadFileFromFilesystem(int pk2, const char *src_folder, const char *dst_folder, const char *filename, int *nothing);
}

#else

extern "C" {
	unsigned int PK2Init(const char *pk2_filename, int *pk2)
	{
		pk2 = NULL;
		return 1;
	}

	unsigned int PK2OpenFile(int pk2, const char *filename)
	{
		return 1;
	}

	unsigned int PK2GetFileSize(int pk2, int file_index)
	{
		return 1;
	}

	unsigned int PK2ReadFile(int pk2, int file_index, unsigned char *buffer, int file_size, int *bytes_read)
	{
		return 1;
	}

	unsigned int PK2WriteFileToFilesystem(int pk2, const char *src_folder, const char *dst_folder, const char *filename, int *size)
	{
		return 1;
	}

	unsigned int PK2ReadFileFromFilesystem(int pk2, const char *src_folder, const char *dst_folder, const char *filename, int *nothing)
	{
		return 1;
	}
}
#endif
