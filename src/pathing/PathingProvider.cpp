#include "PathingProvider.h"

#include <map>
#include <cstring>
#include "Movement.h"

namespace PathingProvider {

  
    ExecutableMoves path(PathingRequest& request) {
        if (request.goal == PathingGoal::GetToLocation) {
            auto result = GetToLocationGoal::Execute(request);

            ExecutableMoves actions;

            auto ret = assess(request, result, actions);

            if (ret == Reject) {
                ret = assess(request, result, actions);
            }

            if (ret == Reject) {
                actions.good = false;
            } else {
                actions.good = true;
            }

            return actions;
        }
    }


    AssesmentStatus assess(PathingRequest& request, std::vector<Move>& moves, ExecutableMoves& out) {
        /*
        sometimes a path may require player intervention: for example, open a door to go through it.
        we want to flag these events so they can be handled by the movement logic in the agent.
        if the agent is unable to traverse the path, obstructing blocks will be marked as SOLID,
        and possibly DESTROYABLE or AVOID. If it's AVOID we will avoid being even adjacent to them.
        */

        std::vector<ExecutableAction> executable_moves;
        std::vector<BlockRef> breaks;
        std::vector<BlockRef> places;

        //let dir = 0; //Forward,Backward,Left,Right,Up,Down,SE,SW,NE,NW
        MovementType dir;

        Move* last = nullptr;

 
        auto breakCount = [&](BlockRef *blk) {
            int count = 0;
            for (auto brk : breaks) {
                if (memcmp(&brk.x, &blk->x, sizeof(int) * 3) == 0) {
                    count++;
                }
            }
            //printf("%d breaks\n", breaks.size());
            return count;
        };

        auto placeCount = [&](BlockRef* blk) {
            int count = 0;
            for (auto brk : places) {
                if (memcmp(&brk.x, &blk->x, sizeof(int) * 3) == 0) {
                    count++;
                }
            }
            //printf("%d places\n", places.size());
            return count;
        };

        //TODO: check block neighbors & simplify path
        for (auto &entry : moves) {
            for (int i = 0; i < entry.numToBreak; i++) {
                ExecutableAction m;
                m.actionType = ActionType::Destroy;
                m.block = entry.toBreak[i];
                m.debugLine = entry.remainingBlocks;
                executable_moves.push_back(m);
                breaks.push_back(m.block);
            }

            // Did previously place a block above the current movement? If so, destroy it
            auto blockOverhead = BlockRef{ 0, entry.x, entry.y + 1, entry.z };
            if (placeCount(&blockOverhead) > breakCount(&blockOverhead)) {
                //printf("Assessment found an error %d %d %d\n", blockOverhead.x, blockOverhead.y, blockOverhead.z);
                ExecutableAction m;
                m.actionType = ActionType::Destroy;
                m.block = blockOverhead;
                m.debugLine = entry.remainingBlocks;
                executable_moves.push_back(m);
                breaks.push_back(m.block);
            }
            // Same check for body block
            auto blockBody = BlockRef{ 0, entry.x, entry.y, entry.z };
            if (placeCount(&blockBody) > breakCount(&blockBody)) {
                //printf("Assessment found an error %d %d %d\n", blockBody.x, blockBody.y, blockBody.z);
                ExecutableAction m;
                m.actionType = ActionType::Destroy;
                m.block = blockBody;
                m.debugLine = entry.remainingBlocks;
                executable_moves.push_back(m);
                breaks.push_back(m.block);
            }
            // End

            for (int i = 0; i < entry.numToPlace; i++) {
                ExecutableAction m;
                m.actionType = ActionType::Place;
                m.block = entry.toPlace[i];
                m.debugLine = entry.remainingBlocks;
                executable_moves.push_back(m);
                places.push_back(m.block);
            }

            // Did previously break the block underneath?
            auto blockUnderneath = BlockRef{ 0, entry.x, entry.y - 1, entry.z };
            //printf("block underneath %d %d %d: %d %d\n", blockUnderneath.x, blockUnderneath.y, blockUnderneath.z, breakCount(&blockUnderneath), placeCount(&blockUnderneath));

            if (breakCount(&blockUnderneath) > placeCount(&blockUnderneath)) {
                //printf("Assessment found an error %d %d %d\n", blockUnderneath.x, blockUnderneath.y, blockUnderneath.z);
                ExecutableAction m;
                m.actionType = ActionType::Place;
                m.block = blockUnderneath;
                m.debugLine = entry.remainingBlocks;
                executable_moves.push_back(m);
                places.push_back(m.block);
            }
            
            // End

            if (last) {
                // North = -z, West=-x
                // last.x - 
                if (last->y == entry.y && last->x == entry.x) {
                    if (last->z < entry.z) {
                        // Move South
                        dir = South;
                    } else {
                        // Move North
                        dir = North;
                    }
                } else if (last->y == entry.y && last->z == entry.z) {
                    if (last->x < entry.x) {
                        // East
                        dir = East;
                    } else {
                        // West
                        dir = West;
                    }
                } else if (last->y == entry.y) {
                    if (last->z < entry.z && last->x < entry.x) {
                        dir = SouthEast;
                    } else if (last->z < entry.z && last->x > entry.x) {
                        dir = SouthWest;
                    } else if (last->z > entry.z && last->x < entry.x) {
                        dir = NorthEast;
                    } else if (last->z > entry.z && last->x > entry.x) {
                        dir = NorthWest;
                    }
                } else if (last->y != entry.y) {
                    // Up
                    dir = last->y < entry.y ? Up : Down;
                } else {
                    fprintf(stderr, "Illegal state: PathingProvider\n");
                }

                ExecutableAction m;
                m.actionType = Movement;
                m.canSprint = true;
                m.movementType = dir;
                m.debugLine = entry.remainingBlocks;
                m.block = BlockRef{ entry.props, entry.x, entry.y, entry.z, entry.blockId, entry.blockMetadata };
                executable_moves.push_back(m);
            } else {
                ExecutableAction m;
                m.actionType = Movement;
                m.canSprint = true;
                m.debugLine = entry.remainingBlocks;
                m.block = BlockRef{ entry.props, entry.x, entry.y, entry.z, entry.blockId, entry.blockMetadata };
                executable_moves.push_back(m);
            }

            last = &entry;

        }

        out.actions = executable_moves;
        out.places = places;
        out.breaks = breaks;
        return Accept;
    }
}