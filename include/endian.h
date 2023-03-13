#ifndef ENDIAN_H
#define ENDIAN_H

#define __bswap16(x) ((uint16_t)((((uint16_t)(x)&0xff00) >> 8) | (((uint16_t)(x)&0x00ff) << 8)))

#define __bswap32(x)                                                                     \
	((uint32_t)((((uint32_t)(x)&0xff000000) >> 24) | (((uint32_t)(x)&0x00ff0000) >> 8) | \
				(((uint32_t)(x)&0x0000ff00) << 8) | (((uint32_t)(x)&0x000000ff) << 24)))

#define be32toh(x) __bswap32(x)
#define be16toh(x) __bswap16(x)

#endif // ENDIAN_H