var addon = require("./" + process.arch + "/MaiaNative");
var MaiaNative = addon;
//var addon2 = require("./" + process.arch + "/MaiaChunk");

const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('Press enter to exit\n\n', (answer) => {
    rl.close();
});

console.log(addon.hello());

//let chunk = new addon.Chunk();

//chunk.setBlockID(0,0,0,1);
//chunk.setBlockMetadata(0,0,0,2);

//console.log(addon, chunk, chunk.getBlockID(0,0,0), chunk.getBlockMetadata(0,0,0));


////

function runBlockTests() {
    class Block {
    constructor() {}
    };

    const exists = function (val) {
        return val !== undefined
    }

    MaiaNative.Chunk.prototype.getBlock = function(pos) {
        const block = new Block(this.getBlockID(pos.x, pos.y, pos.z), this.getBlockBiome(pos.x, pos.y, pos.z), this.getBlockMetadata(pos.x, pos.y, pos.z))
        block.light = this.getBlockLight(pos.x, pos.y, pos.z)
        block.skyLight = this.getSkyLight(pos.x, pos.y, pos.z)
        return block
    }

    MaiaNative.Chunk.prototype.setBlock = function(pos, block) {
        if (exists(block.type)) { this.getBlockID(pos.x, pos.y, pos.z, block.type) }
        if (exists(block.metadata)) { this.setBlockMetadata(pos.x, pos.y, pos.z, block.metadata) }
        if (exists(block.biome)) { this.setBlockBiome(pos.x, pos.y, pos.z, block.biome.id) }
        // if (exists(block.skyLight)) { this.setSkyLight(pos.x, pos.y, pos.z, block.skyLight) }
        // if (exists(block.light)) { this.setBlockLight(pos.x, pos.y, pos.z, block.light) }
    }


    let chunk2 = new MaiaNative.Chunk();
    //chunk2.setBlock({ x: 0, y: 0, z: 0 }, { type: 0, metadata: 0 })

    var chunkData = require('./CHUNK12_13.json');
    let buffer = Buffer.from(chunkData.data);
    //console.log(chunkData, buffer);
    let ok = chunk2.load(chunkData.x, chunkData.z, chunkData.bitMap, chunkData.skyLightSent, chunkData.groundUp, buffer);
    let bid = chunk2.getBlockID(0,0,0);

    chunk2.getBlockID(12,32,12);
    chunk2.getBlockID(12,16,12);
    chunk2.getBlockID(12,2,12);

    let block = chunk2.getBlockID(6,62,4);

    console.log(block);

    //console.log(ok, bid);
    //console.log(ok, bid);
    //chunk2.load()

    return chunk2;
}

var chunk2 = runBlockTests();

function runPathingTests() {
    let pathing = new MaiaNative.Pathing();
    let ret = pathing.getPathToBlock(198, 63, 211, 202, 63, 210);
    console.log(ret);
    //pathing.test();
}

runPathingTests();