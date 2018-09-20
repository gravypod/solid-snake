#ifndef ENGINE_UTIL_BITS_H
#define ENGINE_UTIL_BITS_H

#define IS_BIT_SET(number, index) (number & (1 << index))
#define IS_EVERY_BIT_SET(number, mask) ((number & mask) == mask)

#endif
