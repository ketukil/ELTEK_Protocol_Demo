/**
 * @file crc32.h
 * @author Damjan Belavic (damjan.belavic@vuka.hr)
 * @brief 
 * @version 1.0
 * @date 2020-07-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

/* Desired reverse polynomial */
#define CRC32_POLYNOMIAL 0xEDB88320L

void crc32_init(void);
void crc32_dump_table(void);
uint32_t crc32_calculate(uint8_t *, size_t);

#endif /* CRC32_H */
