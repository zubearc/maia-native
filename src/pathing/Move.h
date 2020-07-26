#pragma once

struct BlockRef {
	short properties;

	int x;
	int y;
	int z;

	short blockId;
	short blockMetadata;

	char ax;
	char ay;
	char az;
};


#define MAX_BLOCK_BREAKS_PER_MOVE 3
#define MAX_BLOCK_PLACES_PER_MOVE 3

struct Move {
	int x;
	int y;
	int z;

	short blockId;
	short blockMetadata;
	short remainingBlocks;
	float cost;
	short props;

	BlockRef toBreak[MAX_BLOCK_BREAKS_PER_MOVE];
	BlockRef toPlace[MAX_BLOCK_PLACES_PER_MOVE];

	char numToBreak = 0;
	char numToPlace = 0;

	//cant use vector because A* lib uses memcpy
	//c arrays are faster anyways, no big deal.
	//std::vector<BlockRef> toBreak;
	//std::vector<BlockRef> toPlace;

	bool jumps;
};