/*
 * crc.h
 *
 *  Created on: 2011-04-03
 *      Author: Isaac Tepper
 */

#ifndef CRC_H_
#define CRC_H_

typedef unsigned short crc16;
typedef unsigned int crc32;
typedef crc16 crc16_table[256];
typedef crc32 crc32_table[256];

typedef struct {
	crc16 polynomial;
	crc16 initial_xor;
	crc16 final_xor;
	unsigned char reflect_data;
	unsigned char reflect_remainder;
	crc16_table table;
} crc16_algorithm;

typedef struct {
	crc32 polynomial;
	crc32 initial_xor;
	crc32 final_xor;
	unsigned char reflect_data;
	unsigned char reflect_remainder;
	crc32_table table;
} crc32_algorithm;

void crc16_create_table(crc16_algorithm *algorithm);
void crc32_create_table(crc32_algorithm *algorithm);
crc16 crc16_calc(crc16_algorithm *algorithm, void *data, int size);
crc32 crc32_calc(crc32_algorithm *algorithm, void *data, int size);

#endif /* CRC_H_ */
