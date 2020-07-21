#pragma once

#include <string>
#include <array>
#include <memory>
#include "JEChunk.h"
#include "../utils/NibbleArray.h"
#include "../Utils.h"

using namespace Andromeda;

class JChunkSection18 : public JChunkSection {
private:
	// @extremeheat: Andromeda code
    unsigned char y;
    std::array<char, 16 * 16 * 16> blocks;
    // 2048 bytes
    std::unique_ptr<Utils::NibbleArray> blockData;
    std::unique_ptr<Utils::NibbleArray> blockLight;
    std::unique_ptr<Utils::NibbleArray> blockSkyLight;

    inline static int getIndex(int x, unsigned char y, int z) {
        return (y << 8) | (z << 4) | x;
    }
public:
	JChunkSection18(unsigned char y) {
        // @extremeheat: Andromeda code
        this->y = y;
        this->blockData = std::unique_ptr<Utils::NibbleArray>(new Utils::NibbleArray(2048));
        this->blockLight = std::unique_ptr<Utils::NibbleArray>(new Utils::NibbleArray(2048));
        this->blockSkyLight = std::unique_ptr<Utils::NibbleArray>(new Utils::NibbleArray(2048));
	}

	void initialize() {
		this->blocks = std::array<char, 16 * 16 * 16>();
	}

	virtual inline unsigned char getY() override {
		return this->y;
	}

	virtual const bool isPaletted() override {
		return false;
	}

	char* getBlockLightBuffer() {
		return (char*)this->blockLight->get();
	}

	char* getSkyLightBuffer() {
		return (char*)this->blockSkyLight->get();
	}

	inline unsigned char getBlockId(char x, char y, char z) {
		return this->blocks[this->getIndex(x, y, z)];
	}

	inline void setBlockId(char x, char y, char z, unsigned char blockId) {
		this->blocks[this->getIndex(x, y, z)] = blockId;
	}

	inline char getBlockMetadata(char x, char y, char z) {
		return this->blockData->get(this->getIndex(x, y, z));
	}

	inline void setBlockMetadata(char x, char y, char z, char metadata) {
		this->blockData->set(this->getIndex(x, y, z), metadata);
	}

	inline char getBlockLight(char x, char y, char z) {
		return this->blockLight->get(this->getIndex(x, y, z));
	}

	inline void setBlockLight(char x, char y, char z, char blockLight) {
		this->blockLight->set(this->getIndex(x, y, z), blockLight);
	}

	inline char getSkyLight(char x, char y, char z) {
		return this->blockSkyLight->get(this->getIndex(x, y, z));
	}

	inline void setSkyLight(char x, char y, char z, char blockLight) {
		this->blockSkyLight->set(this->getIndex(x, y, z), blockLight);
	}

	// Maia
	inline void readBlocksFromNetwork(unsigned char *buffer, int len) {
		auto data = (unsigned short*)buffer;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				for (int z = 0; z < 16; z++) {
					auto i = this->getIndex(x, y, z);
					auto block_id = data[i] >> 4;
					auto block_meta = data[i] & 15;
					this->setBlockId(x, y, z, block_id);
					this->setBlockMetadata(x, y, z, block_meta);
					//printf("%x ", data[i]);
				}
			}
		}
		//printf("\n\n");
	}

	inline void setBlockStateId(char x, char y, char z, int val) {
		auto i = this->getIndex(x, y, z);
		auto block_id = val >> 4;
		auto block_meta = val & 15;
		this->setBlockId(x, y, z, block_id);
		this->setBlockMetadata(x, y, z, block_meta);
	}
};