/*
 * Copyright (C) 2021, Brian Brice. All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "targetver.h"
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////

static const std::string g_junk_filename = "junkfile";
static const std::string g_junk_filename_extension = "bin";

////////////////////////////////////////////////////////////////////////////////

static int64_t get_free_space(void)
{
	ULARGE_INTEGER free_bytes_avail;

	if (GetDiskFreeSpaceEx(nullptr, &free_bytes_avail, nullptr, nullptr))
		return static_cast<int64_t>(free_bytes_avail.QuadPart);

	return 0;
}

static bool file_exists(const std::string& filename)
{
	return GetFileAttributes(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

static void get_next_filename(const std::string& filename_prefix,
                              const std::string& extension,
                                    std::string& filename)
{
	std::ostringstream os;
	os << filename_prefix << '.' << extension;

	for (size_t i = 1; file_exists(os.str()); i++) {
		os.str("");
		os << filename_prefix << std::setw(3) << std::setfill('0') << i << '.' << extension;
	}

	filename = os.str();
}

static bool create_junk_file(const std::string& filename, int64_t size)
{
	bool succ = false;

	HANDLE file_handle = CreateFile(filename.c_str(), GENERIC_WRITE, 0, nullptr,
	                                CREATE_NEW, FILE_FLAG_NO_BUFFERING, nullptr);

	if (file_handle != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER file_pos;
		file_pos.QuadPart = size;

		if (SetFilePointerEx(file_handle, file_pos, nullptr, FILE_BEGIN))
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

	char* units;
	int64_t size = strtoll(argv[1], &units, 10), free_space = get_free_space();

	if (size == 0)
		exit_msg("could not parse argument, file size");

	if (units && *units != '\0') {
		switch (*units) {
			case 't':
			case 'T':
				size *= 1024;
			case 'g':
			case 'G':
				size *= 1024;
			case 'm':
			case 'M':
				size *= 1024;
			case 'k':
			case 'K':
				size *= 1024;
				break;
			default:
				exit_msg("unknown unit");
		}
	}

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
