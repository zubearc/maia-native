#pragma once

#include <map>

typedef char BlockProps;

namespace MinecraftBlockHolder {
	std::map<short, BlockProps> java_block_id_to_blockprops_table;

	// Minecraft 1.12 Numerical ID
	//static void GetBlockFromJavaNID();

	// Minecraft 1.13 String ID
	//static void GetBlockFromJavaSID();

	//BlockProps getJavaBlockProperties(int block_id, int block_meta);
}