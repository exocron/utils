/*
 * crc.c
 *
 *  Created on: 2011-04-03
 *      Author: Isaac Tepper
 */

#include "crc.h"

#define reflect8(x) ({\
	unsigned char _result = x;\
	_result = ((_result & 0xAA) >> 1) | ((_result & 0x55) << 1);\
	_result = ((_result & 0xCC) >> 2) | ((_result & 0x33) << 2);\
	_result = ((_result & 0xF0) >> 4) | ((_result & 0x0F) << 4);\
	_result;\
})

#define reflect16(x) ({\
	unsigned short _result = x;\
	_result = ((_result & 0xAAAA) >> 1) | ((_result & 0x5555) << 1);\
	_result = ((_result & 0xCCCC) >> 2) | ((_result & 0x3333) << 2);\
	_result = ((_result & 0xF0F0) >> 4) | ((_result & 0x0F0F) << 4);\
	_result = ((_result & 0xFF00) >> 8) | ((_result & 0x00FF) << 8);\
	_result;\
})

#define reflect32(x) ({\
	unsigned int _result = x;\
	_result = ((_result & 0xAAAAAAAA) >> 1) | ((_result & 0x55555555) << 1);\
	_result = ((_result & 0xCCCCCCCC) >> 2) | ((_result & 0x33333333) << 2);\
	_result = ((_result & 0xF0F0F0F0) >> 4) | ((_result & 0x0F0F0F0F) << 4);\
	_result = ((_result & 0xFF00FF00) >> 8) | ((_result & 0x00FF00FF) << 8);\
	_result = ((_result & 0xFFFF0000) >> 16) | ((_result & 0x0000FFFF) << 16);\
	_result;\
})

void crc16_create_table(crc16_algorithm *algorithm) {
	unsigned char bit;
	crc16 remainder;
	int dividend;

	for (dividend = 0; dividend < 256; dividend++) {
		if (algorithm->reflect_data)
			remainder = reflect8(dividend) << 8;
		else
			remainder = dividend << 8;
		for (bit = 8; bit > 0; --bit) {
			if (remainder & 0x8000)
				remainder = (remainder << 1) ^ algorithm->polynomial;
			else
				remainder <<= 1;
		}
		if (algorithm->reflect_remainder)
			algorithm->table[dividend] = reflect16(remainder);
		else
			algorithm->table[dividend] = remainder;
	}
}

void crc32_create_table(crc32_algorithm *algorithm) {
	unsigned char bit;
	crc32 remainder;
	int dividend;

	for (dividend = 0; dividend < 256; dividend++) {
		if (algorithm->reflect_data)
			remainder = reflect8(dividend) << 8;
		else
			remainder = dividend << 8;
		for (bit = 8; bit > 0; --bit) {
			if (remainder & 0x80000000)
				remainder = (remainder << 1) ^ algorithm->polynomial;
			else
				remainder <<= 1;
		}
		if (algorithm->reflect_remainder)
			algorithm->table[dividend] = reflect32(remainder);
		else
			algorithm->table[dividend] = remainder;
	}
}

crc16 crc16_calc(crc16_algorithm *algorithm, void *data, int size) {
	unsigned char *buffer = (unsigned char *)data;
	unsigned char byte;
	int i;
	crc16 remainder = algorithm->initial_xor; /* 0 ^ initial_xor; haha, get it? */
	for (i = 0; i < size; i++) {
		byte = (buffer[i] ^ remainder) & 0xFF;
		remainder = algorithm->table[byte] ^ (remainder >> 8);
	}
	return remainder ^ algorithm->final_xor;
}

crc32 crc32_calc(crc32_algorithm *algorithm, void *data, int size) {
	unsigned char *buffer = (unsigned char *)data;
	unsigned char byte;
	int i;
	crc32 remainder = algorithm->initial_xor; /* no? it's because it's the initial VALUE */
	for (i = 0; i < size; i++) {
		byte = (buffer[i] ^ remainder) & 0xFF;
		remainder = algorithm->table[byte] ^ (remainder >> 8);
	}
	return remainder ^ algorithm->final_xor;
}

crc16 crc16_calc_continue(crc16_algorithm *algorithm, void *data, int size, int start) {
	unsigned char *buffer = (unsigned char *)data;
	unsigned char byte;
	int i;
	crc16 remainder = algorithm->initial_xor ^ start; /* this function is to resume a CRC */
	for (i = 0; i < size; i++) {
		byte = (buffer[i] ^ remainder) & 0xFF;
		remainder = algorithm->table[byte] ^ (remainder >> 8);
	}
	return remainder ^ algorithm->final_xor;
}

crc32 crc32_calc_continue(crc32_algorithm *algorithm, void *data, int size, int start) {
	unsigned char *buffer = (unsigned char *)data;
	unsigned char byte;
	int i;
	crc32 remainder = algorithm->initial_xor ^ start;
	for (i = 0; i < size; i++) {
		byte = (buffer[i] ^ remainder) & 0xFF;
		remainder = algorithm->table[byte] ^ (remainder >> 8);
	}
	return remainder ^ algorithm->final_xor;
}
