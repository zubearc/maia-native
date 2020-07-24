#pragma once

#include "../je/WorldProvider.h"
#include "../blocks/MinecraftBlocks.h"
#include "../blocks/MinecraftBlockHelper.h"

#include "Move.h"

using namespace MinecraftBlockHolder;

class Movements {
	int digCost = 6;
	int placeCost = 4;
	int maxDropDown = 4;
	bool allowPlacing = false;
	bool allowBreaking = false;

	bool dontCreateFlow = true;

public:

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

		//printf("%d: Air=%d, Solid=%d, Destroy=%d, Transparent=%d\n", blockId, (blockProperties & BlockProp::AIR) > 0, (blockProperties & BlockProp::SOLID) > 0,
		//	(blockProperties & BlockProp::DESTROYABLE) > 0, (blockProperties & BlockProp::TRANSPARENT) > 0);


		return { blockProperties, x + dx, y + dy, z + dz, blockId, blockMetadata };
	}

	BlockRef getBlock(BlockRef &node, int dx, int dy, int dz) {
		return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
	}

	BlockRef getBlock(Move &node, int dx, int dy, int dz) {
		return this->getBlock(node.x, node.y, node.z, dx, dy, dz);
	}

#define IS_SOLID(BLOCK) (BLOCK & BlockProp::SOLID)
#define IS_TRANSPARENT(BLOCK) (BLOCK & BlockProp::TRANSPARENT)
#define IS_LIQUID(BLOCK) (BLOCK & BlockProp::WATER)
#define IS_BREAKABLE(BLOCK) (BLOCK & BlockProp::DESTROYABLE)
#define IS_PASSABLE(BLOCK) (BLOCK & (BlockProp::AIR | BlockProp::TRANSPARENT))

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

	float safeOrBreak(BlockRef block, BlockRef *toBreak, char &numToBreak) {

		//if (block.x == 198 && block.y == 63 && block.z == 212) _ASSERT(false);

		if (IS_PASSABLE(block.properties)) return 0;
		if (!this->allowBreaking) return 100;
		if (!this->safeToBreak(block)) return 100; // Can't break, so can't move
		
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

		float cost = 2.0f; // move cost (move+jump)
		BlockRef toBreak[3];
		BlockRef toPlace[3];
		char numToBreak = 0;
		char numToPlace = 0;

		if (!IS_SOLID(blockC.properties)) {
			if (!this->allowPlacing) return;
				// if (node.remainingBlocks === 0) return // not enough blocks to place

				// TODO: avoid entities as part of placing blocks
			auto blockD = this->getBlock(block, dx, -1, dz);
			if (!IS_SOLID(blockD.properties)) {
				// if (node.remainingBlocks === 1) return // not enough blocks to place
				// toPlace.push({ x: node.x, y: node.y - 1, z: node.z, dx: dir.x, dy: 0, dz: dir.z })
				toPlace[numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, -dx, 0, -dz };
				cost += this->placeCost; // additional cost for placing a block
			}

				// toPlace.push({ x: node.x + dir.x, y: node.y - 1, z: node.z + dir.z, dx: 0, dy: 1, dz: 0 })
			toPlace[numToPlace++] = BlockRef{ blockC.properties, blockC.x, blockC.y, blockC.z, dx, -1, -dz };
			cost += this->placeCost; // additional cost for placing a block
		}

		cost += this->safeOrBreak(blockA, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockH, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockB, toBreak, numToBreak);
		if (cost > 100) return;
		neighbors.push_back(Move{ blockB.x, blockB.y, blockB.z, blockB.blockId, blockB.blockMetadata, 
			99, cost, blockB.properties, /*toBreak, toPlace*/ });
	}

	void getMoveCardinal(Move block, char dx, char dz, std::vector<Move> &neighbors) {
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockC = this->getBlock(block, dx, 0, dz);
		auto blockD = this->getBlock(block, dx, -1, dz);
			// console.log('blocks', blockB, blockC, blockD);

		float cost = 1.0f; // move cost
		BlockRef toBreak[3];
		BlockRef toPlace[3];
		char numToBreak = 0;
		char numToPlace = 0;

		//printf("GetCardinal %d %d %d %d, %d\n", dx, dz, neighbors.size(), blockD.properties, blockC.properties);
		//printf("Props %d %d\n", IS_SOLID(blockD.properties), IS_LIQUID(blockC.properties));

		if (!IS_SOLID(blockD.properties) && !IS_LIQUID(blockC.properties)) {
			if (!this->allowPlacing) return;
				// if (node.remainingBlocks === 0) return // not enough blocks to place
				// toPlace.push({ type: 'cardinal', x: node.x, y: node.y - 1, z: node.z, dx: dir.x, dy: 0, dz: dir.z })
			toPlace[numToPlace++] = BlockRef{ blockD.properties, blockD.x, blockD.y, blockD.z, -dx, 0, -dz };

			cost += this->placeCost; // additional cost for placing a block
		}

		//printf("Working! %d %d %d\n", dx, dz, neighbors.size());

		cost += this->safeOrBreak(blockB, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockC, toBreak, numToBreak);
		if (cost > 100) return;

		//printf("Good! %d %d %d\n", dx, dz, neighbors.size());


		//if (blockC.x == 198 && blockC.y == 63 && blockC.z == 212) _ASSERT(false);

		neighbors.push_back(Move{ blockC.x, blockC.y, blockC.z, blockC.blockId, blockC.blockMetadata,
			99, cost, blockB.properties, /*toBreak, toPlace*/ });
	}

	void getMoveDiagonal(Move block, int dx, int dz, std::vector<Move> &neighbors) {
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockB1 = this->getBlock(block, 0, 1, dz);
		auto blockB2 = this->getBlock(block, dx, 1, 0);

		auto blockC = this->getBlock(block, dx, 0, dz);
		auto blockC1 = this->getBlock(block, 0, 0, dz);
		auto blockC2 = this->getBlock(block, dx, 0, 0);

		auto blockD = this->getBlock(block, dx, -1, dz);

		float cost = 1.4142f; // move cost
		BlockRef toBreak[3];
		char numToBreak = 0;

		if (!IS_SOLID(blockD.properties)) return; // we don't place blocks in diagonal

		cost += this->safeOrBreak(blockB1, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockB2, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockC1, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockC2, toBreak, numToBreak);
		if (cost > 100) return;

		cost += this->safeOrBreak(blockC, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockB, toBreak, numToBreak);
		if (cost > 100) return;

		//neighbors.push(new Move(blockC.x, blockC.y, blockC.z, node.remainingBlocks, cost, toBreak))
		neighbors.push_back(Move{ blockC.x, blockC.y, blockC.z, blockC.blockId, blockC.blockMetadata,
			99, cost, blockB.properties, /*toBreak*/ });
	}

	BlockRef getLandingBlock(Move block, int dx, int dz) {
		auto blockLand = this->getBlock(block, dx, -2, dz);
		for (auto i = 0; i < this->maxDropDown - 1; i++) {
			if (IS_SOLID(blockLand.properties) || !IS_PASSABLE(blockLand.properties)) break;
			blockLand = this->getBlock(block, dx, -2 - i, dz);
		}
		return blockLand;
	}

	void getMoveDropDown(Move block, char dx, char dz, std::vector<Move> &neighbors) {
		auto blockB = this->getBlock(block, dx, 1, dz);
		auto blockC = this->getBlock(block, dx, 0, dz);
		auto blockD = this->getBlock(block, dx, -1, dz);

		float cost = 1.0f; // move cost
		BlockRef toBreak[3];
		BlockRef toPlace[3];
		char numToBreak = 0;
		char numToPlace = 0;


		auto blockLand = this->getLandingBlock(block, dx, dz);

		// if (!blockLand.solid) return // TODO: place? (bridging down)

		if (!IS_SOLID(blockLand.properties) && !IS_LIQUID(blockLand.properties)) {
			if (!this->allowPlacing) return;
				// if (node.remainingBlocks === 0) return // not enough blocks to place

			auto blockC = this->getBlock(blockLand, 0, -1, 0); // Block underneath
			if (IS_SOLID(blockC.properties)) {
				toPlace[numToPlace++] = { blockLand.properties, blockLand.x, blockLand.y, blockLand.z, 0, -1, 0 };
				cost += this->placeCost; // additional cost for placing a block
			} else {


#define GET_ADJACENT_BLOCK(Name, DX, DZ) \
				if (numToPlace == 0) { \
					BlockRef block##Name = this->getBlock(blockLand, DX, 0, DZ); /* Block adjacent */ \
					if (IS_SOLID(blockB.properties)) { \
						toPlace[numToPlace++] = { blockB.properties, blockLand.x, blockLand.y, blockLand.z, DX, 0, DZ }; \
						cost += this->placeCost; /* additional cost for placing a block */ \
					} \
				}

				GET_ADJACENT_BLOCK(North, -1, 0);
				GET_ADJACENT_BLOCK(South, 1, 0);
				GET_ADJACENT_BLOCK(East, 0, -1);
				GET_ADJACENT_BLOCK(West, 0, 1);
			}


			// If we didn't place anything & block below player's next move is passable,
			// abort this operation.
			if (IS_PASSABLE(blockLand.properties) && numToPlace == 0) {
				return;
			}
		}

		cost += this->safeOrBreak(blockB, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockC, toBreak, numToBreak);
		if (cost > 100) return;
		cost += this->safeOrBreak(blockD, toBreak, numToBreak);
		if (cost > 100) return;

		printf("DD %d,%d,%d %d\n", blockLand.x, blockLand.y, blockLand.z, cost);

		//neighbors.push_back(Move{ blockLand.x, blockLand.y + 1, blockLand.z, node.remainingBlocks - toPlace.length, cost, toBreak, toPlace))
		neighbors.push_back(Move{ blockLand.x, blockLand.y + 1, blockLand.z, blockLand.blockId, blockLand.blockMetadata,
			99, cost, blockLand.properties, /*toBreak, toPlace*/ });
	}

	void getMoveDown(Move &block, std::vector<Move> &neighbors) {
		auto block0 = this->getBlock(block, 0, -1, 0);

		float cost = 1.0f; // move cost
		BlockRef toBreak[3];
		BlockRef toPlace[3];
		char numToBreak = 0;
		char numToPlace = 0;

		auto blockLand = this->getLandingBlock(block, 0, 0);
		if (!IS_SOLID(blockLand.properties)) return;

		cost += this->safeOrBreak(block0, toBreak, numToBreak);
		if (cost > 100) return;

		//neighbors.push(new Move(blockLand.x, blockLand.y + 1, blockLand.z, node.remainingBlocks - toPlace.length, cost, toBreak, toPlace))

		neighbors.push_back(Move{ blockLand.x, blockLand.y, blockLand.z, blockLand.blockId, blockLand.blockMetadata,
	99, cost, blockLand.properties, /*toBreak, toPlace*/ });
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
		for (auto neighbor : neighbors) {
		//	printf("N %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		//neighbors.empty();
		MOVES(South, 1, 0);
		for (auto neighbor : neighbors) {
		//	printf("S %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		MOVES(East, 0, -1);
		//neighbors.empty();
		for (auto neighbor : neighbors) {
		//	printf("E %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		MOVES(West, 0, 1);
		//neighbors.empty();
		for (auto neighbor : neighbors) {
		//	printf("W %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
			// console.log(dir);

					// if (this.allowParkour) {
					//     this.getMoveParkourForward(node, dir, neighbors)
					// }

		// Diagonals
		this->getMoveDiagonal(node, -1, -1, neighbors); //NE
		for (auto neighbor : neighbors) {
		//	printf("NE %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		//neighbors.empty();
		this->getMoveDiagonal(node, -1, 1, neighbors); //NW
		for (auto neighbor : neighbors) {
		//	printf("NW %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		//neighbors.empty();
		this->getMoveDiagonal(node, 1, -1, neighbors); //Se
		for (auto neighbor : neighbors) {
		//	printf("SE %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		//neighbors.empty();
		this->getMoveDiagonal(node, 1, 1, neighbors); //Sw
		//printf("%d %d %d ->\n", node.x, node.y, node.z);

		for (auto neighbor : neighbors) {
			//printf("... %d %d %d\n", neighbor.x, neighbor.y, neighbor.z);
		}
		// this.getMoveDown(node, neighbors)

		// if (this.allow1by1towers) {
		//     this.getMoveUp(node, neighbors)
		// }

		// console.log('Neighbors', node, neighbors);


		return neighbors;
	}
};