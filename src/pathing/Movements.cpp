#include "Movement.h"

#pragma once

#include "../je/WorldProvider.h"
#include "../blocks/MinecraftBlocks.h"
#include "../blocks/MinecraftBlockHelper.h"

#include "Move.h"

using namespace MinecraftBlockHolder;

Movements::Movements() {
	this->digCost = 6;
	this->placeCost = 4;
	this->allowPlacing = false;
	this->allowBreaking = false;
}

void printBlockInfo(BlockRef ref) {
	auto blockProperties = ref.properties;
	printf("(%d %d %d) %d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d, Avoid=%d, Water=%d\n", ref.x, ref.y, ref.z, ref.blockId, (blockProperties & BlockProp::AIR) > 0, (blockProperties & BlockProp::SOLID) > 0,
		(blockProperties & BlockProp::DESTROYABLE) > 0, (blockProperties & BlockProp::TRANSPARENT) > 0, (blockProperties & BlockProp::AVOID) > 0, (blockProperties & BlockProp::WATER) > 0);
}

BlockRef Movements::getBlock(int x, int y, int z, int dx, int dy, int dz) {
	auto X = x + dx;
	auto Y = y + dy;
	auto Z = z + dz;
	auto blockProperties = WorldProvider::getBlockProperties(X, Y, Z);

	short blockId = 0;
	short blockMetadata = 0;

	if (!WorldProvider::getBlock(X, Y, Z, blockId, blockMetadata)) {
		return {}; // Flags would be marked unknown
	}

	return { blockProperties, x + dx, y + dy, z + dz, blockId, blockMetadata };
}

BlockRef Movements::getBlock(BlockRef& node, int dx, int dy, int dz) {
	return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
}

BlockRef Movements::getBlock(Move& node, int dx, int dy, int dz) {
	return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
}


// Tells us if we can stand on the block
#define IS_SOLID(BLOCK) (BLOCK & BlockProp::SOLID)
// Tells us if we can walk through the block
#define IS_TRANSPARENT(BLOCK) (BLOCK & BlockProp::TRANSPARENT)
// Tells us if if water
#define IS_LIQUID(BLOCK) (BLOCK & BlockProp::WATER) //<-- this should probably be IS_WATER! Currently flowing lava is ignored in safeToBreak.
// Tells us if we can break the block
#define IS_BREAKABLE(BLOCK) (BLOCK & BlockProp::DESTROYABLE)
// Tell us if we can place if the block is air or is transparent, breakable and we can place ontop of it
#define IS_PLACABLE(BLOCK) (BLOCK & (BlockProp::AIR) || BLOCK & (BlockProp::TRANSPARENT | BlockProp::DESTROYABLE))
#define IS_PASSABLE(BLOCK) (BLOCK & (BlockProp::AIR | BlockProp::TRANSPARENT))
#define IS_AVOID(BLOCK) (BLOCK & (BlockProp::AVOID))
#define IS_DENY(BLOCK) (BLOCK == BlockProp::AVOID)
// IS_DENY is unequivocal "we can't be near this block."
// Some blocks are AVOID, but are BREAKABLE or TRANSPARENT (placable onto) so they can become non-avoid
// blocks.

inline void Movements::addCostModifierForBlock(BlockRef& block, float& cost) {
	if (IS_LIQUID(block.properties)) {
		cost += 2;
	}
}

bool Movements::safeToBreak(BlockRef& block) {
	if (this->dontCreateFlow) {
		// false if next to liquid
		// TODO: Make this support lava.
		if (IS_LIQUID(this->getBlock(block, 0, 1, 0).properties)) return false;
		if (IS_LIQUID(this->getBlock(block, -1, 0, 0).properties)) return false;
		if (IS_LIQUID(this->getBlock(block, 1, 0, 0).properties)) return false;
		if (IS_LIQUID(this->getBlock(block, 0, 0, -1).properties)) return false;
		if (IS_LIQUID(this->getBlock(block, 0, 0, 1).properties)) return false;
	}
	return IS_BREAKABLE(block.properties);
}

// Called when we may have to break (assumed non-solid) block and replace with a block 
float Movements::breakForReplace(BlockRef& block, BlockRef* toBreak, char& numToBreak) {
	auto blockProperties = block.properties;

	// If block is a transparent block, it must be broken & replaced (if possible) with a solid block.
	// maybe it's a sign or a torch. some transparent blocks we don't want to destory like rails.
	// If we have a DENY block (eg Lava) and it's not marked as SOLID and it's TRANSPARENT, we can place ontop of it.
	if (IS_TRANSPARENT(block.properties) && !IS_AVOID(block.properties)) {
		if (!this->allowBreaking) return 100;

		if (IS_BREAKABLE(block.properties)) {
			// TODO: This should getBlockDig cost, not do a simple hardness check
			auto digTime = MinecraftBlockHolder::getJavaBlockHardness(block.blockId, block.blockMetadata);

			// return (1 + 3 * digTime / 1000) * this.digCost
			if ((numToBreak + 1) > MAX_BLOCK_BREAKS_PER_MOVE) {
				return 100; // We can only break up to 3 blocks per move
			}
			toBreak[numToBreak++] = block;

			return digTime + this->digCost;
		}

		return 100;
	}
	return 0;
}

// Called when we want to break a block
float Movements::safeOrBreak(BlockRef& block, BlockRef* toBreak, char& numToBreak) {
	// Lava blocks are PASSABLE (so can place ontop of) but also AVOID so we can't pass through them. 
	// They're not BREAKABLE though so this returns 100.
	if (IS_PASSABLE(block.properties) && !IS_AVOID(block.properties)) return 0;
	if (!this->allowBreaking) return 100;
	if (!this->safeToBreak(block)) return 100; // Can't break, so can't move

	if ((numToBreak + 1) > MAX_BLOCK_BREAKS_PER_MOVE) {
		return 100; // We can only break up to 3 blocks per move
	}

	toBreak[numToBreak++] = block;

	auto digTime = MinecraftBlockHolder::getJavaBlockHardness(block.blockId, block.blockMetadata);

	// return (1 + 3 * digTime / 1000) * this.digCost
	//TODO: Figure out a good way to do this in native
	return digTime + this->digCost;
}

void Movements::getMoveJumpUp(Move& block, char dx, char dz, std::vector<Move>& neighbors) {
	auto blockA = this->getBlock(block, 0, 2, 0);
	auto blockH = this->getBlock(block, dx, 2, dz);
	auto blockB = this->getBlock(block, dx, 1, dz);
	auto blockC = this->getBlock(block, dx, 0, dz);

	Move move;


	move.cost = 2.0f; // move cost (move+jump)
	move.numToBreak = 0;
	move.numToPlace = 0;

	move.cost += this->safeOrBreak(blockA, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockH, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;

	// Don't jump above AVOID blocks
	if (IS_AVOID(blockC.properties)) {
		// If we can break & replace this block, try it
		if (IS_BREAKABLE(blockC.properties)) {
			move.cost += breakForReplace(blockC, move.toBreak, move.numToBreak);
			if (move.cost > 100) return;

			auto blockUnderC = this->getBlock(blockC, 0, -1, 0);

			if (!IS_SOLID(blockUnderC.properties)) {
				// Make sure it's air because we're not going to bother breaking more than 3 blocks
				if (!(blockUnderC.properties & BlockProp::AIR)) {
					return;
				}
				move.toPlace[move.numToPlace++] = BlockRef{ blockUnderC.properties, blockUnderC.x, blockUnderC.y, blockUnderC.z, blockUnderC.blockId, blockUnderC.blockMetadata, -dx, 0, -dz };
				move.cost += this->placeCost;
			}

			move.toPlace[move.numToPlace++] = BlockRef{ blockC.properties, blockC.x, blockC.y, blockC.z, blockC.blockId, blockC.blockMetadata, -dx, 0, -dz };
			move.cost += this->placeCost; // additional cost for placing a block
		}
		else {
			return;
		}
	} else if (!IS_SOLID(blockC.properties)) {
		if (!this->allowPlacing) return;

		move.cost += breakForReplace(blockC, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;

		auto blockD = this->getBlock(block, dx, -1, dz);

		if (!IS_SOLID(blockD.properties)) {
			move.cost += breakForReplace(blockD, move.toBreak, move.numToBreak);
			if (move.cost > 100) return;

			move.toPlace[move.numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, blockD.blockId, blockD.blockMetadata, -dx, 0, -dz };
			move.cost += this->placeCost; // additional cost for placing a block
		}

		move.toPlace[move.numToPlace++] = BlockRef{ blockC.properties, blockC.x, blockC.y, blockC.z, blockC.blockId, blockC.blockMetadata, 0, -1, 0 };
		move.cost += this->placeCost; // additional cost for placing a block
	}

	// TODO: some of this code is left over from debug code, remove once this is stable.
	move.x = blockB.x;
	move.y = blockB.y;
	move.z = blockB.z;
	move.blockId = block.x;
	move.blockMetadata = block.z;
	move.remainingBlocks = (short)blockA.blockId;
	move.props = blockB.properties;
	neighbors.push_back(move);
}

void Movements::getMoveCardinal(Move& block, char dx, char dz, std::vector<Move>& neighbors) {
	auto blockB = this->getBlock(block, dx, 1, dz);
	auto blockC = this->getBlock(block, dx, 0, dz);
	auto blockD = this->getBlock(block, dx, -1, dz);

	if (IS_DENY(blockB.properties) || IS_DENY(blockC.properties) || IS_DENY(blockD.properties)) {
		return;
	}

	Move move;

	move.cost = 1.0f; // move cost
	move.numToBreak = 0;
	move.numToPlace = 0;

	if (!IS_SOLID(blockD.properties) && !IS_LIQUID(blockC.properties)) { //isn't solid & isn't water
		if (!this->allowPlacing) return;
		//printf("Non-solid next block\n");
		///printBlockInfo(blockD);
		move.cost += breakForReplace(blockD, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;
		//printf("Able to place\n");

		/*if (IS_TRANSPARENT(blockD.properties) && !IS_AVOID(blockD.properties)) {
			auto replaceCost = getBreakCost(blockD);
			if (replaceCost == 100) {
				return; // We can't place a block here
			} else {
				move.cost += replaceCost;
				move.toBreak[move.numToBreak++] = blockD;
			}
		}*/


		//move.cost += safeOrBreak(blockD, move.toBreak, move.numToBreak);

		move.toPlace[move.numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, blockD.blockId, blockD.blockMetadata, -dx, 0, -dz };

		move.cost += this->placeCost; // additional cost for placing a block
	}

	move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockC, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;

	addCostModifierForBlock(blockC, move.cost);

	move.x = blockC.x;
	move.y = blockC.y;
	move.z = blockC.z;
	move.blockId = blockC.blockId;
	move.blockMetadata = blockC.blockMetadata;
	move.remainingBlocks = 208;
	move.props = blockC.properties;
	neighbors.push_back(move);
}

void Movements::getMoveDiagonal(Move& block, int dx, int dz, std::vector<Move>& neighbors) {
	auto blockB = this->getBlock(block, dx, 1, dz);
	auto blockB1 = this->getBlock(block, 0, 1, dz);
	auto blockB2 = this->getBlock(block, dx, 1, 0);

	auto blockC = this->getBlock(block, dx, 0, dz);
	auto blockC1 = this->getBlock(block, 0, 0, dz);
	auto blockC2 = this->getBlock(block, dx, 0, 0);

	auto blockD = this->getBlock(block, dx, -1, dz);

	Move move;

	move.cost = 1.4142f; // move cost
	move.numToBreak = 0;

	if (!IS_SOLID(blockD.properties) && !IS_LIQUID(blockC.properties)) return; // we don't place blocks in diagonal

	// From https://github.com/Karang/mineflayer-pathfinder -->
	// There's way too many breaks here, no?
	// In Minecraft you can't truly go diagonal through a wall, you have to break
	// at a cardinal direction first, then break the blocks at the end location
	// Taking 4 breaks, not 6 as done here. This would probably be done better
	// in multiple cardinal moves. Due to high cost this probably isn't used in the end
	// path anyway, so it doesn't really matter :]
	// Because our Move struct may only account for 3 breaks
	// for memory efficiency purposes, this is automatically aborted in safeOrBreak once
	// we reach 3 blocks to break.

	// HOTFIX: Because of the way we compare nodes, breaking blocks here leads to improper behavior.
	// Because we compare node equality soley based on XYZ coordinates, the broken and placed blocks
	// may change depending on what the parent node to this move is. So block places and breaks not
	// directly under or above the node we are adding (restricted to Y axes) can be wrong if the parents
	// are not equal.

	if (!IS_PASSABLE(blockB1.properties) || !IS_PASSABLE(blockB2.properties) || !IS_PASSABLE(blockC1.properties) ||
		!IS_PASSABLE(blockC2.properties) || !IS_PASSABLE(blockC.properties) || !IS_PASSABLE(blockB.properties) ||
		IS_AVOID(blockB1.properties) || IS_AVOID(blockB2.properties) || IS_AVOID(blockC1.properties) ||
		IS_AVOID(blockC2.properties) || IS_AVOID(blockC.properties) || IS_AVOID(blockB.properties)
		) {
		return;
	}

	/*move.cost += this->safeOrBreak(blockB1, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockB2, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockC1, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockC2, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;

	move.cost += this->safeOrBreak(blockC, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;*/

	addCostModifierForBlock(blockC, move.cost);

	move.x = blockC.x;
	move.y = blockC.y;
	move.z = blockC.z;
	move.blockId = blockC.blockId;
	move.blockMetadata = blockC.blockMetadata;
	move.remainingBlocks = 330;
	move.props = blockC.properties;
	neighbors.push_back(move);
}

BlockRef Movements::getLandingBlock(Move& block, int dx, int dz) {
	auto blockLand = this->getBlock(block, dx, -2, dz);
	auto lowestDdd = this->maxDropDown - 1;
	if (allowLongDistanceDropsToWater) {
		for (int y = block.y - 2, i = 0; y > 2; y--, i++) {
			if (blockLand.properties & (BlockProp::SOLID | BlockProp::WATER | BlockProp::AVOID) || !IS_PASSABLE(blockLand.properties)) {
				if (i > lowestDdd) {
					if (IS_LIQUID(blockLand.properties)) {
						return blockLand;
					} else {
						return this->getBlock(block, dx, -2, dz);
					}
				}
				break;
			}

			blockLand = this->getBlock(block, dx, -2 - i, dz);
		}
	} else {
		for (auto i = 0; i < this->maxDropDown - 1; i++) {
			if (blockLand.properties & (BlockProp::SOLID | BlockProp::WATER | BlockProp::AVOID) || !IS_PASSABLE(blockLand.properties)) break;
			blockLand = this->getBlock(block, dx, -2 - i, dz);
		}
	}
	return blockLand;
}

void Movements::getMoveDropDown(Move& block, char dx, char dz, std::vector<Move>& neighbors) {
	auto blockB = this->getBlock(block, dx, 1, dz);
	auto blockC = this->getBlock(block, dx, 0, dz);
	auto blockD = this->getBlock(block, dx, -1, dz);

	Move move;

	move.cost = 1.0f; // move cost

	move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockC, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;
	move.cost += this->safeOrBreak(blockD, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;


	auto blockLand = this->getLandingBlock(block, dx, dz);


	if (IS_AVOID(blockLand.properties)) { // Dangerious block. Don't even think aout it (or should we? if we can reach block, maybe replace it)
		return;
	} else if (!IS_SOLID(blockLand.properties) && !IS_LIQUID(blockLand.properties)) {
		if (!this->allowPlacing) return;

		auto blockC = this->getBlock(blockLand, 0, -1, 0); // Block underneath
		if (IS_SOLID(blockC.properties) && false) {
			//???
			// blockLand is unreachable by only 1 block, place block on blockLand to make it reachable
			//move.cost += breakForReplace(blockLand, move.toBreak, move.numToBreak);
			//if (move.cost > 100) return;

			//move.toPlace[move.numToPlace++] = { blockLand.properties, blockLand.x, blockLand.y, blockLand.z, blockLand.blockId, blockLand.blockMetadata,  0, -1, 0 };
			//move.cost += this->placeCost; // additional cost for placing a block
		} else {

			// Code below checks to make sure we have a reference block before we try to place somewhere below
#define GET_ADJACENT_BLOCK(Name, DX, DZ) \
				if (move.numToPlace == 0) { \
					BlockRef block##Name = this->getBlock(blockLand, DX, 0, DZ); /* Block adjacent */ \
					if (IS_SOLID(block##Name.properties)) { \
						move.cost += breakForReplace(blockLand, move.toBreak, move.numToBreak); \
						if (move.cost > 100) return; \
						move.toPlace[move.numToPlace++] = { blockLand.properties, blockLand.x, blockLand.y, blockLand.z, blockLand.blockId, blockLand.blockMetadata, DX, 0, DZ }; \
						move.cost += this->placeCost; /* additional cost for placing a block */ \
					} \
				}

			GET_ADJACENT_BLOCK(North, -1, 0);
			GET_ADJACENT_BLOCK(South, 1, 0);
			GET_ADJACENT_BLOCK(East, 0, -1);
			GET_ADJACENT_BLOCK(West, 0, 1);
		}


		// If we didn't place anything & block below player's next move is passable,
		// abort this operation.
		if (IS_PASSABLE(blockLand.properties) && move.numToPlace == 0) {
			return;
		}
	}

	addCostModifierForBlock(blockC, move.cost);

	if (IS_LIQUID(blockC.properties)) {
		move.cost += 2; // We are underwater--do not go any lower
	}

	//printf("%d %d %d -> Landing on: with max block drop distance %d:\n", move.x, move.y, move.z, this->maxDropDown);
	//printBlockInfo(blockLand);


	move.x = blockLand.x;
	move.y = IS_LIQUID(blockLand.properties) ? blockLand.y : blockLand.y + 1;
	move.z = blockLand.z;
	move.blockId = blockLand.blockId;
	move.blockMetadata = blockLand.blockMetadata;
	move.remainingBlocks = 335;
	move.props = blockLand.properties;
	neighbors.push_back(move);
}

void Movements::getMoveDown(Move& block, std::vector<Move>& neighbors) {
	auto block0 = this->getBlock(block, 0, -1, 0);

	Move move;

	move.cost = 1.0f; // move cost
	move.numToBreak = 0;
	move.numToPlace = 0;

	auto blockLand = this->getLandingBlock(block, 0, 0);
	if (!IS_SOLID(blockLand.properties)) return;

	move.cost += this->safeOrBreak(block0, move.toBreak, move.numToBreak);
	if (move.cost > 100) return;


	move.x = blockLand.x;
	move.y = blockLand.y;
	move.z = blockLand.z;
	move.blockId = blockLand.blockId;
	move.blockMetadata = blockLand.blockMetadata;
	move.remainingBlocks = 368;
	move.props = blockLand.properties;
	neighbors.push_back(move);

}


// for each cardinal direction:
// "." is head. "+" is feet and current location.
// "#" is initial floor which is always solid. "a"-"u" are blocks to check
//
//   --0123-- horizontalOffset
//  |
// +2  aho
// +1  .bip
//  0  +cjq
// -1  #dkr
// -2   els
// -3   fmt
// -4   gn
//  |
//  dy

std::vector<Move> Movements::getNeighbors(Move& node) {
	std::vector<Move> neighbors;

	// Simple moves in 4 cardinal points

#define MOVES(Direction, DX, DZ) \
	this->getMoveCardinal(node, DX, DZ, neighbors); \
	this->getMoveJumpUp(node, DX, DZ, neighbors); \
	this->getMoveDropDown(node, DX, DZ, neighbors);

	MOVES(North, 0, -1);
	MOVES(South, 0, 1);
	MOVES(East, 1, 0);
	MOVES(West, -1, 0);

	// if (this.allowParkour) {
	//     this.getMoveParkourForward(node, dir, neighbors)
	// }

	// Diagonals
	this->getMoveDiagonal(node, -1, -1, neighbors); //NE
	this->getMoveDiagonal(node, -1, 1, neighbors); //NW
	this->getMoveDiagonal(node, 1, -1, neighbors); //Se
	this->getMoveDiagonal(node, 1, 1, neighbors); //Sw

	// this.getMoveDown(node, neighbors)

	// if (this.allow1by1towers) {
	//     this.getMoveUp(node, neighbors)
	// }


	return neighbors;
}