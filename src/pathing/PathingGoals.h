#pragma once

#include <vector>
#ifdef __cplusplus
extern "C" {
#endif
#include "AStar.h"
#ifdef __cplusplus
}
#endif
#include "Move.h"

// We want to get to a specifc block type
namespace GetToLocationGoal {
	std::vector<Move> Execute(PathingRequest &request);

	static float Heuristic(void* fromNode, void* toNode, void* context);

	static float Heuristic2(void* fromNode, void* toNode, void* context);

	static int ShouldExit(AInt visitedCount, void* visitingNode, void* goalNode, void* context);

	static void NodeNeighbors(ASNeighborList neighbors, void* node, void* context);

	static int NodeCompare(void* node1, void* node2, void* context);
}

#if 0
namespace GetToBlockTypeGoal {
	void Execute(PathingRequest& request);

	static float Heuristic(void* fromNode, void* toNode, void* context);

	static int ShouldExit(AInt visitedCount, void* visitingNode, void* goalNode, void* context);

	static void NodeNeighbors(ASNeighborList neighbors, void* node, void* context);

	static int NodeCompare(void* node1, void* node2, void* context);
}

namespace GetToSafeLocationFromProjectileGoal {
	void Execute(PathingRequest& request);

	static float Heuristic(void* fromNode, void* toNode, void* context);

	static int ShouldExit(AInt visitedCount, void* visitingNode, void* goalNode, void* context);

	static void NodeNeighbors(ASNeighborList neighbors, void* node, void* context);

	static int NodeCompare(void* node1, void* node2, void* context);
}
#endif