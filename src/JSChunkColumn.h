#pragma once

#include <napi.h>
#include "je/AnvilChunkColumn.h"

class JSChunkColumn : public Napi::ObjectWrap<JSChunkColumn> {
private:
    AnvilChunkColumn* column = 0;
public:
    static Napi::Object Initialize(Napi::Env& env, Napi::Object& target);

    JSChunkColumn(const Napi::CallbackInfo& info); // Constructor
    Napi::Value Load(const Napi::CallbackInfo& info);

    Napi::Value GetBlockID(const Napi::CallbackInfo& info);
    Napi::Value GetBlockMetadata(const Napi::CallbackInfo& info);
    Napi::Value GetBlockSkyLight(const Napi::CallbackInfo& info);
    Napi::Value GetBlockBlockLight(const Napi::CallbackInfo& info);
    Napi::Value GetBlockBiome(const Napi::CallbackInfo& info);


    void SetBlockID(const Napi::CallbackInfo& info);
    void SetBlockMetadata(const Napi::CallbackInfo& info);
    void SetBlockBiome(const Napi::CallbackInfo& info);
    void SetBlockStateId(const Napi::CallbackInfo& info);

    ~JSChunkColumn() {
        delete column;
    }

};