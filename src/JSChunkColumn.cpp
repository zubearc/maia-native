#include "JSChunkColumn.h"

const auto sectionCount = 16;
const auto width = 16;
const auto length = 16;

#ifndef _ASSERT
#define _ASSERT
#endif

Napi::Object JSChunkColumn::Initialize(Napi::Env& env, Napi::Object& target) {
    Napi::Function constructor = DefineClass(env, "Chunk", {
        //InstanceAccessor("value", &GetSomething, &SetSomething),
          InstanceMethod("load", &JSChunkColumn::Load),

          InstanceMethod("getBlockID", &JSChunkColumn::GetBlockID),
          InstanceMethod("getBlockMetadata", &JSChunkColumn::GetBlockMetadata),
          InstanceMethod("getBlockBiome", &JSChunkColumn::GetBlockBiome),
          InstanceMethod("getBlockLight", &JSChunkColumn::GetBlockBlockLight),
          InstanceMethod("getSkyLight", &JSChunkColumn::GetBlockSkyLight),


          InstanceMethod("setBlockID", &JSChunkColumn::SetBlockID),
          InstanceMethod("setBlockStateID", &JSChunkColumn::SetBlockStateId),
          InstanceMethod("setBlockMetadata", &JSChunkColumn::SetBlockMetadata),
          InstanceMethod("setBlockBiome", &JSChunkColumn::SetBlockBiome),
          //InstanceMethod("setBlockLight", &SetBlockLight),
          //InstanceMethod("setSkyLight", &SetSkyLight),
        });
    target.Set("Chunk", constructor);
    return target;
}

JSChunkColumn::JSChunkColumn(const Napi::CallbackInfo& info) : Napi::ObjectWrap<JSChunkColumn>(info) {
    Napi::Env env = info.Env();
    // ...
    //Napi::Number value = info[0].As<Napi::Number>();
    std::vector<JChunkSection*> sections;
    this->column = new AnvilChunkColumn(0, 0, sections);
}

std::vector<int> getChunksCount(int bitmap) {
    std::vector<int> chunks;
    for (auto y = 0; y < sectionCount; ++y) {
        auto yes = bitmap & (1 << y);
        if (yes) chunks.push_back(y);
    }
    return chunks;
}

Napi::Value JSChunkColumn::Load(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    //printf("#0\n");

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

    //printf("#1\n");

    std::string loadedStr;

    for (int sectioni = 0; sectioni < 16; sectioni++) {
        for (auto chunkSent : chunksSent) {
            if (chunkSent == sectioni) {
                loadedStr += std::to_string(sectioni) + ", ";
                //printf("Reading chunk section %d\n", sectioni);
                auto section = new JChunkSection18(sectioni);
                //printf("Created %d\n", sectioni);
                //const sectionBuffer = Buffer.alloc(SECTION_SIZE)
                //offset += data.copy(sectionBuffer, 0, offset, offset + w * l * sh * 2)
                // offsetLight += data.copy(sectionBuffer, w * l * sh * 2, offsetLight, offsetLight + w * l * sh / 2)
                //if (this.skyLightSent) offsetSkyLight += data.copy(sectionBuffer, w * l * sh * 5 / 2, offsetLight, offsetSkyLight + w * l * sh / 2)
                    //this.sections[i].load(sectionBuffer, skyLightSent)

                //memcpy(section->blocks.data(), data.Data() + offset, 16 * 16 * 16 * 2);
                section->readBlocksFromNetwork(data.Data() + offset, 16 * 16 * 16 * 2);
                offset += 16 * 16 * 16 * 2;
                //printf("Read Blocks %d\n", offset);
                memcpy(section->getBlockLightBuffer(), data.Data() + offsetLight, 16 * 16 * 8);
                offsetLight += 16 * 16 * 8;
                //printf("Read blocklight %d, %d\n", offsetLight);
                if (offsetSkyLight) {
                    //printf("Buffr ptr: %p\n", section->getSkyLightBuffer());
                    memcpy(section->getSkyLightBuffer(), data.Data() + offsetSkyLight, 16 * 16 * 8);
                    offsetSkyLight += 16 * 16 * 8;
                }
                //printf("Read skylight %d\n", offsetSkyLight);
                sections.push_back(section);
                //printf("Done %d\n", offset);
            }
        }
    }

    //printf("[%d, %d] Loaded %s sections\n", chunkx, chunkz, loadedStr.c_str());

    for (int i = 0; i < sections.size(); i++) {
        auto next = (i + 1) < sections.size() ? sections[i + 1] : nullptr;
        auto current = sections[i];
        if (next) {
            if (next->getY() < current->getY()) {
                printf("ERROR!\n", sections.size());
                _ASSERT(false);
            }
        }
    }

    //printf("#2\n");

    this->column->x = chunkx;
    this->column->z = chunkz;
    this->column->setChunkSections(sections);

    if (fullChunk) {
        this->column->readBiomesFromNetwork(data.Data() + offset, 256);
        //data.copy(this.biome, 16 * 16 * sectionCount * chunkCount * (sentSkylight ? 3 : 5 / 2))
    }

    //printf("#3\n");

    auto sectionSize = (16 * 16 * 16) * (sentSkylight ? 3 : 5 / 2);

    auto expectedSize = sectionSize * chunkCount + (fullChunk ? 16 * 16 : 0);
    if (data.Length() != expectedSize) {
        Napi::TypeError::New(env, "Data buffer not correct size(was " + std::to_string(data.Length()) + ", expected " + std::to_string(expectedSize)).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    //printf("Good data buffer!\n");

    return Napi::Boolean::New(env, true);
}

Napi::Value JSChunkColumn::GetBlockID(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();


    auto ret = this->column->getBlockId(x, y, z);
    return Napi::Number::New(env, ret);
}

Napi::Value JSChunkColumn::GetBlockMetadata(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();


    auto ret = this->column->getBlockMetadata(x, y, z);
    return Napi::Number::New(env, ret);
}

Napi::Value JSChunkColumn::GetBlockSkyLight(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();


    auto ret = this->column->getSkyLight(x, y, z);
    return Napi::Number::New(env, ret);

}

Napi::Value JSChunkColumn::GetBlockBlockLight(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();


    auto ret = this->column->getBlockLight(x, y, z);
    return Napi::Number::New(env, ret);
}

Napi::Value JSChunkColumn::GetBlockBiome(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int z = info[1].As<Napi::Number>().Int32Value();


    auto ret = this->column->getBiome(x, z);
    return Napi::Number::New(env, ret);
}

void JSChunkColumn::SetBlockID(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();
    int val = info[3].As<Napi::Number>().Int32Value();
    //printf("SetBlockState %d %d %d --> %d\n", x, y, z, val);

    this->column->setBlockId(x, y, z, val);
}

void JSChunkColumn::SetBlockMetadata(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();
    int val = info[3].As<Napi::Number>().Int32Value();

    this->column->setBlockMetadata(x, y, z, val);
}

void JSChunkColumn::SetBlockBiome(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int z = info[1].As<Napi::Number>().Int32Value();
    int val = info[2].As<Napi::Number>().Int32Value();

    this->column->setBiome(x, z, val);
}

void JSChunkColumn::SetBlockStateId(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int z = info[2].As<Napi::Number>().Int32Value();
    int val = info[3].As<Napi::Number>().Int32Value();

    this->column->setBlockStateId(x, y, z, val);
}

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    JSChunkColumn::Initialize(env, exports);
    return exports;
}

NODE_API_MODULE(ChunkColumn, Init);