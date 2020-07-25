#include "WorldProvider.h"

#define printf
namespace WorldProvider {

	std::map<uint64_t, AnvilChunkColumn*> loadedAnvilChunks;

	uint64_t lastChunkIndex = 0;
	AnvilChunkColumn* lastChunk = 0;

	inline uint64_t getIndex(int x, int z) {
		uint64_t i = ((uint64_t)x << 31) | (uint64_t)z & 0xffffffff;
		return i;
	}

	void addLoadedChunk(AnvilChunkColumn* col) {
		removeLoadedChunk(col);
		// max X/Z 1874999
		//uint64_t i = ((uint64_t)col->getX() << 31) | (uint64_t)col->getZ() & 0xffffffff;
		auto i = getIndex(col->getX(), col->getZ());
		loadedAnvilChunks[i] = col;

		printf("[%d, %d] Loaded chunk %lld \n", col->getX(), col->getZ(), i);
	}
	
	void removeLoadedChunk(AnvilChunkColumn* col) {
		int eraseIndex = -1;

		//todo: see if we can use std::erase_if with C++20
		for (auto it = loadedAnvilChunks.cbegin(), next_it = it; it != loadedAnvilChunks.cend(); it = next_it) {
			++next_it;
			if (it->second == col) {
				printf("[%d, %d] Unloaded world \n", col->getX(), col->getZ());
				loadedAnvilChunks.erase(it);
			}
		}
		lastChunk = 0;
		lastChunkIndex = 0;
	}

	void removeLoadedChunk(int chunkX, int chunkZ) {
		auto i = getIndex(chunkX, chunkZ);
		for (auto it = loadedAnvilChunks.cbegin(), next_it = it; it != loadedAnvilChunks.cend(); it = next_it) {
			++next_it;
			if (it->first == i) {
				printf("[%d, %d] Unloaded world \n", it->second->getX(), it->second->getZ());
				loadedAnvilChunks.erase(it);
			}
		}
		lastChunk = 0;
		lastChunkIndex = 0;
	}

	bool isChunkLoaded(int chunkX, int chunkZ) {
		auto i = getIndex(chunkX, chunkZ);

		if (loadedAnvilChunks.find(i) == loadedAnvilChunks.end()) {
			// not found
			printf("[%d, %d] NOT LOADED! \n", chunkX, chunkZ);
			return false;
		} else {
			// found
			return true;
		}
	}
	
	AnvilChunkColumn* getChunkAt(int worldX, int worldY, int worldZ) {
		auto cx = worldX >> 4;
		auto cz = worldZ >> 4;
		auto cy = worldY >> 4;
		auto i = getIndex(cx, cz);
		printf("CX: %d, CZ: %d -> %lld\n", cx, cz, i);

		if (lastChunk && lastChunkIndex == i) {
			return lastChunk;
		}

		if (loadedAnvilChunks.find(i) == loadedAnvilChunks.end()) {
			// not found
			return 0;
		} else {
			// found
			auto chunk = loadedAnvilChunks[i];
			lastChunkIndex = i;
			lastChunk = chunk;
			if (chunk->chunkSections.size() > cy) {
				printf("ACX: %d, ACZ: %d\n", chunk->getX(), chunk->getZ());
				return chunk;
			}
			return 0;
		}
		return 0;
	}
	
	bool getBlock(int worldX, int worldY, int worldZ, short& blockId, short& blockMetadata) {
		auto chunk = getChunkAt(worldX, worldY, worldZ);
		if (!chunk) {
			return false;
		}
		auto sx = worldX & 15;
		//auto sy = worldY & 15; // <-- this is calculated already in getBlockId in AnvilChunkColumn
		auto sz = worldZ & 15;
		blockId = chunk->getBlockId(sx, worldY, sz);
		blockMetadata = chunk->getBlockMetadata(sx, worldY, sz);
		printf("[%d, %d] X: %d, Y: %d, Z: %d -> %d:%d\n", chunk->getX(), chunk->getZ(), worldX, worldY, worldZ, blockId, blockMetadata);
		return true;
	}

	bool getBlockId(int worldX, int worldY, int worldZ, short& blockId) {
		auto chunk = getChunkAt(worldX, worldY, worldZ);
		if (!chunk) {
			return false;
		}
		auto sx = worldX & 15;
		//auto sy = worldY & 15; // <-- this is calculated already in getBlockId in AnvilChunkColumn
		auto sz = worldZ & 15;
		blockId = chunk->getBlockId(sx, worldY, sz);
		return true;
	}

	BlockProps getBlockProperties(int worldX, int worldY, int worldZ) {
		auto chunk = getChunkAt(worldX, worldY, worldZ);
		if (!chunk) {
			return false;
		}
		auto sx = worldX & 15;
		//auto sy = worldY & 15; // <-- this is calculated already in getBlockId in AnvilChunkColumn
		auto sz = worldZ & 15;
		return chunk->getBlockProperties(sx, worldY, sz);
	}
	
	bool setBlockProperties(int worldX, int worldY, int worldZ, BlockProps props) {
		auto chunk = getChunkAt(worldX, worldY, worldZ);
		if (!chunk) {
			return false;
		}
		auto sx = worldX & 15;
		//auto sy = worldY & 15; // <-- this is calculated already in getBlockId in AnvilChunkColumn
		auto sz = worldZ & 15;
		return chunk->setBlockProperties(sx, worldY, sz, props);
	}
}