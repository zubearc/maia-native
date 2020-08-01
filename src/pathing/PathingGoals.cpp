#include <cstdio>
#include <ctime>
#include <cstring>
#include <cmath>
#include "PathingGoals.h"
#include "Movement.h"

const float sqrt2 = 1.4142;

namespace GetToLocationGoal {
    float distanceXZ(float x, float z) {
        auto dx = abs(x);
        auto dz = abs(z);
        return abs(dx - dz) + fmin(dx, dz) * sqrt2;
    }

    static const ASPathNodeSource PathNodeSource = {
        sizeof(Move),
        &GetToLocationGoal::NodeNeighbors,
        &GetToLocationGoal::Heuristic2,
        &GetToLocationGoal::ShouldExit,
        &GetToLocationGoal::NodeCompare
    };

    unsigned int clockStart;

    std::vector<Move> Execute(PathingRequest& request) {
        auto movements = new Movements();
        movements->allowBreaking = request.allowBreaking;
        movements->allowPlacing = request.allowPlacing;

        request.movements = movements;

        Move from{ request.sX, request.sY, request.sZ };
        Move to{ request.eX, request.eY, request.eZ };

        auto clockStartTime = clock();

        request.startMove = &from;
        request.endMove = &to;

        ASPath path = ASPathCreate(&PathNodeSource, &request, &from, &to);

        std::vector<Move> moves;


        if (ASPathGetCount(path) > 1) {
            for (int i = 0; i < ASPathGetCount(path); i++) {
                Move* pathNode = (Move*)ASPathGetNode(path, i);
                printf("(%d, %d, %d [Places=%d, Destoys=%d] #%d), ", pathNode->x, pathNode->y, pathNode->z, pathNode->numToPlace, pathNode->numToBreak, pathNode->remainingBlocks);
                moves.push_back(*pathNode);
            }
        } else {
            printf("No path found!\n");
        }
        //auto timeEnd = time(NULL);
        auto clockEndTime = clock();
        auto clockDelta = clockEndTime - clockStartTime;
        fprintf(stdout, "Took %d cycles for %d nodes (%4.4fs) \n %d", clockDelta, request.traversedNodes, ((float)clockDelta) / CLOCKS_PER_SEC, CLOCKS_PER_SEC / 4);
        ASPathDestroy(path);
        delete movements;

        return moves;
	}
    
    float Heuristic(void* fromNode, void* toNode, void* context) {
        auto from = (Move*)fromNode;
        auto to = (Move*)toNode;

        // Heuristic used in Baritone/mineflayer-pathfinder
        // It prioritizes XZ axes
        auto dx = from->x - to->x;
        auto dy = from->y - to->y;
        auto dz = from->z - to->z;
        return distanceXZ(dx, dz) + abs(dy);
    }
#define MIN(a,b) (((a)<(b))?(a):(b))

    float Heuristic2(void* fromNode, void* toNode, void* context) {
        auto from = (Move*)fromNode;
        auto to = (Move*)toNode;

        auto D = 1;
        auto D2 = sqrt2;
        auto d1 = abs(to->x - from->x);
        auto d2 = abs(to->y - from->y);
        auto d3 = abs(to->z - from->z);
        // return 0;
        return (D * (d1 + d2 + d3)) + ((D2 - (2 * D)) * MIN(d1, MIN(d2, d3)));

        return 0.0f;
    }
    
    int ShouldExit(AInt visitedCount, void* visitingNode, void* goalNode, void* context) {
        auto pathingRequest = (PathingRequest*)context;

        // TODO: Investigate the pros-cons of using visitedCount to determine when to abort a path
        // or clock()
        if (pathingRequest->maximumNodesToTraverse && visitedCount > pathingRequest->maximumNodesToTraverse) {
            pathingRequest->traversedNodes = visitedCount;
            return 1;
        }
        pathingRequest->traversedNodes = visitedCount;
        return 0;
    }
    
    void NodeNeighbors(ASNeighborList neighbors, void* node, void* context) {
        auto move = (Move*)node;
        auto pathingRequest = (PathingRequest*)context;
        auto n = ((Movements*)pathingRequest->movements)->getNeighbors(*move);

        //printf("Neighbors for %d,%d,%d=%d\n", move->x, move->y, move->z, n.size());

        for (auto neighbor : n) {
            //printf("-> %d,%d,%d=%d %d\n", neighbor.x, neighbor.y, neighbor.z, neighbor.blockId, neighbor.cost);
            ASNeighborListAdd(neighbors, &neighbor, neighbor.cost);
        }
    }
    
    int NodeCompare(void* node1, void* node2, void* context) {
        //auto move1 = (Move*)node1;
        //auto move2 = (Move*)node2;

        //if (move1->x == move2->x && move1->y == move2->y && move1->z == move2->z && move1->numToBreak == move2->numToBreak)

        /*if (memcmp(node2, ((PathingRequest*)context)->startMove, sizeof(int) * 3) == 0 || 
            memcmp(node2, ((PathingRequest*)context)->endMove, sizeof(int) * 3) == 0) {
            memcpy((char*)node1, (char*)node2, sizeof(Move));
            return 0;
        } else {
            auto cmp = memcmp(node1, node2, sizeof(int) * 4);
            return cmp;
        }*/

        auto cmp = memcmp(node1, node2, sizeof(int) * 3);
        if (cmp == 0) {
            memcpy((char*)node2, (char*)node1, sizeof(Move));

            //if (memcmp(node1, ((PathingRequest*)context)->endMove, sizeof(int) * 3) == 0)
             //   memcpy((char*)node1, (char*)node2, sizeof(Move));
            return cmp;
        } else {
            return cmp;
        }
    }
}


namespace GetToLocationGoal {

}

