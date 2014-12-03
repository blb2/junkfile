#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////

static const std::string g_junk_filename = "junkfile";
static const std::string g_junk_filename_extension = "bin";

////////////////////////////////////////////////////////////////////////////////

static __int64 get_free_space(void)
{
	ULARGE_INTEGER free_bytes_avail;

	if (GetDiskFreeSpaceEx(nullptr, &free_bytes_avail, nullptr, nullptr) != FALSE)
		return static_cast<__int64>(free_bytes_avail.QuadPart);

	return 0;
}

static bool file_exists(const std::string& filename)
{
	return GetFileAttributes(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static void get_next_filename(const std::string& filename_prefix, const std::string& extension, std::string& filename)
{
	std::ostringstream os;
	os << filename_prefix << '.' << extension;

	for (size_t i = 1; file_exists(os.str()); i++)
	{
		os.str("");
		os << filename_prefix << std::setw(3) << std::setfill('0') << i << '.' << extension;
	}

	filename = os.str();
}

static bool create_junk_file(const std::string& filename, const __int64 size)
{
	bool succ = false;

	HANDLE file_handle = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_FLAG_NO_BUFFERING, NULL);
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER file_pos;
		file_pos.QuadPart = size;

		if (SetFilePointerEx(file_handle, file_pos, NULL, FILE_BEGIN) != FALSE)
			succ = (SetEndOfFile(file_handle) != FALSE);

		CloseHandle(file_handle);

		if (!succ)
			DeleteFile(filename.c_str());
	}

	return succ;
}

static void exit_msg(const std::string& msg)
{
	std::cerr << "error: " << msg << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		exit_msg("expected 1 argument, file size");

	__int64 size = 0, free_space = get_free_space();
	if (sscanf(argv[1], "%I64d", &size) != 1)
		exit_msg("could not parse argument, file size");

	size *= 1024 * 1024; // MiB

	if (size < 0)
		size = free_space + size;

	if (size < 0 || free_space < size)
		exit_msg("file size is out of range");

	std::string filename;
	get_next_filename(g_junk_filename, g_junk_filename_extension, filename);

	if (!create_junk_file(filename, size))
		exit_msg("could not create junk file of requested size");

	std::cout << "created file: " << filename << ", " << size << " bytes" << std::endl;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
