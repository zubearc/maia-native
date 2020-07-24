#pragma once

#include <napi.h>
#include "je/AnvilChunkColumn.h"

class JSPathingProvider : public Napi::ObjectWrap<JSPathingProvider> {
private:

public:
    static Napi::Object Initialize(Napi::Env& env, Napi::Object& target);

    JSPathingProvider(const Napi::CallbackInfo& info); // Constructor

    Napi::Value GetPathToBlock(const Napi::CallbackInfo& info);

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