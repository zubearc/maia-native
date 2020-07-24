#pragma once

#include <map>
#include <napi.h>
//#include "je/AnvilChunkColumn.h"
#include "JSChunkColumn.h"
#include "JSPathingProvider.h"

/*
addColumn({
        x: meta.x,
        z: meta.z,
        bitMap: meta.bitMap,
        heightmaps: packet.heightmaps,
        skyLightSent: packet.skyLightSent,
        groundUp: true,
        data: packet.data.slice(offset, offset + size)
      })

*/
auto sectionCount = 16;
auto width = 16;
auto length = 16;

namespace MaiaChunk {
    Napi::String Method(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        return Napi::String::New(env, "world");
    }

    std::vector<int> getChunksCount(int bitmap) {
        std::vector<int> chunks;
        for (auto y = 0; y < sectionCount; ++y) {
            auto yes = bitmap & (1 << y);
            if (yes) chunks.push_back(y);
        }
        return chunks;
    }
#if 0
    Napi::Boolean AddChunkColumn(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        if (info.Length() < 7) {
            Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }

        if (!info[0].IsNumber() || !info[1].IsNumber()) {
            Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }

        int chunkx = info[0].As<Napi::Number>().Int32Value();
        int chunkz = info[1].As<Napi::Number>().Int32Value();
        double bitmap = info[2].As<Napi::Number>().Int32Value();
        double sentSkylight = info[3].As<Napi::Boolean>().ToBoolean();
        double fullChunk = info[4].As<Napi::Boolean>().ToBoolean();
        auto data = info[5].As<Napi::Buffer<uint8_t>>();

        //Napi::Number num = Napi::Number::New(env, arg0 + arg1);

        std::vector<JChunkSection*> sections;

        auto chunksSent = getChunksCount(bitmap);
        auto chunkCount = chunksSent.size();

        auto offset = 0;
        auto offsetLight = width * length * sectionCount * chunkCount * 2;
        auto offsetSkyLight = (sentSkylight) ? width * length * sectionCount * chunkCount / 2 * 5 : 0;

        for (int sectioni = 0; sectioni < 16; sectioni++) {
            for (auto chunkSent : chunksSent) {
                if (chunkSent == sectioni) {
                    auto section = new JChunkSection18(sectioni);
                    //const sectionBuffer = Buffer.alloc(SECTION_SIZE)
                    //offset += data.copy(sectionBuffer, 0, offset, offset + w * l * sh * 2)
                    // offsetLight += data.copy(sectionBuffer, w * l * sh * 2, offsetLight, offsetLight + w * l * sh / 2)
                    //if (this.skyLightSent) offsetSkyLight += data.copy(sectionBuffer, w * l * sh * 5 / 2, offsetLight, offsetSkyLight + w * l * sh / 2)
                        //this.sections[i].load(sectionBuffer, skyLightSent)
                    
                    //memcpy(section->blocks.data(), data.Data() + offset, 16 * 16 * 16 * 2);
                    section->readBlocksFromNetwork(data.Data(), 16 * 16 * 16 * 2);
                    offset += 16 * 16 * 16 * 2;
                    memcpy(section->getBlockLightBuffer(), data.Data() + offset, 16 * 16 * 8);
                    offset += 16 * 16 * 8;
                    if (offsetSkyLight) {
                        memcpy(section->getSkyLightBuffer(), data.Data() + offset, 16 * 16 * 8);
                        offset += 16 * 16 * 8;
                    }

                    sections.push_back(section);
                }
            }
        }

        auto chunkColumn = new AnvilChunkColumn(chunkx, chunkz, sections);

        if (fullChunk) {
            chunkColumn->readBiomesFromNetwork(data.Data() + offset, 256);
            //data.copy(this.biome, 16 * 16 * sectionCount * chunkCount * (sentSkylight ? 3 : 5 / 2))
        }

        auto expectedSize = 16 * 16 * 16 * chunkCount + (fullChunk ? 16 * 16 : 0);
        if (data.Length() != expectedSize) { 
            Napi::TypeError::New(env, "Data buffer not correct size(was " + std::to_string(data.Length()) + ", expected " + std::to_string(expectedSize)).ThrowAsJavaScriptException();
            return Napi::Boolean::New(env, false);
        }

        return Napi::Boolean::New(env, true);
    }
#endif
    Napi::Object Init(Napi::Env env, Napi::Object exports) {
        exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Method));

        //exports.Set(Napi::String::New(env, "addChunkColumn"), Napi::Function::New(env, AddChunkColumn));


        JSChunkColumn::Initialize(env, exports);
        JSPathingProvider::Initialize(env, exports);

        return exports;
    }



    NODE_API_MODULE(MaiaChunk, Init);
}