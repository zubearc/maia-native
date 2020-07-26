#pragma once

#include "../je/WorldProvider.h"
#include "../blocks/MinecraftBlocks.h"
#include "../blocks/MinecraftBlockHelper.h"

#include "Move.h"

using namespace MinecraftBlockHolder;

class Movements {
public:
	int digCost = 6;
	int placeCost = 4;
	int maxDropDown = 3;
	bool allowLongDistanceDropsToWater = true;
	bool allowPlacing = false;
	bool allowBreaking = false;

	bool dontCreateFlow = true;

	Movements() {
		//this.graph = graph;

		this->digCost = 6;
		this->placeCost = 4;
		// this.dontCreateFlow = true
		// this.allow1by1towers = true
		// this.allowFreeMotion = false
		// this.allowParkour = true

		this->allowPlacing = false;
		this->allowBreaking = false;

		// this.maxDropDown = 4
	}

	// countScaffoldingItems() {
	//     let count = 0
	//     const items = this.bot.inventory.items()
	//     for (const id of this.scafoldingBlocks) {
	//         for (const j in items) {
	//             const item = items[j]
	//             if (item.type === id) count += item.count
	//         }
	//     }
	//     return count
	// }

	// getScaffoldingItem() {
	//     const items = this.bot.inventory.items()
	//     for (const id of this.scafoldingBlocks) {
	//         for (const j in items) {
	//             const item = items[j]
	//             if (item.type === id) return item
	//         }
	//     }
	//     return null
	// }

	void printBlockInfo(BlockRef ref) {
		auto blockProperties = ref.properties;
		printf("(%d %d %d) %d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d, Avoid=%d, Water=%d\n", ref.x,ref.y,ref.z, ref.blockId, (blockProperties & BlockProp::AIR) > 0, (blockProperties & BlockProp::SOLID) > 0,
			(blockProperties & BlockProp::DESTROYABLE) > 0, (blockProperties & BlockProp::TRANSPARENT) > 0, (blockProperties & BlockProp::AVOID) > 0, (blockProperties & BlockProp::WATER) > 0);
	}



	BlockRef getBlock(int x, int y, int z, int dx, int dy, int dz) {
		auto X = x + dx;
		auto Y = y + dy;
		auto Z = z + dz;
		auto blockProperties = WorldProvider::getBlockProperties(X, Y, Z);
		
		short blockId = 0;
		short blockMetadata = 0;
		
		if (!WorldProvider::getBlock(X, Y, Z, blockId, blockMetadata)) {
			return {}; // Flags would be marked unknown
		}

		//printf("(%d %d %d) %d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d\n", X,Y,Z, blockId, (blockProperties & BlockProp::AIR) > 0, (blockProperties & BlockProp::SOLID) > 0,
		//	(blockProperties & BlockProp::DESTROYABLE) > 0, (blockProperties & BlockProp::TRANSPARENT) > 0);


		return { blockProperties, x + dx, y + dy, z + dz, blockId, blockMetadata };
	}

	BlockRef getBlock(BlockRef &node, int dx, int dy, int dz) {
		return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
	}

	BlockRef getBlock(Move &node, int dx, int dy, int dz) {
		return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
	}


// Tells us if we can stand on the block
#define IS_SOLID(BLOCK) (BLOCK & BlockProp::SOLID)
// Tells us if we can walk through the block
#define IS_TRANSPARENT(BLOCK) (BLOCK & BlockProp::TRANSPARENT)
// Tells us if if water
#define IS_LIQUID(BLOCK) (BLOCK & BlockProp::WATER)
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

	inline void addCostModifierForBlock(BlockRef& block, float& cost) {
		if (IS_LIQUID(block.properties)) {
			cost += 2;
		}
	}

	bool safeToBreak(BlockRef &block) {
		if (this->dontCreateFlow) {
			// false if next to liquid
			if (IS_LIQUID(this->getBlock(block, 0, 1, 0).properties)) return false;
			if (IS_LIQUID(this->getBlock(block, -1, 0, 0).properties)) return false;
			if (IS_LIQUID(this->getBlock(block, 1, 0, 0).properties)) return false;
			if (IS_LIQUID(this->getBlock(block, 0, 0, -1).properties)) return false;
			if (IS_LIQUID(this->getBlock(block, 0, 0, 1).properties)) return false;
		}
		return IS_BREAKABLE(block.properties);
		// TODO: break exclusion areas
	}

	// Called when we may have to break (assumed non-solid) block and replace with a block 
	float breakForReplace(BlockRef &block, BlockRef* toBreak, char& numToBreak) {
		auto blockProperties = block.properties;
		//printf("%d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d\n", block.blockId, (blockProperties & BlockProp::AIR) > 0, (blockProperties & BlockProp::SOLID) > 0,
		//	(blockProperties & BlockProp::DESTROYABLE) > 0, (blockProperties & BlockProp::TRANSPARENT) > 0);

		
		// If blockD is a transparent block, it must be broken & replaced (if possible) with a solid block.
		// maybe it's a sign or a torch. some transparent blocks we don't want to destory like rails.
		// If we have a DENY block (eg Lava) and it's not marked as SOLID and it's TRANSPARENT, we can place ontop of it.
		if (IS_TRANSPARENT(block.properties) && !IS_AVOID(block.properties)) {
			if (!this->allowBreaking) return 100;

			if (IS_BREAKABLE(block.properties)) {
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
	float safeOrBreak(BlockRef block, BlockRef *toBreak, char &numToBreak) {
		// Lava blocks are PASSABLE (so can place ontop of) but also AVOID so we can't pass through them. 
		// They're not BREAKABLE though so this returns 100.
		if (IS_PASSABLE(block.properties) && !IS_AVOID(block.properties)) return 0;
		if (!this->allowBreaking) return 100;
		if (!this->safeToBreak(block)) return 100; // Can't break, so can't move

		if ((numToBreak + 1) > MAX_BLOCK_BREAKS_PER_MOVE) {
			return 100; // We can only break up to 3 blocks per move
		}
		
		toBreak[numToBreak++] = block;

			// const tool = this.bot.pathfinder.bestHarvestTool(block)
			// const enchants = (tool && tool.nbt) ? nbt.simplify(tool.nbt).Enchantments : []
			// const effects = this.bot.entity.effects
			// const digTime = block.digTime(tool ? tool.type : null, false, false, false, enchants, effects)

		auto digTime = MinecraftBlockHolder::getJavaBlockHardness(block.blockId, block.blockMetadata);

		// return (1 + 3 * digTime / 1000) * this.digCost
		return digTime + this->digCost;
	}

	void getMoveJumpUp(Move block, char dx, char dz, std::vector<Move> &neighbors) {
		auto blockA = this->getBlock(block, 0, 2, 0);
		auto blockH = this->getBlock(block, dx, 2, dz);
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockC = this->getBlock(block, dx, 0, dz);

		Move move;

		move.cost = 2.0f; // move cost (move+jump)
		//BlockRef toBreak[3];
		//BlockRef toPlace[3];
		move.numToBreak = 0;
		move.numToPlace = 0;

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
					move.toPlace[move.numToPlace++] = BlockRef{ blockUnderC.properties, blockUnderC.x, blockUnderC.y, blockUnderC.z, -dx, 0, -dz };
					move.cost += this->placeCost;
				}

				move.toPlace[move.numToPlace++] = BlockRef{ blockC.properties, blockC.x, blockC.y, blockC.z, -dx, 0, -dz };
				move.cost += this->placeCost; // additional cost for placing a block
			} else {
				return;
			}
		} else if (!IS_SOLID(blockC.properties)) {
			if (!this->allowPlacing) return;
				// if (node.remainingBlocks === 0) return // not enough blocks to place

			move.cost += breakForReplace(blockC, move.toBreak, move.numToBreak);
			if (move.cost > 100) return;

			// TODO: avoid entities as part of placing blocks
			auto blockD = this->getBlock(block, dx, -1, dz);
						
			if (!IS_SOLID(blockD.properties)) {
				// if (node.remainingBlocks === 1) return // not enough blocks to place
				// toPlace.push({ x: node.x, y: node.y - 1, z: node.z, dx: dir.x, dy: 0, dz: dir.z })
				
				move.cost += breakForReplace(blockD, move.toBreak, move.numToBreak);
				if (move.cost > 100) return;
				
				move.toPlace[move.numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, -dx, 0, -dz };
				move.cost += this->placeCost; // additional cost for placing a block
			}

				// toPlace.push({ x: node.x + dir.x, y: node.y - 1, z: node.z + dir.z, dx: 0, dy: 1, dz: 0 })
			move.toPlace[move.numToPlace++] = BlockRef{ blockC.properties, blockC.x, blockC.y, blockC.z, dx, -1, -dz };
			move.cost += this->placeCost; // additional cost for placing a block
		}

		move.cost += this->safeOrBreak(blockA, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;
		move.cost += this->safeOrBreak(blockH, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;
		move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;

		move.x = blockB.x; 
		move.y = blockB.y; 
		move.z = blockB.z;
		move.blockId = blockB.blockId;
		move.blockMetadata = blockB.blockMetadata;
		move.remainingBlocks = 169;
		move.props = blockB.properties;
		neighbors.push_back(move);
		//neighbors.push_back(Move{ blockB.x, blockB.y, blockB.z, blockB.blockId, blockB.blockMetadata, 
		//	99, cost, blockB.properties, /*toBreak, toPlace*/ });
	}

	void getMoveCardinal(Move block, char dx, char dz, std::vector<Move> &neighbors) {
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

		//printf("GetCardinal %d %d %d %d, %d\n", dx, dz, neighbors.size(), blockD.properties, blockC.properties);
		//printf("Props %d %d\n", IS_SOLID(blockD.properties), IS_LIQUID(blockC.properties));

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

			move.toPlace[move.numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, -dx, 0, -dz };

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

	void getMoveDiagonal(Move block, int dx, int dz, std::vector<Move> &neighbors) {
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockB1 = this->getBlock(block, 0, 1, dz);
		auto blockB2 = this->getBlock(block, dx, 1, 0);

		auto blockC = this->getBlock(block, dx, 0, dz);
		auto blockC1 = this->getBlock(block, 0, 0, dz);
		auto blockC2 = this->getBlock(block, dx, 0, 0);

		auto blockD = this->getBlock(block, dx, -1, dz);

		Move move;

		move.cost = 1.4142f; // move cost
		//BlockRef toBreak[3];
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

		move.cost += this->safeOrBreak(blockB1, move.toBreak, move.numToBreak);
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
		if (move.cost > 100) return;

		addCostModifierForBlock(blockC, move.cost);

		move.x = blockC.x;
		move.y = blockC.y;
		move.z = blockC.z;
		move.blockId = blockC.blockId;
		move.blockMetadata = blockC.blockMetadata;
		move.remainingBlocks = 251;
		move.props = blockC.properties;
		neighbors.push_back(move);

		//neighbors.push(new Move(blockC.x, blockC.y, blockC.z, node.remainingBlocks, cost, toBreak))
		//neighbors.push_back(Move{ blockC.x, blockC.y, blockC.z, blockC.blockId, blockC.blockMetadata,
		//	99, cost, blockB.properties, /*toBreak*/ });
	}

	BlockRef getLandingBlock(Move block, int dx, int dz) {
		auto blockLand = this->getBlock(block, dx, -2, dz);
		if (allowLongDistanceDropsToWater) {
			for (int y = block.y - 2, i = 0; y > 2; y--, i++) {
				if (blockLand.properties & (BlockProp::SOLID | BlockProp::WATER | BlockProp::AVOID) || !IS_PASSABLE(blockLand.properties)) break;

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

	void getMoveDropDown(Move block, char dx, char dz, std::vector<Move> &neighbors) {
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockC = this->getBlock(block, dx, 0, dz);
		auto blockD = this->getBlock(block, dx, -1, dz);

		Move move;

		move.cost = 1.0f; // move cost
		//BlockRef toBreak[3];
		//BlockRef toPlace[3];
		//numToBreak = 0;
		//char numToPlace = 0;

		move.cost += this->safeOrBreak(blockB, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;
		move.cost += this->safeOrBreak(blockC, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;
		move.cost += this->safeOrBreak(blockD, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;


		auto blockLand = this->getLandingBlock(block, dx, dz);

		//auto deltaY = block.y - blockLand.y;
		//move.cost += 0.05 * deltaY; // Slight cost the farther we go down so A* can prefer shorter drops

		//printf("LandingBlock: %d %d %d -> %d, %d\n", blockLand.x, blockLand.y, blockLand.z, blockLand.blockId, IS_LIQUID(blockLand.properties));

		//printBlockInfo(blockLand);

		// if (!blockLand.solid) return // TODO: place? (bridging down)

		if (IS_AVOID(blockLand.properties)) { // Dangerious block. Don't even think aout it
			return;
		} else if (!IS_SOLID(blockLand.properties) && !IS_LIQUID(blockLand.properties)) {
			if (!this->allowPlacing) return;
				// if (node.remainingBlocks === 0) return // not enough blocks to place

			auto blockC = this->getBlock(blockLand, 0, -1, 0); // Block underneath
			if (IS_SOLID(blockC.properties)) {
				// blockLand is unreachable by only 1 block, place block on blockLand to make it reachable

				move.cost += breakForReplace(blockLand, move.toBreak, move.numToBreak);
				if (move.cost > 100) return;

				move.toPlace[move.numToPlace++] = { blockLand.properties, blockLand.x, blockLand.y, blockLand.z, 0, -1, 0 };
				move.cost += this->placeCost; // additional cost for placing a block
			} else {

				// Code below checks to make sure we have a reference block before we try to place somewhere below
#define GET_ADJACENT_BLOCK(Name, DX, DZ) \
				if (move.numToPlace == 0) { \
					BlockRef block##Name = this->getBlock(blockLand, DX, 0, DZ); /* Block adjacent */ \
					if (IS_SOLID(block##Name.properties)) { \
						move.cost += breakForReplace(blockLand, move.toBreak, move.numToBreak); \
						if (move.cost > 100) return; \
						move.toPlace[move.numToPlace++] = { blockLand.properties, blockLand.x, blockLand.y, blockLand.z, DX, 0, DZ }; \
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

		//printf("DD %d,%d,%d %d\n", blockLand.x, blockLand.y, blockLand.z, move.cost);

		move.x = blockLand.x;
		move.y = IS_LIQUID(blockLand.properties) ? blockLand.y : blockLand.y + 1;
		move.z = blockLand.z;
		move.blockId = blockLand.blockId;
		move.blockMetadata = blockLand.blockMetadata;
		move.remainingBlocks = 335;
		move.props = blockLand.properties;
		neighbors.push_back(move);

		//neighbors.push_back(Move{ blockLand.x, blockLand.y + 1, blockLand.z, node.remainingBlocks - toPlace.length, cost, toBreak, toPlace))
		//neighbors.push_back(Move{ blockLand.x, blockLand.y + 1, blockLand.z, blockLand.blockId, blockLand.blockMetadata,
		//	99, cost, blockLand.properties, /*toBreak, toPlace*/ });
	}

	void getMoveDown(Move &block, std::vector<Move> &neighbors) {
		auto block0 = this->getBlock(block, 0, -1, 0);

		Move move;

		move.cost = 1.0f; // move cost
		//BlockRef toBreak[3];
		//BlockRef toPlace[3];
		move.numToBreak = 0;
		move.numToPlace = 0;

		auto blockLand = this->getLandingBlock(block, 0, 0);
		if (!IS_SOLID(blockLand.properties)) return;

		move.cost += this->safeOrBreak(block0, move.toBreak, move.numToBreak);
		if (move.cost > 100) return;

		//neighbors.push(new Move(blockLand.x, blockLand.y + 1, blockLand.z, node.remainingBlocks - toPlace.length, cost, toBreak, toPlace))

		move.x = blockLand.x;
		move.y = blockLand.y;
		move.z = blockLand.z;
		move.blockId = blockLand.blockId;
		move.blockMetadata = blockLand.blockMetadata;
		move.remainingBlocks = 368;
		move.props = blockLand.properties;
		neighbors.push_back(move);


//		neighbors.push_back(Move{ blockLand.x, blockLand.y, blockLand.z, blockLand.blockId, blockLand.blockMetadata,
//	99, cost, blockLand.properties, /*toBreak, toPlace*/ });
	}

	/*void getMoveUp(node, neighbors) {
		const block2 = this._getBlock(node, 0, 2, 0)
			let cost = 1 // move cost
			const toBreak = []
			const toPlace = []
			cost += this.safeOrBreak(block2, toBreak)
			if (cost > 100) return

				if (!this.allowPlacing) return
					// if (node.remainingBlocks === 0) return // not enough blocks to place
					toPlace.push({ x: node.x, y : node.y - 1, z : node.z, dx : 0, dy : 1, dz : 0, jump : true })
					toPlace.push({ x: node.x, y : node.y - 1, z : node.z, dx : 0, dy : 1, dz : 0, jump : true })

					cost += this.placeCost // additional cost for placing a block

					neighbors.push(new Move(node.x, node.y + 1, node.z, node.remainingBlocks - toPlace.length, cost, toBreak, toPlace))
	}

	// Jump up, down or forward over a 1 block gap
	getMoveParkourForward(node, dir, neighbors) {
		if (this.getBlock(node, dir.x, -1, dir.z).physical ||
			!this.getBlock(node, dir.x, 0, dir.z).safe ||
			!this.getBlock(node, dir.x, 1, dir.z).safe) return

			const dx = dir.x * 2
			const dz = dir.z * 2
			const blockB = this.getBlock(node, dx, 1, dz)
			const blockC = this.getBlock(node, dx, 0, dz)
			const blockD = this.getBlock(node, dx, -1, dz)

			if (blockB.safe && blockC.safe && blockD.physical) {
				// Forward
				if (!this.getBlock(node, 0, 2, 0).safe ||
					!this.getBlock(node, dir.x, 2, dir.z).safe) return
					neighbors.push(new Move(blockC.position.x, blockC.position.y, blockC.position.z, node.remainingBlocks, 1, [], [], true))
			}
			else if (blockB.safe && blockC.physical) {
				// Up
				if (!this.getBlock(node, 0, 2, 0).safe ||
					!this.getBlock(node, dir.x, 2, dir.z).safe ||
					!this.getBlock(node, dx, 2, dz).safe) return
					neighbors.push(new Move(blockB.position.x, blockB.position.y, blockB.position.z, node.remainingBlocks, 1, [], [], true))
			}
			else if (blockC.safe && blockD.safe) {
				// Down
				const blockE = this.getBlock(node, dx, -2, dz)
					if (!blockE.physical) return
						neighbors.push(new Move(blockE.x, blockE.position.y, blockE.position.z, node.remainingBlocks, 1, [], [], true))
			}
	}*/

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

	std::vector<Move> getNeighbors(Move &node) {
		std::vector<Move> neighbors;

		// Simple moves in 4 cardinal points

#define MOVES(Direction, DX, DZ) \
	this->getMoveCardinal(node, DX, DZ, neighbors); \
	this->getMoveJumpUp(node, DX, DZ, neighbors); \
	this->getMoveDropDown(node, DX, DZ, neighbors);

		MOVES(North, -1, 0);
		MOVES(South, 1, 0);
		MOVES(East, 0, -1);
		MOVES(West, 0, 1);
			
		// console.log(dir);

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

		// console.log('Neighbors', node, neighbors);


		return neighbors;
	}
};