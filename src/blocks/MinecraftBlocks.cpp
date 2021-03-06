#include "MinecraftBlocks.h"

namespace MinecraftBlockHolder {
	unsigned char* block_properties = new unsigned char[256]();
	float* block_hardness = new float[256]();
	void initBlockProperties() {
		block_properties[0 /* air */] = BlockProp::AIR | 0;
		block_properties[1 /* stone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[2 /* grass */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[3 /* dirt */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[4 /* cobblestone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[5 /* planks */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[6 /* sapling */] = BlockProp::TRANSPARENT | 0;
		block_properties[7 /* bedrock */] = BlockProp::SOLID | 0;
		block_properties[8 /* flowing_water */] = BlockProp::WATER | BlockProp::TRANSPARENT | 0;
		block_properties[9 /* water */] = BlockProp::WATER | BlockProp::TRANSPARENT | 0;
		block_properties[10 /* flowing_lava */] = BlockProp::TRANSPARENT | BlockProp::AVOID | 0;
		block_properties[11 /* lava */] = BlockProp::TRANSPARENT | BlockProp::AVOID | 0;
		block_properties[12 /* sand */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[13 /* gravel */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[14 /* gold_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[15 /* iron_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[16 /* coal_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[17 /* log */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[18 /* leaves */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[19 /* sponge */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[20 /* glass */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[21 /* lapis_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[22 /* lapis_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[23 /* dispenser */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[24 /* sandstone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[25 /* noteblock */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[26 /* bed */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[27 /* golden_rail */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[28 /* detector_rail */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[29 /* sticky_piston */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[30 /* web */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[31 /* tallgrass */] = BlockProp::AIR | 0;
		block_properties[32 /* deadbush */] = BlockProp::TRANSPARENT | 0;
		block_properties[33 /* piston */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[34 /* piston_head */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[35 /* wool */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[36 /* piston_extension */] = BlockProp::SOLID | 0;
		block_properties[37 /* yellow_flower */] = BlockProp::SOLID | 0;
		block_properties[38 /* red_flower */] = BlockProp::SOLID | 0;
		block_properties[39 /* brown_mushroom */] = BlockProp::SOLID | 0;
		block_properties[40 /* red_mushroom */] = BlockProp::SOLID | 0;
		block_properties[41 /* gold_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[42 /* iron_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[43 /* double_stone_slab */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[44 /* stone_slab */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[45 /* brick_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[46 /* tnt */] = BlockProp::SOLID | 0;
		block_properties[47 /* bookshelf */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[48 /* mossy_cobblestone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[49 /* obsidian */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[50 /* torch */] = BlockProp::TRANSPARENT | 0;
		block_properties[51 /* fire */] = BlockProp::SOLID | BlockProp::AVOID | 0;
		block_properties[52 /* mob_spawner */] = BlockProp::SOLID | BlockProp::DESTROYABLE | BlockProp::AVOID | 0;
		block_properties[53 /* oak_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[54 /* chest */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[55 /* redstone_wire */] = BlockProp::TRANSPARENT | 0;
		block_properties[56 /* diamond_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[57 /* diamond_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[58 /* crafting_table */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[59 /* wheat */] = BlockProp::TRANSPARENT | 0;
		block_properties[60 /* farmland */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[61 /* furnace */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[62 /* lit_furnace */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[63 /* standing_sign */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[64 /* wooden_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[65 /* ladder */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[66 /* rail */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[67 /* stone_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[68 /* wall_sign */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[69 /* lever */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[70 /* stone_pressure_plate */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[71 /* iron_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[72 /* wooden_pressure_plate */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[73 /* redstone_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[74 /* lit_redstone_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[75 /* unlit_redstone_torch */] = BlockProp::TRANSPARENT | 0;
		block_properties[76 /* redstone_torch */] = BlockProp::TRANSPARENT | 0;
		block_properties[77 /* stone_button */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[78 /* snow_layer */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[79 /* ice */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[80 /* snow */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[81 /* cactus */] = BlockProp::SOLID | BlockProp::DESTROYABLE | BlockProp::AVOID | 0;
		block_properties[82 /* clay */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[83 /* reeds */] = BlockProp::TRANSPARENT | 0;
		block_properties[84 /* jukebox */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[85 /* fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[86 /* pumpkin */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[87 /* netherrack */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[88 /* soul_sand */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[89 /* glowstone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[90 /* portal */] = BlockProp::SOLID | 0;
		block_properties[91 /* lit_pumpkin */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[92 /* cake */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[93 /* unpowered_repeater */] = BlockProp::TRANSPARENT | 0;
		block_properties[94 /* powered_repeater */] = BlockProp::TRANSPARENT | 0;
		block_properties[95 /* stained_glass */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[96 /* trapdoor */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[97 /* monster_egg */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[98 /* stonebrick */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[99 /* brown_mushroom_block */] = BlockProp::SOLID | 0;
		block_properties[100 /* red_mushroom_block */] = BlockProp::SOLID | 0;
		block_properties[101 /* iron_bars */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[102 /* glass_pane */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[103 /* melon_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[104 /* pumpkin_stem */] = BlockProp::TRANSPARENT | 0;
		block_properties[105 /* melon_stem */] = BlockProp::TRANSPARENT | 0;
		block_properties[106 /* vine */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[107 /* fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[108 /* brick_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[109 /* stone_brick_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[110 /* mycelium */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[111 /* waterlily */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[112 /* nether_brick */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[113 /* nether_brick_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[114 /* nether_brick_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[115 /* nether_wart */] = BlockProp::TRANSPARENT | 0;
		block_properties[116 /* enchanting_table */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[117 /* brewing_stand */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[118 /* cauldron */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[119 /* end_portal */] = BlockProp::SOLID | 0;
		block_properties[120 /* end_portal_frame */] = BlockProp::SOLID | 0;
		block_properties[121 /* end_stone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[122 /* dragon_egg */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[123 /* redstone_lamp */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[124 /* lit_redstone_lamp */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[125 /* double_wooden_slab */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[126 /* wooden_slab */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[127 /* cocoa */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[128 /* sandstone_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[129 /* emerald_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[130 /* ender_chest */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[131 /* tripwire_hook */] = BlockProp::TRANSPARENT | 0;
		block_properties[132 /* tripwire */] = BlockProp::TRANSPARENT | 0;
		block_properties[133 /* emerald_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[134 /* spruce_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[135 /* birch_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[136 /* jungle_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[137 /* command_block */] = BlockProp::SOLID | 0;
		block_properties[138 /* beacon */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[139 /* cobblestone_wall */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[140 /* flower_pot */] = BlockProp::SOLID | 0;
		block_properties[141 /* carrots */] = BlockProp::SOLID | 0;
		block_properties[142 /* potatoes */] = BlockProp::SOLID | 0;
		block_properties[143 /* wooden_button */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[144 /* skull */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[145 /* anvil */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[146 /* trapped_chest */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[147 /* light_weighted_pressure_plate */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[148 /* heavy_weighted_pressure_plate */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[149 /* unpowered_comparator */] = BlockProp::TRANSPARENT | 0;
		block_properties[150 /* powered_comparator */] = BlockProp::TRANSPARENT | 0;
		block_properties[151 /* daylight_detector */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[152 /* redstone_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[153 /* quartz_ore */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[154 /* hopper */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[155 /* quartz_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[156 /* quartz_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[157 /* activator_rail */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[158 /* dropper */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[159 /* stained_hardened_clay */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[160 /* stained_glass_pane */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[161 /* leaves2 */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[162 /* log2 */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[163 /* acacia_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[164 /* dark_oak_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[165 /* slime */] = BlockProp::TRANSPARENT | 0;
		block_properties[166 /* barrier */] = BlockProp::TRANSPARENT | 0;
		block_properties[167 /* iron_trapdoor */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[168 /* prismarine */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[169 /* sea_lantern */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[170 /* hay_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[171 /* carpet */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[172 /* hardened_clay */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[173 /* coal_block */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[174 /* packed_ice */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[175 /* double_plant */] = BlockProp::AIR | BlockProp::SOLID | 0;
		block_properties[176 /* standing_banner */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[177 /* wall_banner */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[178 /* daylight_detector_inverted */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[179 /* red_sandstone */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[180 /* red_sandstone_stairs */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[181 /* double_stone_slab2 */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[182 /* stone_slab2 */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[183 /* spruce_fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[184 /* birch_fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[185 /* jungle_fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[186 /* dark_oak_fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[187 /* acacia_fence_gate */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[188 /* spruce_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[189 /* birch_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[190 /* jungle_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[191 /* dark_oak_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[192 /* acacia_fence */] = BlockProp::SOLID | BlockProp::DESTROYABLE | 0;
		block_properties[193 /* spruce_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[194 /* birch_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[195 /* jungle_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[196 /* acacia_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_properties[197 /* dark_oak_door */] = BlockProp::DESTROYABLE | BlockProp::TRANSPARENT | 0;
		block_hardness[0 /* air */] = 0;
		block_hardness[1 /* stone */] = 1.5;
		block_hardness[2 /* grass */] = 0.6;
		block_hardness[3 /* dirt */] = 0.5;
		block_hardness[4 /* cobblestone */] = 2;
		block_hardness[5 /* planks */] = 2;
		block_hardness[6 /* sapling */] = 0;
		block_hardness[7 /* bedrock */] = -1;
		block_hardness[8 /* flowing_water */] = 100;
		block_hardness[9 /* water */] = 100;
		block_hardness[10 /* flowing_lava */] = 100;
		block_hardness[11 /* lava */] = 100;
		block_hardness[12 /* sand */] = 0.5;
		block_hardness[13 /* gravel */] = 0.6;
		block_hardness[14 /* gold_ore */] = 3;
		block_hardness[15 /* iron_ore */] = 3;
		block_hardness[16 /* coal_ore */] = 3;
		block_hardness[17 /* log */] = 2;
		block_hardness[18 /* leaves */] = 0.2;
		block_hardness[19 /* sponge */] = 0.6;
		block_hardness[20 /* glass */] = 0.3;
		block_hardness[21 /* lapis_ore */] = 3;
		block_hardness[22 /* lapis_block */] = 3;
		block_hardness[23 /* dispenser */] = 3.5;
		block_hardness[24 /* sandstone */] = 0.8;
		block_hardness[25 /* noteblock */] = 0.8;
		block_hardness[26 /* bed */] = 0.2;
		block_hardness[27 /* golden_rail */] = 0.7;
		block_hardness[28 /* detector_rail */] = 0.7;
		block_hardness[29 /* sticky_piston */] = 0.5;
		block_hardness[30 /* web */] = 4;
		block_hardness[31 /* tallgrass */] = 0;
		block_hardness[32 /* deadbush */] = 0;
		block_hardness[33 /* piston */] = 0.5;
		block_hardness[34 /* piston_head */] = 0.5;
		block_hardness[35 /* wool */] = 0.8;
		block_hardness[36 /* piston_extension */] = -1;
		block_hardness[37 /* yellow_flower */] = 0;
		block_hardness[38 /* red_flower */] = 0;
		block_hardness[39 /* brown_mushroom */] = 0;
		block_hardness[40 /* red_mushroom */] = 0;
		block_hardness[41 /* gold_block */] = 3;
		block_hardness[42 /* iron_block */] = 5;
		block_hardness[43 /* double_stone_slab */] = 2;
		block_hardness[44 /* stone_slab */] = 2;
		block_hardness[45 /* brick_block */] = 2;
		block_hardness[46 /* tnt */] = 0;
		block_hardness[47 /* bookshelf */] = 1.5;
		block_hardness[48 /* mossy_cobblestone */] = 2;
		block_hardness[49 /* obsidian */] = 50;
		block_hardness[50 /* torch */] = 0;
		block_hardness[51 /* fire */] = 0;
		block_hardness[52 /* mob_spawner */] = 5;
		block_hardness[53 /* oak_stairs */] = 2;
		block_hardness[54 /* chest */] = 2.5;
		block_hardness[55 /* redstone_wire */] = 0;
		block_hardness[56 /* diamond_ore */] = 3;
		block_hardness[57 /* diamond_block */] = 5;
		block_hardness[58 /* crafting_table */] = 2.5;
		block_hardness[59 /* wheat */] = 0;
		block_hardness[60 /* farmland */] = 0.6;
		block_hardness[61 /* furnace */] = 3.5;
		block_hardness[62 /* lit_furnace */] = 3.5;
		block_hardness[63 /* standing_sign */] = 1;
		block_hardness[64 /* wooden_door */] = 3;
		block_hardness[65 /* ladder */] = 0.4;
		block_hardness[66 /* rail */] = 0.7;
		block_hardness[67 /* stone_stairs */] = 2;
		block_hardness[68 /* wall_sign */] = 1;
		block_hardness[69 /* lever */] = 0.5;
		block_hardness[70 /* stone_pressure_plate */] = 0.5;
		block_hardness[71 /* iron_door */] = 5;
		block_hardness[72 /* wooden_pressure_plate */] = 0.5;
		block_hardness[73 /* redstone_ore */] = 3;
		block_hardness[74 /* lit_redstone_ore */] = 3;
		block_hardness[75 /* unlit_redstone_torch */] = 0;
		block_hardness[76 /* redstone_torch */] = 0;
		block_hardness[77 /* stone_button */] = 0.5;
		block_hardness[78 /* snow_layer */] = 0.2;
		block_hardness[79 /* ice */] = 0.5;
		block_hardness[80 /* snow */] = 0.2;
		block_hardness[81 /* cactus */] = 0.4;
		block_hardness[82 /* clay */] = 0.6;
		block_hardness[83 /* reeds */] = 0;
		block_hardness[84 /* jukebox */] = 2;
		block_hardness[85 /* fence */] = 2;
		block_hardness[86 /* pumpkin */] = 1;
		block_hardness[87 /* netherrack */] = 0.4;
		block_hardness[88 /* soul_sand */] = 0.5;
		block_hardness[89 /* glowstone */] = 0.3;
		block_hardness[90 /* portal */] = -1;
		block_hardness[91 /* lit_pumpkin */] = 1;
		block_hardness[92 /* cake */] = 0.5;
		block_hardness[93 /* unpowered_repeater */] = 0;
		block_hardness[94 /* powered_repeater */] = 0;
		block_hardness[95 /* stained_glass */] = 0.3;
		block_hardness[96 /* trapdoor */] = 3;
		block_hardness[97 /* monster_egg */] = 0.75;
		block_hardness[98 /* stonebrick */] = 1.5;
		block_hardness[99 /* brown_mushroom_block */] = 0;
		block_hardness[100 /* red_mushroom_block */] = 0;
		block_hardness[101 /* iron_bars */] = 5;
		block_hardness[102 /* glass_pane */] = 0.3;
		block_hardness[103 /* melon_block */] = 1;
		block_hardness[104 /* pumpkin_stem */] = 0;
		block_hardness[105 /* melon_stem */] = 0;
		block_hardness[106 /* vine */] = 0.2;
		block_hardness[107 /* fence_gate */] = 2;
		block_hardness[108 /* brick_stairs */] = 2;
		block_hardness[109 /* stone_brick_stairs */] = 1.5;
		block_hardness[110 /* mycelium */] = 0.6;
		block_hardness[111 /* waterlily */] = 0.6;
		block_hardness[112 /* nether_brick */] = 2;
		block_hardness[113 /* nether_brick_fence */] = 2;
		block_hardness[114 /* nether_brick_stairs */] = 2;
		block_hardness[115 /* nether_wart */] = 0;
		block_hardness[116 /* enchanting_table */] = 5;
		block_hardness[117 /* brewing_stand */] = 0.5;
		block_hardness[118 /* cauldron */] = 2;
		block_hardness[119 /* end_portal */] = -1;
		block_hardness[120 /* end_portal_frame */] = -1;
		block_hardness[121 /* end_stone */] = 3;
		block_hardness[122 /* dragon_egg */] = 3;
		block_hardness[123 /* redstone_lamp */] = 0.3;
		block_hardness[124 /* lit_redstone_lamp */] = 0.3;
		block_hardness[125 /* double_wooden_slab */] = 2;
		block_hardness[126 /* wooden_slab */] = 2;
		block_hardness[127 /* cocoa */] = 0.2;
		block_hardness[128 /* sandstone_stairs */] = 0.8;
		block_hardness[129 /* emerald_ore */] = 3;
		block_hardness[130 /* ender_chest */] = 22.5;
		block_hardness[131 /* tripwire_hook */] = 0;
		block_hardness[132 /* tripwire */] = 0;
		block_hardness[133 /* emerald_block */] = 5;
		block_hardness[134 /* spruce_stairs */] = 2;
		block_hardness[135 /* birch_stairs */] = 2;
		block_hardness[136 /* jungle_stairs */] = 2;
		block_hardness[137 /* command_block */] = -1;
		block_hardness[138 /* beacon */] = 3;
		block_hardness[139 /* cobblestone_wall */] = 2;
		block_hardness[140 /* flower_pot */] = 0;
		block_hardness[141 /* carrots */] = 0;
		block_hardness[142 /* potatoes */] = 0;
		block_hardness[143 /* wooden_button */] = 0.5;
		block_hardness[144 /* skull */] = 1;
		block_hardness[145 /* anvil */] = 5;
		block_hardness[146 /* trapped_chest */] = 2.5;
		block_hardness[147 /* light_weighted_pressure_plate */] = 0.5;
		block_hardness[148 /* heavy_weighted_pressure_plate */] = 0.5;
		block_hardness[149 /* unpowered_comparator */] = 0;
		block_hardness[150 /* powered_comparator */] = 0;
		block_hardness[151 /* daylight_detector */] = 0.2;
		block_hardness[152 /* redstone_block */] = 5;
		block_hardness[153 /* quartz_ore */] = 3;
		block_hardness[154 /* hopper */] = 3;
		block_hardness[155 /* quartz_block */] = 0.8;
		block_hardness[156 /* quartz_stairs */] = 0.8;
		block_hardness[157 /* activator_rail */] = 0.7;
		block_hardness[158 /* dropper */] = 3.5;
		block_hardness[159 /* stained_hardened_clay */] = 1.25;
		block_hardness[160 /* stained_glass_pane */] = 0.3;
		block_hardness[161 /* leaves2 */] = 0.2;
		block_hardness[162 /* log2 */] = 2;
		block_hardness[163 /* acacia_stairs */] = 2;
		block_hardness[164 /* dark_oak_stairs */] = 2;
		block_hardness[165 /* slime */] = 0;
		block_hardness[166 /* barrier */] = -1;
		block_hardness[167 /* iron_trapdoor */] = 5;
		block_hardness[168 /* prismarine */] = 1.5;
		block_hardness[169 /* sea_lantern */] = 0.3;
		block_hardness[170 /* hay_block */] = 0.5;
		block_hardness[171 /* carpet */] = 0.1;
		block_hardness[172 /* hardened_clay */] = 1.25;
		block_hardness[173 /* coal_block */] = 5;
		block_hardness[174 /* packed_ice */] = 0.5;
		block_hardness[175 /* double_plant */] = 0;
		block_hardness[176 /* standing_banner */] = 1;
		block_hardness[177 /* wall_banner */] = 1;
		block_hardness[178 /* daylight_detector_inverted */] = 0.2;
		block_hardness[179 /* red_sandstone */] = 0.8;
		block_hardness[180 /* red_sandstone_stairs */] = 0.8;
		block_hardness[181 /* double_stone_slab2 */] = 2;
		block_hardness[182 /* stone_slab2 */] = 2;
		block_hardness[183 /* spruce_fence_gate */] = 2;
		block_hardness[184 /* birch_fence_gate */] = 2;
		block_hardness[185 /* jungle_fence_gate */] = 2;
		block_hardness[186 /* dark_oak_fence_gate */] = 2;
		block_hardness[187 /* acacia_fence_gate */] = 2;
		block_hardness[188 /* spruce_fence */] = 2;
		block_hardness[189 /* birch_fence */] = 2;
		block_hardness[190 /* jungle_fence */] = 2;
		block_hardness[191 /* dark_oak_fence */] = 2;
		block_hardness[192 /* acacia_fence */] = 2;
		block_hardness[193 /* spruce_door */] = 3;
		block_hardness[194 /* birch_door */] = 3;
		block_hardness[195 /* jungle_door */] = 3;
		block_hardness[196 /* acacia_door */] = 3;
		block_hardness[197 /* dark_oak_door */] = 3;
	}
}