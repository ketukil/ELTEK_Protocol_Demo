/**
 * @file crc32.c
 * @author Damjan Belavic (damjan.belavic@vuka.hr)
 * @brief 
 * @version 1.0
 * @date 2020-07-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "crc32.h"

static uint32_t *crc_lut = NULL;

/*==============================================================================
                         PRIVATE FUNCTION DECLARATIONS
==============================================================================*/

static void crc32_build_lut(uint32_t);

/*==============================================================================
                                PUBLIC FUNCTIONS
==============================================================================*/

void crc32_init(void)
{
    if (crc_lut == NULL)
    {
        crc_lut = (uint32_t *)calloc(256, sizeof(uint32_t));
    }
    /* Build CRC-32 lookup table */
    crc32_build_lut(CRC32_POLYNOMIAL);
}

void crc32_dump_table(void)
{
    const int ROWS = 32;
    const int COLUMNS = 8;
    printf("-=[ CRC-32 lookup table ]=-\n");

    for (int row = 0; row < ROWS; row++)
    {
        for (int column = 0; column < COLUMNS; column++)
        {
            printf("%08x ", crc_lut[COLUMNS * row + column]);
        }
        printf("\n");
    }
    printf("========\n");
}

uint32_t
crc32_calculate(uint8_t *buffer, size_t buffer_size)
{   
    if (crc_lut == NULL)
    {
        return 0;
    }

    /* CRC-32 initialization */
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < buffer_size; i++)
    {
        uint8_t character = buffer[i];
        uint32_t index = (character ^ crc) & 0xFF;
        crc = (crc >> 8) ^ crc_lut[index];
    }
    return ~crc;
}

/*==============================================================================
                                PRIVATE FUNCTIONS
==============================================================================*/

/**
 * @brief Generates a CRC-32 lookup table
 * 
 * @param crc_lut lookup table pointer
 * @param reversed_poly hashing polynomial
 */
void
crc32_build_lut(uint32_t reversed_poly)
{
    if (crc_lut == NULL)
    {
        return;
    }

    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t character = i;
        uint32_t crc = 0;
        for (size_t j = 0; j < 8; j++)
        {
            uint32_t bit = (character ^ crc) & 1;
            crc >>= 1;
            if (bit)
            {
                crc = crc ^ reversed_poly;
            }
            character >>= 1;
        }
        crc_lut[i] = crc;
    }
}
