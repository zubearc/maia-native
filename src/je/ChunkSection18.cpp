#pragma once

#include "ChunkSection18.h"
#include <string>
#include <array>
#include <memory>

#include "../blocks/MinecraftBlockHelper.h"

void JChunkSection18::updateBlockProperties(char x, char y, char z) {
	if (this->blockProperties == nullptr) {
		return; //haven't loaded blockProperties
	}

	auto block = this->getBlockId(x, y, z);
	auto i = this->getIndex(x, y, z);
	this->blockProperties[i] = MinecraftBlockHolder::getJavaBlockProperties(block, 0);
}


void JChunkSection18::setBlockId(char x, char y, char z, unsigned char blockId) {
	auto i = this->getIndex(x, y, z);
	this->blocks[i] = blockId;

	// TODO: Clean up & post-processing impl
	if (this->blockProperties) {
		//auto props = MinecraftBlockHolder::getJavaBlockProperties(blockId, 0);

		//printf("%d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d\n", blockId, (props & BlockProp::AIR) > 0, (props & BlockProp::SOLID) > 0,
		//	(props & BlockProp::DESTROYABLE) > 0, (props & BlockProp::TRANSPARENT) > 0);

		this->blockProperties[i] = MinecraftBlockHolder::getJavaBlockProperties(blockId, 0);
	}

}
