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
#include <zlib.h>

#include <vector>
#include <queue>

using namespace vne;

// ============================================================================

sf::String ResourceFolder::sResourcePath = "";
FILE* ResourceFolder::sPackedFolder = 0;
std::unordered_map<std::basic_string<Uint32>, Uint32> ResourceFolder::sPackedFolderMap;
const Uint8* ResourceFolder::sResourceKey = 0;

Uint8 gIV[] =
{
	0x00,
	0x01,
	0x02,
	0x03,
	0x04,
	0x05,
	0x06,
	0x07,
	0x08,
	0x09,
	0x0a,
	0x0b,
	0x0c,
	0x0d,
	0x0e,
	0x0f
};

// ============================================================================

void ResourceFolder::setPath(const sf::String& path)
{
	sResourcePath = path;

	// Open and read header if it is a packed folder
	sPackedFolder = FOPEN(path, "rb");
	if (sPackedFolder)
	{
		Uint32 fnameSize = 0;

		// Read file name string size
		// This read is used to determine when file is over
		while (fread(&fnameSize, sizeof(Uint32), 1, sPackedFolder))
		{
			// Read string
			Uint32* fnameData = (Uint32*)malloc((fnameSize + 1) * sizeof(Uint32));
			fread(fnameData, sizeof(Uint32), fnameSize, sPackedFolder);
			fnameData[fnameSize] = 0;
			sf::String fname(fnameData);
			// Free string data
			free(fnameData);

			// Map file name to file offset
			Uint32 fpos = (Uint32)ftell(sPackedFolder);
			sPackedFolderMap[fname.toUtf32()] = fpos;

			// Read to info bytes
			Uint16 temp;
			fread(&temp, 2, 1, sPackedFolder);

			// Get file sizes
			Uint32 u_size = 0, c_size = 0;
			fread(&u_size, sizeof(Uint32), 1, sPackedFolder);
			fread(&c_size, sizeof(Uint32), 1, sPackedFolder);
			// Pad size
			if (c_size % 16 != 0)
				c_size += 16 - c_size % 16;

			// Seek next file
			fseek(sPackedFolder, c_size, SEEK_CUR);
		}
	}
}

// ============================================================================

void ResourceFolder::setKey(const Uint8* key)
{
	sResourceKey = key;
}

// ============================================================================

Uint8* ResourceFolder::open(const sf::String& path, Uint32& size)
{
	if (sPackedFolder)
		return openPacked(path, size);
	else
		return openNormal(path, size);
}

// ============================================================================

Uint8* ResourceFolder::openNormal(const sf::String& path, Uint32& size)
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

Uint8* ResourceFolder::openPacked(const sf::String& path, Uint32& size)
{
	// Get file offset
	auto it = sPackedFolderMap.find(path.toUtf32());
	if (it == sPackedFolderMap.end()) return 0;
	Uint32 offset = it->second;


	// Go to offset
	fseek(sPackedFolder, offset, SEEK_SET);

	// Read compression / encryption info
	bool isCompressed, isEncrypted;
	fread(&isCompressed, sizeof(bool), 1, sPackedFolder);
	fread(&isEncrypted, sizeof(bool), 1, sPackedFolder);

	// Read file sizes
	Uint32 u_size, c_size;
	fread(&u_size, sizeof(Uint32), 1, sPackedFolder);
	fread(&c_size, sizeof(Uint32), 1, sPackedFolder);
	// Pad compression size
	Uint32 c_size_p = c_size;
	if (c_size_p % 16 != 0)
		c_size_p += 16 - c_size_p % 16;

	// Allocate and read data
	Uint8* c_data = (Uint8*)malloc(c_size_p);
	fread(c_data, c_size_p, 1, sPackedFolder);


	// Decrypt
	if (isEncrypted)
	{
		AES_ctx context;
		AES_init_ctx_iv(&context, sResourceKey, gIV);
		AES_CBC_decrypt_buffer(&context, c_data, c_size_p);
	}


	// Decompress
	Uint8* u_data = c_data;
	if (isCompressed)
	{
		u_data = (Uint8*)malloc(u_size);
		int result = uncompress(u_data, (uLongf*)&u_size, c_data, c_size);

		if (result < 0)
			return 0;
	}


	// Free compressed data if necessary
	if (u_data != c_data)
		free(c_data);


	// Return values
	size = u_size;
	return u_data;
}

// ============================================================================
// ============================================================================

void ResourceFolder::pack(const sf::String& dst)
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


	for (Uint32 i = 0; i < files.size(); ++i)
	{
		FILE* f = FOPEN(files[i], "rb");
		if (!f) continue;

		// Get file size
		fseek(f, 0, SEEK_END);
		Uint32 fsize = (Uint32)ftell(f);
		fseek(f, 0, SEEK_SET);

		if (!fsize) continue;


		// Calculate uncompressed sizes
		Uint32 u_size = fsize;
		Uint32 u_size_p = u_size;
		if (u_size_p % 16 != 0)
			u_size_p += 16 - u_size_p % 16;

		// Create buffer using padded size
		Uint8* u_data = (Uint8*)malloc(u_size_p);
		// Read data using unpadded size
		fread(u_data, u_size, 1, f);

		// Close file
		fclose(f);


		// Calculate compressed size, compressed size is not certain yet, so don't calculate padded size yet
		Uint32 c_size = (Uint32)(u_size * 1.1) + 12;

		// Create buffer for compressed data
		Uint8* c_data = (Uint8*)malloc(c_size);
		// Compress data
		compress(c_data, (uLongf*)&c_size, u_data, u_size);

		// Calculate compressed padded buffer size
		Uint32 c_size_p = c_size;
		if (c_size_p % 16 != 0)
			c_size_p += 16 - c_size_p % 16;


		// Determine which data to use based on their sizes
		Uint8* f_data = c_data;
		Uint32 f_u_size = u_size;
		Uint32 f_c_size = c_size;
		Uint32 f_c_size_p = c_size_p;
		if (f_c_size >= f_u_size)
		{
			// Use uncompressed data if it is smaller than compressed version
			f_data = u_data;
			f_c_size = u_size;
			f_c_size_p = u_size_p;
		}


		// Encrypt data if key is provided
		if (sResourceKey)
		{
			AES_ctx context;
			AES_init_ctx_iv(&context, sResourceKey, gIV);
			AES_CBC_encrypt_buffer(&context, f_data, f_c_size_p);
		}


		// Write filename
		sf::String fname(files[i].substring(dirLen));
		Uint32 fnameLen = fname.getSize();
		fwrite(&fnameLen, sizeof(Uint32), 1, packed);
		fwrite(fname.getData(), sizeof(Uint32), fnameLen, packed);

		// Write compression / encryption info
		bool isCompressed = f_data == c_data;
		bool isEncrypted = sResourceKey ? true : false;
		fwrite(&isCompressed, sizeof(bool), 1, packed);
		fwrite(&isEncrypted, sizeof(bool), 1, packed);

		// Write data
		fwrite(&f_u_size, sizeof(Uint32), 1, packed);
		fwrite(&f_c_size, sizeof(Uint32), 1, packed);
		fwrite(f_data, f_c_size_p, 1, packed);


		// Free data
		free(u_data);
		free(c_data);
	}

	// Close packed folder
	fclose(packed);
}

// ============================================================================