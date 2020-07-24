import json

blocks = []
blocks2 = []
# 
def set_bit(v, index, x):
  """Set the index:th bit of v to 1 if x is truthy, else to 0, and return the new value."""
  mask = 1 << index   # Compute mask, an integer with just bit 'index' set.
  v &= ~mask          # Clear the bit indicated by the mask (if x is False)
  if x:
    v |= mask         # If x was True, set the bit indicated by the mask.
  return v            # Return the result, we're done.

with open('blocks.json', 'r') as f:
    data = json.loads(f.read())

    for block in data:
        name = block['name']

        is_air = False
        is_solid = False
        is_destroyable = False
        is_water = False
        is_unused = False
        is_transparent = False
        is_avoid = False
        is_adjsolid = False

        if name == 'air':
            is_air = True
        elif 'water' in name:
            is_water = True

        if block['transparent']:
            is_transparent = True
        else:
            is_solid = True

        if block['hardness']:
            if block['hardness'] > 0 and block['hardness'] < 60:
                is_destroyable = True
            
        blocks.append([block['name'], is_air, is_solid, is_destroyable, is_water, is_unused, is_transparent, is_adjsolid])

        val = 0

        if is_air:
            val |= 1 << 0
        if is_solid:
            val |= 1 << 1
        if is_destroyable:
            val |= 1 << 2
        if is_water:
            val |= 1 << 3
        if is_unused:
            val |= 1 << 4
        if is_transparent:
            val |= 1 << 5
        if is_avoid:
            val |= 1 << 6
        if is_adjsolid:
            val |= 1 << 7

        s = ''
        if is_air:
            s += "BlockProp::AIR | "
        if is_solid:
            s += "BlockProp::SOLID | "
        if is_destroyable:
            s += "BlockProp::DESTROYABLE | "
        if is_water:
            s += "BlockProp::WATER | "
        if is_unused:
            s += "BlockProp::UNUSED | "
        if is_transparent:
            s += "BlockProp::TRANSPARENT | "
        if is_avoid:
            s += "BlockProp::AVOID | "
        if is_adjsolid:
            s += "BlockProp::ADJ_SOLID | "
        s += "0"

        print val
        blocks2.append([block['id'], block['name'], val, s])

        
# static UNKNOWN = 0;
# static AIR = 0b1;
# static SOLID = 0b10;
# static DESTROYABLE = 0b100;
# static WATER = 0b1000;
# static UNUSED = 0b10000;
# static TRANSPARENT = 0b100000;
# static AVOID = 0b1000000;
# static ADJ_SOLID = 0b10000000;

print 'unsigned char *block_properties = new unsigned char[256]();'
print 'void initBlockProperties() {'
for block in blocks2:
    print 'block_properties[%d /* %s */] = %s;' % (block[0], block[1], block[3])
print '}'
print blocks

