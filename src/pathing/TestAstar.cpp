#ifdef __cplusplus
extern "C" {
#endif
#include "AStar.h"
#ifdef __cplusplus
}
#endif

typedef struct {
    int x;
    int y;
} PathNode;

PathNode pathFrom = { 0,0 };
PathNode pathTo = { 0,0 };

static void PathNodeNeighbors(ASNeighborList neighbors, void* node, void* context) {

}

static float PathNodeHeuristic(void* fromNode, void* toNode, void* context) {

}

static const ASPathNodeSource PathNodeSource = {
    sizeof(PathNode),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    NULL,
    NULL
};
