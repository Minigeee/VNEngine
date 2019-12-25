#include <Resource/ResourceMgr.h>

#ifdef _WIN32
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#endif
#include <tinydir.h>
#include <aes.hpp>
#include <zlib.h>

#include <vector>
#include <queue>

// ============================================================================

sf::String ResourceFolder::sResourcePath = "";
FILE* ResourceFolder::sPackedFolder = 0;
const Uint8* ResourceFolder::sResourceKey = 0;

// ============================================================================

void ResourceFolder::SetPath(const sf::String& path)
{
	sResourcePath = path;

	// Open and read header if it is a packed folder
	sPackedFolder = FOPEN(path, "rb");
	if (sPackedFolder)
	{
		/* TODO : Implement packed folder */
	}
}

// ============================================================================

void ResourceFolder::SetKey(const Uint8* key)
{
	sResourceKey = key;
}

// ============================================================================

Uint8* ResourceFolder::Open(const sf::String& path, Uint32& size)
{
	if (sPackedFolder)
		return OpenPacked(path, size);
	else
		return OpenNormal(path, size);
}

// ============================================================================

Uint8* ResourceFolder::OpenNormal(const sf::String& path, Uint32& size)
{
	// Open file
	sf::String fname(sResourcePath + "/" + path);
	FILE* f = FOPEN(fname, "rb");
	if (!f) return 0;

	// Get file size
	fseek(f, 0, SEEK_END);
	size = (Uint32)ftell(f);
	fseek(f, 0, SEEK_SET);

	if (!size) return 0;

	// Allocate space and read data
	Uint8* data = (Uint8*)malloc(size);
	fread(data, size, 1, f);

	return data;
}

// ============================================================================

Uint8* ResourceFolder::OpenPacked(const sf::String& path, Uint32& size)
{
	/* TODO : Implement packed folder */
	return 0;
}

// ============================================================================
// ============================================================================

void ResourceFolder::Pack(const sf::String& dst)
{
	// Get length of directory path
	Uint32 dirLen = (Uint32)sResourcePath.getSize() + 1;

	std::vector<sf::String> files;
	std::queue<sf::String> dirs;
	dirs.push(sResourcePath);

	sf::String currentDir(".");
	sf::String parentDir("..");
	

	while (!dirs.empty())
	{
		sf::String& dirPath = dirs.front();

		// Open resource folder
		tinydir_dir dir;
#ifdef _WIN32
		tinydir_open(&dir, dirPath.toWideString().c_str());
#else
		tinydir_open(&dir, dirPath.toAnsiString().c_str());
#endif

		// Remove from queue
		dirs.pop();

		while (dir.has_next)
		{
			tinydir_file file;
			tinydir_readfile(&dir, &file);
			sf::String path(file.path);

			if (file.is_dir)
			{
				sf::String name(file.name);

				if (name != currentDir && name != parentDir)
					dirs.push(path);
			}
			else
				files.push_back(path);

			tinydir_next(&dir);
		}

		// Close directory
		tinydir_close(&dir);
	}


	// Open packed folder
	FILE* packed = FOPEN(dst, "wb");
	if (!packed)
		packed = FOPEN(dst, "ab");
	if (!packed) return;

	// Start encryption context if key is provided
	AES_ctx encryptContext;
	if (sResourceKey)
		AES_init_ctx(&encryptContext, sResourceKey);


	for (Uint32 i = 0; i < files.size(); ++i)
	{
		FILE* f = FOPEN(files[i], "rb");
		if (!f) continue;

		// Get file size
		fseek(f, 0, SEEK_END);
		Uint32 size = (Uint32)ftell(f);
		fseek(f, 0, SEEK_SET);

		if (!size) continue;

		// Allocate space and read data, make sure buffer is padded to multiple of 16 in case this is encrypted
		Uint32 paddedSize = size + (16 - size % 16);
		Uint8* data = (Uint8*)malloc(paddedSize);
		fread(data, size, 1, f);

		// Close file
		fclose(f);

		// Make sure compressed buffer is padded to multiple of 16
		Uint32 compressedSize = (Uint32)(size * 1.1) + 12;
		compressedSize += 16 - compressedSize % 16;
		// Allocate space for compressed data
		Uint8* compressedData = (Uint8*)malloc(compressedSize);

		// Compress
		compress(compressedData, (uLongf*)&compressedSize, data, size);

		// Choose which data to use based on size
		Uint8* writeData = compressedData;
		Uint32 writeSize = compressedSize;
		if (size < compressedSize)
		{
			writeData = data;
			writeSize = sResourceKey ? paddedSize : size;
		}

		// Encrypt if key is provided
		if (sResourceKey)
		{
			if (writeSize % 16 != 0)
				writeSize += 16 - writeSize % 16;
			AES_CBC_encrypt_buffer(&encryptContext, writeData, writeSize);
		}


		// Write file name relative to resource folder
		sf::String fname(files[i].substring(dirLen));
		Uint32 fnameSize = fname.getSize();
		std::wstring test(fname);

		fwrite(&fnameSize, sizeof(Uint32), 1, packed);
		fwrite(fname.getData(), sizeof(Uint32), fnameSize, packed);

		// Write booleans for encryption / compression info
		bool isCompressed = writeData == compressedData;
		bool isEncrypted = sResourceKey ? true : false;
		fwrite(&isCompressed, sizeof(bool), 1, packed);
		fwrite(&isEncrypted, sizeof(bool), 1, packed);

		// Write data size, then the data
		fwrite(&writeSize, sizeof(Uint32), 1, packed);
		fwrite(writeData, writeSize, 1, packed);


		// Free data
		free(data);
		free(compressedData);
	}

	// Close packed folder
	fclose(packed);
}

// ============================================================================