#include "AnvilChunkColumn.h"

#define RAISE_ERROR
#define _ASSERT

#include "WorldProvider.h"

bool AnvilChunkColumn::isLightPopulated() {
	return false;
}

void AnvilChunkColumn::setLightPopulated(bool value) {

}

bool AnvilChunkColumn::isTerrainPopulated() {
	return false;
}

void AnvilChunkColumn::setTerrainPopulated(bool value) {
}

bool AnvilChunkColumn::isGenerated() {
	return false;
}

void AnvilChunkColumn::setGenerated(bool value) {
}

#define SECTION_CHECK \
		int Y = y >> 4; int a = this->chunkSections.size(); \
		if (Y > (a - 1)) return 0; \
		auto *chunk = this->chunkSections[Y];

#define LEGACY_CHUNK static_cast<JChunkSection18*>(chunk)

unsigned char AnvilChunkColumn::getBlockId(int x, int y, int z) {
	SECTION_CHECK;

	/*if (chunk->isPaletted()) {
		auto ablock = chunk->getBlock(x, y, z);
		unsigned char blockid;
		char meta;
		ablock.getEncapsulatedBlock()->getJavaBlock(ablock.variant, blockid, meta);
		return blockid;
	}
	else*/
	//printf("GetBlock %d %d/%d/%d %d --> %d\n", x, Y, y, y&15,z, LEGACY_CHUNK->getBlockId(x, y & 15, z));
		return LEGACY_CHUNK->getBlockId(x, y & 15, z);
}

void AnvilChunkColumn::setBlockId(int x, int y, int z, unsigned char blockId) {
	int Y = y >> 4;
	JChunkSection *chunk = nullptr;
	int sections = this->chunkSections.size();
	auto a = sections - 1;
	if (Y > a) {
		if (Y < 0 || Y > 16) {
			RAISE_ERROR("y-value is greater than 256 blocks");
			return;
		} else {
			for (size_t i = sections; i <= Y; i++) {
				auto section = new JChunkSection18(i);
				section->initialize(); // zero initialize
				this->chunkSections.push_back(section);
				printf("Added section %d, size of sections is now %d\n", i + 1, this->chunkSections.size());
			}
			chunk = this->chunkSections[Y];
			LEGACY_CHUNK->setBlockId(x, y & 15, z, blockId);
		}
	} else {
		chunk = this->chunkSections[Y];
		//printf("Set %d %d\n", Y, sections);
		/*if (chunk->isPaletted()) {
			auto ablock = BlockFactory::getBlockFromJava(blockId, 0);
			chunk->setBlock(x, y - 16 * Y, z, ablock);
		}
		else {*/
		printf("SetBlock %d %d/%d/%d %d --> %d\n", x, Y, y, y & 15, z, blockId);
		LEGACY_CHUNK->setBlockId(x, y & 15, z, blockId);
		//}
	}
}

void AnvilChunkColumn::setBlockStateId(int x, int y, int z, int value) {
	printf("SetBlockState %d %d %d --> %d\n", x, y, z, value);
	auto block_id = value >> 4;
	auto block_meta = value & 15;
	this->setBlockId(x, y, z, block_id);
	this->setBlockMetadata(x, y, z, block_meta);
}

char AnvilChunkColumn::getBlockMetadata(int x, int y, int z) {
	SECTION_CHECK;

	/*if (chunk->isPaletted()) {
		auto ablock = chunk->getBlock(x, y, z);
		unsigned char blockid;
		char metadata;
		ablock.block->getJavaBlock(ablock.variant, blockid, metadata);
		return metadata;
	}
	else*/
		return LEGACY_CHUNK->getBlockMetadata(x, y & 15, z);
}

bool AnvilChunkColumn::setBlockMetadata(int x, int y, int z, char blockData) {
	SECTION_CHECK;

	/*if (chunk->isPaletted()) {
		auto ablock = BlockFactory::getBlockFromJava(getBlockId(x, y, z), blockData);
		chunk->setBlock(x, y - 16 * Y, z, ablock);
	}
	else*/
		LEGACY_CHUNK->setBlockMetadata(x, y & 15, z, blockData);

	return true;
}

char AnvilChunkColumn::getBlockLight(int x, int y, int z) {
	SECTION_CHECK;

	return chunk->getBlockLight(x, y & 15, z);
}

bool AnvilChunkColumn::setBlockLight(int x, int y, int z, char lightLevel) {
	SECTION_CHECK;

	chunk->setBlockLight(x, y & 15, z, lightLevel);

	return true;
}

char AnvilChunkColumn::getSkyLight(int x, int y, int z) {
	SECTION_CHECK;

	return chunk->getSkyLight(x, y & 15, z);
}

bool AnvilChunkColumn::setSkyLight(int x, int y, int z, char lightLevel) {
	SECTION_CHECK;

	chunk->setSkyLight(x, y & 15, z, lightLevel);

	return true;
}

BlockProps AnvilChunkColumn::getBlockProperties(int x, int y, int z) {
	SECTION_CHECK;

	return chunk->getBlockProperties(x, y & 15, z);
}

bool AnvilChunkColumn::setBlockProperties(int x, int y, int z, BlockProps properties) {
	SECTION_CHECK;

	chunk->setBlockProperties(x, y & 15, z, properties);
	return true;
}

unsigned char AnvilChunkColumn::getBiome(int x, int z) {
	return biomes[(x << 4) | z];
}

void AnvilChunkColumn::setBiome(int x, int z, unsigned char biome) {
	biomes[(x << 4) | z] = biome;
}

int AnvilChunkColumn::getBiomeColor(int x, int z) {
	return 0;

}

void AnvilChunkColumn::setBiomeColor(int x, int z, int color) {

}

void AnvilChunkColumn::readBiomesFromNetwork(unsigned char* buffer, int length) {
	_ASSERT(length == 16 * 16);
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			auto i = z * 16 + x;
			this->setBiome(x, z, buffer[i]);
		}
	}
}

AnvilChunkColumn::~AnvilChunkColumn() {
	WorldProvider::removeLoadedChunk(this);
	for (auto section : chunkSections) {
		delete section;
	}
}