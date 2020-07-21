#pragma once

// Andromeda

class JChunkColumn {
	virtual inline int getX() = 0;

	virtual inline int getZ() = 0;

	virtual unsigned char getBlockId(int x, int y, int z) = 0;

	virtual void setBlockId(int x, int y, int z, unsigned char blockId) = 0;

	virtual char getBlockMetadata(int x, int y, int z) = 0;

	virtual bool setBlockMetadata(int x, int y, int z, char blockData) = 0;

	virtual char getBlockLight(int x, int y, int z) = 0;

	virtual bool setBlockLight(int x, int y, int z, char lightLevel) = 0;

	virtual char getSkyLight(int x, int y, int z) = 0;

	virtual bool setSkyLight(int x, int y, int z, char lightLevel) = 0;

	virtual unsigned char getBiome(int x, int z) = 0;

	virtual void setBiome(int x, int z, unsigned char biome) = 0;
};

class JChunkSection {
public:
	virtual inline unsigned char getY() = 0;

	virtual const bool isPaletted() = 0;

	//inline virtual unsigned char getBlockId(char x, char y, char z);
	//inline virtual void setBlockId(char x, char y, char z, short blockId);

	/*inline virtual char getBlockMetadata(char x, char y, char z);
	inline virtual void setBlockMetadata(char x, char y, char z, char metadata);
	inline virtual void setBlockState(byte x, byte y, byte z, PalleteMappedBlockState blockState);
	inline virtual PalleteMappedBlockState getBlockState(byte x, byte y, byte z);*/


	// LIGHTING FUNCTIONS

	inline virtual char getBlockLight(char x, char y, char z) = 0;

	inline virtual void setBlockLight(char x, char y, char z, char blockLight) = 0;

	inline virtual char getSkyLight(char x, char y, char z) = 0;

	inline virtual void setSkyLight(char x, char y, char z, char blockLight) = 0;

	// EXPORT FUNCTIONS

	//virtual nbt::tag_compound toLegacyNBT() = 0;
	//virtual std::string toLegacyBuffer() = 0;

	//virtual nbt::tag_compound toPalettedNBT() { return nbt::tag_compound{}; };
	//virtual void toLocallyPalettedBinary(std::ostringstream& stream) = 0;

	virtual ~JChunkSection() = default;
};