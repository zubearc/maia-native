#pragma once

#include <vector>
#include "Move.h"

void testAstar();

std::vector<Move> astar(int x1, int y1, int z1, int x2, int y2, int z2, bool allowBreaking = false, bool allowPlacing = false);