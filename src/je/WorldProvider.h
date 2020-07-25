#pragma once

#include <vector>
#include <map>
#include "AnvilChunkColumn.h"
#include "../Utils.h"

// When we load different "dimensions" we unload all the previous dimensions' chunks
// So we do not have to worry about that complexity here and use a single WorldProvider

namespace WorldProvider {

	//std::vector<AnvilChunkColumn*> loadedAnvilChunks;
	extern std::map<uint64_t, AnvilChunkColumn*> loadedAnvilChunks;

	// Quick access last chunk
	extern uint64_t lastChunkIndex;
	extern AnvilChunkColumn* lastChunk;

	void addLoadedChunk(AnvilChunkColumn* col);

	void removeLoadedChunk(AnvilChunkColumn* col);
	void removeLoadedChunk(int chunkX, int chunkZ);

	bool isChunkLoaded(int chunkX, int chunkZ);

	AnvilChunkColumn* getChunkAt(int worldX, int worldY, int worldZ);

	//TODO: To add support for JE 1.13/BE 1.3+, we could return a BlockState integer instead
	bool getBlock(int worldX, int worldY, int worldZ, short &blockId, short &blockMetadata);
	bool getBlockId(int worldX, int worldY, int worldZ, short& blockId);

	//bool setBlockId(int worldX, int worldY, int worldZ, char blockId);
	//bool setBlockMetadata(int worldX, int worldY, int worldZ, char metadata);

	BlockProps getBlockProperties(int worldX, int worldY, int worldZ);
	//float getBlockHardness(int worldX, int worldY, int worldZ);

	bool setBlockProperties(int worldX, int worldY, int worldZ, BlockProps props);
};