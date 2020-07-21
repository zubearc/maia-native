#pragma once

#include <vector>

#include "JEChunk.h"
#include "ChunkSection18.h"
#include "../utils/MinecraftSerialVersions.h"

class AnvilChunkColumn : public JChunkColumn {
public:

	// @extremeheat: Andromeda code
	JavaEditionVersion chunkVersion;

	int x;
	int z;

	std::vector<JChunkSection*> chunkSections;
	std::array<int8_t, 16 * 16> biomes;
	std::array<unsigned short, 16 * 16> heightMap;

	//std::vector<std::shared_ptr<BlockEntity>> mBlockEntities;
	AnvilChunkColumn(int x, int z, std::vector<JChunkSection*> sections) : biomes() {
		this->x = x;
		this->z = z;
		/*if (sections.size() == 0) {
			for (int y = 0; y < 8; y++) {
				sections.push_back(new JChunkSection18(y));
			}
		}*/
		this->chunkSections = sections;
		this->chunkVersion = (JavaEditionVersion)0;

		//printf("!! x=%d, z=%d\n", this->getX(), this->getZ());
	}

	virtual inline int getX() override {
		return this->x;
	}

	virtual inline int getZ() override {
		return this->z;
	}

	bool isLightPopulated();

	void setLightPopulated(bool value);

	bool isTerrainPopulated();

	void setTerrainPopulated(bool value);

	bool isGenerated();

	void setGenerated(bool value);

	inline void setChunkSections(std::vector<JChunkSection*> sections) {
		for (auto section : this->chunkSections) delete section;
		this->chunkSections = sections;
	}

	// getNbtCompound();

	//static ChunkColumn& generateEmptyChunk();

	unsigned char getBlockId(int x, int y, int z);

	void setBlockId(int x, int y, int z, unsigned char blockId);

	void setBlockStateId(int x, int y, int z, int value);

	char getBlockMetadata(int x, int y, int z);

	bool setBlockMetadata(int x, int y, int z, char blockData);

	char getBlockLight(int x, int y, int z);

	bool setBlockLight(int x, int y, int z, char lightLevel);

	char getSkyLight(int x, int y, int z);

	bool setSkyLight(int x, int y, int z, char lightLevel);

	unsigned char getBiome(int x, int z);

	void setBiome(int x, int z, unsigned char biome);

	int getBiomeColor(int x, int z);

	void setBiomeColor(int x, int z, int color);

	void readBiomesFromNetwork(unsigned char* buffer, int length);
	
	~AnvilChunkColumn();
};