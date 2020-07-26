#include "JSPathingProvider.h"

#include "pathing/TestAStar.h"

Napi::Object JSPathingProvider::Initialize(Napi::Env& env, Napi::Object& target) {
    Napi::Function constructor = DefineClass(env, "Pathing", {
          InstanceMethod("getPathToBlock", &JSPathingProvider::GetPathToBlock),
          InstanceMethod("test", &JSPathingProvider::Test),
        });
    target.Set("Pathing", constructor);
    return target;
}

JSPathingProvider::JSPathingProvider(const Napi::CallbackInfo& info) : Napi::ObjectWrap<JSPathingProvider>(info) {

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

void JSPathingProvider::Test(const Napi::CallbackInfo& info) {
    testAstar();
}
