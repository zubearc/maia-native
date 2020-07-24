#include "MinecraftBlockHelper.h"

void MinecraftBlockHolder::init() {
	initBlockProperties();
}

BlockProps MinecraftBlockHolder::getJavaBlockProperties(int block_id, int block_meta) {
	if (block_id < 256 && block_id >= 0) {
		return block_properties[block_id];
	}
}

float MinecraftBlockHolder::getJavaBlockHardness(int block_id, int block_meta) {
	if (block_id < 256 && block_id >= 0) {
		return block_hardness[block_id];
	}
	return -1;
}
