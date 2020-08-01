#pragma once

#include <vector>
#include "Move.h"

class Movements {
public:
	int digCost = 6;
	int placeCost = 4;
	int maxDropDown = 3;
	bool allowLongDistanceDropsToWater = true;
	bool allowPlacing = false;
	bool allowBreaking = false;

	bool dontCreateFlow = true;

	Movements();

	BlockRef getBlock(int x, int y, int z, int dx, int dy, int dz);

	BlockRef getBlock(BlockRef& node, int dx, int dy, int dz);

	BlockRef getBlock(Move& node, int dx, int dy, int dz);

	inline void addCostModifierForBlock(BlockRef& block, float& cost);

	bool safeToBreak(BlockRef& block);

	// Called when we may have to break (assumed non-solid) block and replace with a block 
	float breakForReplace(BlockRef& block, BlockRef* toBreak, char& numToBreak);

	// Called when we want to break a block
	float safeOrBreak(BlockRef& block, BlockRef* toBreak, char& numToBreak);

	void getMoveJumpUp(Move& block, char dx, char dz, std::vector<Move>& neighbors);

	void getMoveCardinal(Move& block, char dx, char dz, std::vector<Move>& neighbors);

	void getMoveDiagonal(Move& block, int dx, int dz, std::vector<Move>& neighbors);

	BlockRef getLandingBlock(Move& block, int dx, int dz);

	void getMoveDropDown(Move& block, char dx, char dz, std::vector<Move>& neighbors);

	void getMoveDown(Move& block, std::vector<Move>& neighbors);

	std::vector<Move> getNeighbors(Move& node);
};