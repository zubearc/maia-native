#pragma once

#include <map>
#include "MinecraftBlocks.h"
#include "../Utils.h"

namespace MinecraftBlockHolder {
	//std::map<short, BlockProps> java_block_id_to_blockprops_table;

	// Minecraft 1.12 Numerical ID
	//static void GetBlockFromJavaNID();

	// Minecraft 1.13 String ID
	//static void GetBlockFromJavaSID();

	void init();

	BlockProps getJavaBlockProperties(int block_id, int block_meta);
	float getJavaBlockHardness(int block_id, int block_meta);
}