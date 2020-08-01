#pragma once

#include <functional>
#include <vector>
#include "PathingGoals.h"

// For sanity purposes only one PathingProvider
namespace PathingProvider {

    enum ActionType : int {
        Destroy,
        Place,
        Movement
    };

    enum MovementType : int {
        North,
        South,
        East,
        West,
        NorthEast,
        NorthWest,
        SouthEast,
        SouthWest,
        Up,
        Down
    };

    struct ExecutableAction {
        ActionType actionType;
        MovementType movementType;
        bool canSprint;
        BlockRef block;
        int debugLine;
    };

	struct ExecutableMoves {
        bool good;
        std::vector<ExecutableAction> actions;
        std::vector<BlockRef> places;
        std::vector<BlockRef> breaks;
	};

    enum AssesmentStatus {
        // The path is rejected and must be re-calculated. We will try only once more
        // before returning to the callee.
        Reject,
        Accept
    };

    ExecutableMoves path(PathingRequest& request);

    AssesmentStatus assess(PathingRequest& request, std::vector<Move>& moves, ExecutableMoves& out);

};