#include "JSPathingProvider.h"

#include "pathing/TestAStar.h"
#include "pathing/PathingProvider.h"
#include "je/WorldProvider.h"

Napi::Object JSPathingProvider::Initialize(Napi::Env& env, Napi::Object& target) {
    Napi::Function constructor = DefineClass(env, "Pathing", {
        InstanceMethod("getPathToBlockSync", &JSPathingProvider::GetPathToBlockOld),
        InstanceMethod("getPathToBlock", &JSPathingProvider::GetPathToBlock),
        InstanceMethod("getPathToEntity", &JSPathingProvider::GetPathToBlock),
        InstanceMethod("setDenyBlock", &JSPathingProvider::SetDenyBlock),
        InstanceMethod("test", &JSPathingProvider::Test),
    });
    target.Set("Pathing", constructor);
    return target;
}

JSPathingProvider::JSPathingProvider(const Napi::CallbackInfo& info) : Napi::ObjectWrap<JSPathingProvider>(info) {

}

Napi::Value JSPathingProvider::GetPathToBlockOld(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    int x1 = info[0].As<Napi::Number>().Int32Value();
    int y1 = info[1].As<Napi::Number>().Int32Value();
    int z1 = info[2].As<Napi::Number>().Int32Value();

    int x2 = info[3].As<Napi::Number>().Int32Value();
    int y2 = info[4].As<Napi::Number>().Int32Value();
    int z2 = info[5].As<Napi::Number>().Int32Value();

    auto options = info[6].As<Napi::Object>();

    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    bool breaking = false;
    bool placing = false;

    // Note: allowPlacing or allowBreaking cannot be false. Must be undefined or it will be assumed as true.
    if (options.Has("allowPlacing")) {
        placing = true;
    }
    
    if (options.Has("allowBreaking")) {
        breaking = true;
    }

    auto moves = astar(x1, y1, z1, x2, y2, z2, breaking, placing);

    printf("\nPathing (%d %d %d) -> (%d %d %d)\n", x1, y1, z1, x2, y2, z2);

    Napi::Array jsmoves = Napi::Array::New(info.Env(), moves.size());
    
    for (int i = 0; i < moves.size(); i++) {
        auto move = moves[i];
        auto jsmove = Napi::Object::New(info.Env());

        jsmove.Set("x", move.x);
        jsmove.Set("y", move.y);
        jsmove.Set("z", move.z);

        jsmove.Set("cost", move.cost);

        auto toPlace = Napi::Array::New(info.Env(), move.numToPlace);
        auto toBreak = Napi::Array::New(info.Env(), move.numToBreak);

        for (int j = 0; j < move.numToPlace; j++) {
            auto toplace = move.toPlace[j];

            auto jsplace = Napi::Object::New(info.Env());

            jsplace.Set("x", toplace.x);
            jsplace.Set("y", toplace.y);
            jsplace.Set("z", toplace.z);

            jsplace.Set("dx", toplace.ax);
            jsplace.Set("dy", toplace.ay);
            jsplace.Set("dz", toplace.az);

            toPlace[j] = jsplace;
        }

        for (int j = 0; j < move.numToBreak; j++) {
            auto toplace = move.toBreak[j];

            auto jsplace = Napi::Object::New(info.Env());

            jsplace.Set("x", toplace.x);
            jsplace.Set("y", toplace.y);
            jsplace.Set("z", toplace.z);

            jsplace.Set("dx", toplace.ax);
            jsplace.Set("dy", toplace.ay);
            jsplace.Set("dz", toplace.az);

            toBreak[j] = jsplace;
        }

        jsmove.Set("toPlace", toPlace);
        jsmove.Set("toBreak", toBreak);

        jsmove.Set("Debug", move.remainingBlocks);
        
        jsmoves[i] = jsmove;
    }

    return jsmoves;
}

Napi::Value generateJSObject(Napi::Env &env, PathingProvider::ExecutableMoves &ret) {
    auto jsassessedPath = Napi::Object::New(env);

    Napi::Array jsactions = Napi::Array::New(env, ret.actions.size());

    Napi::Array jsplaces = Napi::Array::New(env, ret.places.size());
    Napi::Array jsbreaks = Napi::Array::New(env, ret.breaks.size());

    for (int i = 0; i < ret.actions.size(); i++) {
        auto action = ret.actions[i];

        auto jsaction = Napi::Object::New(env);

        jsaction.Set("ActionType", (int)action.actionType);
        jsaction.Set("CanSprint", action.canSprint);
        jsaction.Set("MovementType", (int)action.movementType);

        jsaction.Set("BlockID", action.block.blockId);
        jsaction.Set("BlockMeta", action.block.blockMetadata);
        jsaction.Set("x", action.block.x);
        jsaction.Set("y", action.block.y);
        jsaction.Set("z", action.block.z);

        if (action.actionType == PathingProvider::ActionType::Place) {
            jsaction.Set("ax", action.block.ax);
            jsaction.Set("ay", action.block.ay);
            jsaction.Set("az", action.block.az);
        }

        jsaction.Set("Debug", action.debugLine);

        jsactions[i] = jsaction;
    }

    for (int i = 0; i < ret.places.size(); i++) {
        auto action = ret.places[i];
        auto jsaction = Napi::Object::New(env);

        jsaction.Set("BlockID", action.blockId);
        jsaction.Set("BlockMeta", action.blockMetadata);

        jsaction.Set("x", action.x);
        jsaction.Set("y", action.y);
        jsaction.Set("z", action.z);

        jsaction.Set("ax", action.ax);
        jsaction.Set("ay", action.ay);
        jsaction.Set("az", action.az);

        jsplaces[i] = jsaction;
    }

    for (int i = 0; i < ret.breaks.size(); i++) {
        auto action = ret.breaks[i];
        auto jsaction = Napi::Object::New(env);

        jsaction.Set("BlockID", action.blockId);
        jsaction.Set("BlockMeta", action.blockMetadata);

        jsaction.Set("x", action.x);
        jsaction.Set("y", action.y);
        jsaction.Set("z", action.z);

        jsaction.Set("ax", action.ax);
        jsaction.Set("ay", action.ay);
        jsaction.Set("az", action.az);

        jsbreaks[i] = jsaction;
    }

    jsassessedPath.Set("success", ret.good);
    jsassessedPath.Set("actions", jsactions);
    jsassessedPath.Set("places", jsplaces);
    jsassessedPath.Set("breaks", jsbreaks);

    return jsassessedPath;
}

Napi::Value JSPathingProvider::GetPathToBlock(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    int x1 = info[0].As<Napi::Number>().Int32Value();
    int y1 = info[1].As<Napi::Number>().Int32Value();
    int z1 = info[2].As<Napi::Number>().Int32Value();

    int x2 = info[3].As<Napi::Number>().Int32Value();
    int y2 = info[4].As<Napi::Number>().Int32Value();
    int z2 = info[5].As<Napi::Number>().Int32Value();

    auto options = info[6].As<Napi::Object>();

    auto callback = info[7].As<Napi::Function>();

    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    PathingRequest request;

    if (info.Length() < 6) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    bool breaking = false;
    bool placing = false;
    bool dryRun = false;

    // Note: allowPlacing or allowBreaking cannot be false. Must be undefined or it will be assumed as true.
    if (options.Has("allowPlacing")) {
        placing = true;
    }

    if (options.Has("allowBreaking")) {
        breaking = true;
    }

    if (options.Has("dryRun")) {
        dryRun = true;
    }

    request.goal = PathingGoal::GetToLocation;
    request.allowPlacing = placing;
    request.allowBreaking = breaking;
    if (dryRun) {
        request.maximumNodesToTraverse = 10000;
        printf("Doing a dry run!\n");
    }

    request.sX = x1;
    request.sY = y1;
    request.sZ = z1;

    request.eX = x2;
    request.eY = y2;
    request.eZ = z2;

    printf("\nPathing (%d %d %d) -> (%d %d %d)\n", x1, y1, z1, x2, y2, z2);

    JSPathingWorker* asyncWorker = new JSPathingWorker(callback, clock());
    asyncWorker->setRequest(request);

    asyncWorker->Queue();

    return Napi::Boolean::New(env, true);
}

Napi::Value JSPathingProvider::SetDenyBlock(const Napi::CallbackInfo& info) {
    int x1 = info[0].As<Napi::Number>().Int32Value();
    int y1 = info[1].As<Napi::Number>().Int32Value();
    int z1 = info[2].As<Napi::Number>().Int32Value();

    // Technically this should be DENY but 0 works too for now (til raytracing)
    WorldProvider::setBlockProperties(x1, y1, z1, 0);
    return Napi::Boolean::New(info.Env(), true);
}

void JSPathingProvider::Test(const Napi::CallbackInfo& info) {
    testAstar();
}

JSPathingWorker::JSPathingWorker(Napi::Function& callback, clock_t runTime)
    : Napi::AsyncWorker(callback), runTime(runTime) {
    this->startTime = runTime;
}

void JSPathingWorker::Execute() {
    this->actions = PathingProvider::path(request);
    //printf("executed...\n");
}

void JSPathingWorker::OnOK() {
    auto env = Env();
    auto jsassesedPath = generateJSObject(env, this->actions);
    //printf("returning %d...\n", this->actions.good);
    Callback().Call({ env.Null(), jsassesedPath });
}
