
/**
 * @file protocol.h
 * @author Damjan Belavic (damjan.belavic@vuka.hr)
 * @brief 
 * @version 1.0
 * @date 2020-07-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PACKED __attribute__((__packed__))

/* Globally available serialization buffer */
char *serialized_message;

typedef struct PACKED protocol_message
{
    uint32_t address;
    uint16_t size;
    const char *data;
    uint32_t crc;
} protocol_message_t;

#define ASCII_HEAD_LEN ((sizeof(uint32_t) + sizeof(uint16_t)) * 2)
#define ASCII_TAIL_LEN (sizeof(uint32_t) + 2)

void protocol_message_send(protocol_message_t *);
int protocol_message_read(protocol_message_t *, const char *);
void protocol_dump_message(protocol_message_t *);

#endif /* PROTOCOL_H */
