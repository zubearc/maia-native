#pragma once

//std::map<unsigned char, unsigned char> block_properties;

#undef TRANSPARENT
namespace MinecraftBlockHolder {

	enum BlockProp : unsigned char {
		UNKNOWN = 0,
		AIR = 0b1,
		SOLID = 0b10,
		DESTROYABLE = 0b100,
		WATER = 0b1000,
		UNUSED = 0b10000,
		TRANSPARENT = 0b100000,
		AVOID = 0b1000000,
		ADJ_SOLID = 0b10000000
	};

	extern unsigned char* block_properties;
	extern float* block_hardness;

	void initBlockProperties();

}