#include <Resource/ResourceMgr.h>

// ============================================================================

sf::String ResourceFolder::sResourcePath = "";
FILE* ResourceFolder::sPackedFolder = 0;

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
	// Open folder
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