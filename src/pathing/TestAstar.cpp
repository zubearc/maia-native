#include <cstring>
#include <cmath>
#include <cstdio>
#include <ctime>

#include "TestAStar.h"
#include "Movement.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "AStar.h"
#ifdef __cplusplus
}
#endif

/*struct PathNode {
    int x;
    int y;
    int z;
};*/
#if 1
// 198 63 211
Move pathFrom = { 198, 63, 211 };
Move pathTo = { 202,63,210 };

Movements* movements = nullptr;

/*struct Move2 {
    int x;
    int y;
    int z;
};*/

static void PathNodeNeighbors(ASNeighborList neighbors, void* node, void* context) {
    auto move = (Move*)node;
    auto n = movements->getNeighbors(*move);

    //printf("Neighbors for %d,%d,%d=%d\n", move->x, move->y, move->z, n.size());

    for (auto neighbor : n) {
        //printf("-> %d,%d,%d=%d %d\n", neighbor.x, neighbor.y, neighbor.z, neighbor.blockId, neighbor.cost);
        ASNeighborListAdd(neighbors, &neighbor, neighbor.cost);
    }
/*
    auto x = move->x;
    auto y = move->y;
    auto z = move->z;

    auto grid = [&](int x, int y, int z) {
        Move move{ x, y, z };
        ASNeighborListAdd(neighbors, &move, 1.0f);
        return 1;
    };


    auto west = grid(x - 1, y, z);
    auto east = grid(x + 1, y, z);
    auto south = grid(x, y, z - 1);
    auto north = grid(x, y, z + 1);

    auto northup = grid(x, y + 1, z + 1);
    auto northdown = grid(x, y - 1, z + 1);

    auto southup = grid(x, y + 1, z - 1);
    auto southdown = grid(x, y - 1, z - 1);
    auto eastup = grid(x + 1, y + 1, z);
    auto eastdown = grid(x + 1, y - 1, z);
    auto westup = grid(x - 1, y + 1, z);
    auto westdown = grid(x - 1, y - 1, z);

    auto sw = grid(x - 1, y, z - 1);
    auto se = grid(x + 1, y, z - 1);
    auto nw = grid(x - 1, y, z + 1);
    auto ne = grid(x + 1, y, z + 1);
    */
}

const float sqrt2 = 1.4142;

//
float distanceXZ(float x, float z) {
    auto dx = abs(x);
    auto dz = abs(z);
    return abs(dx - dz) + fmin(dx, dz) * sqrt2;
}


static float PathNodeHeuristic(void* fromNode, void* toNode, void* context) {
    auto from = (Move*)fromNode;
    auto to = (Move*)toNode;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define BAR 1
#if BAR
    auto dx = from->x - to->x;
    auto dy = from->y - to->y;
    auto dz = from->z - to->z;
    return distanceXZ(dx, dz) + abs(dy);
#else
    auto D = 1;
    auto D2 = sqrt2;
    auto d1 = abs(to->x - from->x);
    auto d2 = abs(to->y - from->y);
    auto d3 = abs(to->z - from->z);
    // return 0;
    return (D * (d1 + d2 + d3)) + ((D2 - (2 * D)) * MIN(d1, d3));

#endif


}

int distance();

int manhattan(Move &pos1, Move &pos0) {
    auto d1 = abs(pos1.x - pos0.x);
    auto d2 = abs(pos1.y - pos0.y);
    auto d3 = abs(pos1.z - pos0.z);
    return d1 + d2 + d3;
}

static int PathNodeCompare(void* node1, void* node2, void* context) {
    auto from = (Move*)node1;
    auto to = (Move*)node2;

    //printf("CMP (%d %d %d), (%d %d %d) -> ", from->x, from->y, from->z, to->x, from->y, from->z);

    return memcmp(node1, node2, sizeof(int) * 3);
    auto dist1 = manhattan(*from, *(Move*)context);
    auto dist2 = manhattan(*to, *(Move*)context);

    // Clever optimization here - we can simply memcmp on the first 12 bytes
    if (memcmp(node1, node2, sizeof(int) * 3) == 0) {
        return 0;
    }

    return dist1 < dist2 ? -1 : 1;
#if 0
    if (from->x == to->x && from->y == to->y && from->z == to->z) { // are the same
        //printf("0\n");
        return 0;
    }

    // Below does't really matter--we can return 0 or 1, and it will be sorted out later
    //auto xyz1 = from->x + from->y + from->z;
    //auto xyz2 = to->x + to->y + to->z;
    //auto a = from->cost < to->cost ? -1 

    auto c = memcmp(node1, node2, sizeof(Move));

    //printf("%d\n", c);
    return c;
#endif
}

unsigned int clockStart;

static int PathShouldExit(AInt visitedCount, void* visitingNode, void* goalNode, void* context) {

    auto clockNow = clock();

    auto clockDelta = clockNow - clockStart;

    //printf("Visited: %d, Cycles Sofar: %d\n", visitedCount, clockDelta);

    if (visitedCount > 10000) {
        return 1;
    }

    return 0;
}


static const ASPathNodeSource PathNodeSource = {
    sizeof(Move),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    &PathShouldExit,
    //NULL,
    &PathNodeCompare
};
//auto from = new Move2{ 198, 63, 211 };
//auto to = new Move2{ 204, 64, 213 };

void testAstar() {
    movements = new Movements();

    for (int i = 0; i < 10000; i++) {
        ASPath path = ASPathCreate(&PathNodeSource, NULL, &pathFrom, &pathTo);


        if (ASPathGetCount(path) > 1) {
            for (int i = 0; i < ASPathGetCount(path); i++) {
                Move* pathNode = (Move*)ASPathGetNode(path, i);

                //printf("(%d, %d, %d [Places=%d, Destoys=%d]), ", pathNode->x, pathNode->y, pathNode->z, pathNode->numToPlace, pathNode->numToBreak);
            }
        }
        else {
            //printf("No path found!\n");
        }

        ASPathDestroy(path);
    }
    
    delete movements;

}

std::vector<Move> astar(int x1, int y1, int z1, int x2, int y2, int z2, bool allowBreaking, bool allowPlacing) {
    movements = new Movements();
    movements->allowBreaking = allowBreaking;
    movements->allowPlacing = allowPlacing;

    Move from{x1, y1, z1};
    Move to{x2, y2, z2};

    auto timeStart = time(NULL);

    auto clockStartTime = clock();
    clockStart = clockStartTime;

    ASPath path = ASPathCreate(&PathNodeSource, NULL, &from, &to);

    std::vector<Move> moves;


    if (ASPathGetCount(path) > 1) {
        for (int i = 0; i < ASPathGetCount(path); i++) {
            Move* pathNode = (Move*)ASPathGetNode(path, i);

            printf("(%d, %d, %d [Places=%d, Destoys=%d]), ", pathNode->x, pathNode->y, pathNode->z, pathNode->numToPlace, pathNode->numToBreak);

            moves.push_back(*pathNode);
        }
    } else {
        printf("No path found!\n");
    }
    //auto timeEnd = time(NULL);
    auto clockEndTime = clock();
    auto clockDelta = clockEndTime - clockStartTime;
    fprintf(stdout, "Took %d cycles (%4.4fs)\n %d", clockDelta, ((float)clockDelta) / CLOCKS_PER_SEC, CLOCKS_PER_SEC / 4);
    ASPathDestroy(path);
    delete movements;
    return moves;
}

#else

static const int blockSize = 50;
static const int worldWidth = 20;
static const int worldHeight = 13;


static int world[worldWidth * worldHeight] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

typedef struct {
    int x;
    int y;
    int z;
} PathNode;

PathNode pathFrom = { 0,0 };
PathNode pathTo = { 8,5 };

static int WorldAt(int x, int y)
{
    if (x >= 0 && x < worldWidth && y >= 0 && y < worldHeight) {
        return world[y * worldWidth + x];
    }
    else {
        return -1;
    }
}

static void PathNodeNeighbors(ASNeighborList neighbors, void* node, void* context) {
    PathNode* pathNode = (PathNode*)node;

    if (WorldAt(pathNode->x + 1, pathNode->y) == 0) {
        auto neighbor = PathNode{ pathNode->x + 1, pathNode->y };
        ASNeighborListAdd(neighbors, &neighbor, 1);
    }
    if (WorldAt(pathNode->x - 1, pathNode->y) == 0) {
        auto neighbor = PathNode{ pathNode->x - 1, pathNode->y };
        ASNeighborListAdd(neighbors, &neighbor, 1);
    }
    if (WorldAt(pathNode->x, pathNode->y + 1) == 0) {
        auto neighbor = PathNode{ pathNode->x, pathNode->y + 1 };
        ASNeighborListAdd(neighbors, &neighbor, 1);
    }
    if (WorldAt(pathNode->x, pathNode->y - 1) == 0) {
        auto neighbor = PathNode{ pathNode->x, pathNode->y - 1 };
        ASNeighborListAdd(neighbors, &neighbor, 1);
    }
}

static float PathNodeHeuristic(void* fromNode, void* toNode, void* context)
{
    PathNode* from = (PathNode*)fromNode;
    PathNode* to = (PathNode*)toNode;

    // using the manhatten distance since this is a simple grid and you can only move in 4 directions
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}

static const ASPathNodeSource PathNodeSource =
{
    sizeof(PathNode),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    NULL,
    NULL
};

struct Test {
    void* something = 0;
};

void testAstar() {
    Test* test = (Test*)calloc(sizeof(Test), 1);

    test->something = realloc(test->something, 8 * 8);
    auto from = new PathNode{ 0,0 };
    auto to = new PathNode{ 8,5 };

    ASPath path = ASPathCreate(&PathNodeSource, NULL, from, to);

    printf("Path len: %d\n", ASPathGetCount(path));

    if (ASPathGetCount(path) > 1) {
        for (int i = 0; i < ASPathGetCount(path); i++) {
            PathNode* pathNode = (PathNode*)ASPathGetNode(path, i);
            printf("(%d, %d, %d), ", pathNode->x, pathNode->y, 0);
        }
    }

    ASPathDestroy(path);
}


#endif