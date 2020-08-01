#pragma once

#include <napi.h>
#include <ctime>
#include "je/AnvilChunkColumn.h"
#include "pathing/PathingProvider.h"

class JSPathingProvider : public Napi::ObjectWrap<JSPathingProvider> {
private:
    struct XYZ {
        int x;
        int y;
        int z;
    };
    std::vector<XYZ> denyBlocks;
public:
    static Napi::Object Initialize(Napi::Env& env, Napi::Object& target);

    JSPathingProvider(const Napi::CallbackInfo& info); // Constructor

    Napi::Value GetPathToBlockOld(const Napi::CallbackInfo& info);

    Napi::Value GetPathToBlock(const Napi::CallbackInfo& info);

    // This is called when we run into an obstruction while executing a path
    // It marks the blocks as AVOID and a new path is generated.
    Napi::Value SetDenyBlock(const Napi::CallbackInfo& info);

    void Test(const Napi::CallbackInfo& info);
    
    //Napi::Value PathToEntity(const Napi::CallbackInfo& info);
    //Napi::Value GetBlockSkyLight(const Napi::CallbackInfo& info);
    //Napi::Value GetBlockBlockLight(const Napi::CallbackInfo& info);
    //Napi::Value GetBlockBiome(const Napi::CallbackInfo& info);

    //Napi::Value DoSomething(const Napi::CallbackInfo& info);

    /*~JSPathingProvider() {
        delete column;
    }*/

};

// https://github.com/nodejs/node-addon-api/blob/master/doc/async_worker.md
// This does not run in a new thread, rather it waits for an exisiting
// node.js thread to become avaliable and executes the task on that thread.
// This does not block up the main JS loop.

class JSPathingWorker : public Napi::AsyncWorker {
private:
    PathingRequest request;
    clock_t startTime;
    PathingProvider::ExecutableMoves actions;
public:
    JSPathingWorker(Napi::Function& callback, clock_t runTime);

    inline void setRequest(PathingRequest request) {
        this->request = request;
    }
    
    void Execute();
    void OnOK();

    virtual ~JSPathingWorker() {};
private:
    int runTime;
};
