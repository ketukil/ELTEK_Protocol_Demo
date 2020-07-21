/**
 * @file protocol.c
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
#include <stddef.h>
#include <string.h>

#include "crc32.h"
#include "protocol.h"

/*==============================================================================
                         PRIVATE FUNCTION DECLARATIONS
==============================================================================*/

static uint32_t     parse_msg_address(const char *);
static int          parse_msg_data_length(const char *);
static const char*  parse_msg_payload(const char *, int);
static uint32_t     parse_msg_crc(const char *, int);

/*==============================================================================
                                PUBLIC FUNCTIONS
==============================================================================*/

/**
 * @brief Send message to slave device
 * 
 * @param address Address on slave device
 * @param data String of data
 */
void protocol_message_send(protocol_message_t *msg)
{
    uint32_t crc;
    int length;

    /* get length of the message payload */
    length = strlen(msg->data);
    /* allocate enough memory for message buffer */
    serialized_message = calloc((ASCII_HEAD_LEN + length + ASCII_TAIL_LEN), sizeof(char));
    /* Fill buffer with message header and payload */
    sprintf(serialized_message, ":%08x%04x%s", msg->address, length, msg->data);
    /* Adjust new length */
    length += ASCII_HEAD_LEN + 1;
    /* Calculate whole message CRC-32 */
    crc = crc32_calculate((uint8_t *)serialized_message, length);
    /* Append CRC-32 and ending to buffer*/
    sprintf(serialized_message + length, "%08x\r\n", crc);
}

/**
 * @brief Reads message form buffer to the message data structure
 * 
 * @param message Destination message structure
 * @param data Source message ASCII buffer
 * @return int -1 on error
 */
int protocol_message_read(protocol_message_t *message, const char *data)
{
    // :[address],[len],[payload],[crc32],[end]
    // :deadbeef,0019,SomeDataPayloadOfSomeKind,daa16065,\r\n
    // :deadbeef0019SomeDataPayloadOfSomeKinddaa16065 len=46 + 2

    uint32_t msg_crc, actual_crc;
    uint32_t msg_address;
    uint16_t msg_length;
    const char *msg_data = NULL;

    /* Check if message start character is ok */
    if (*data != ':')
    {
        return -1;
    }
    /* Get message length */
    int len = strlen(data);

    /* Check if end of the message is ok */
    if (strcmp((data + len - 2), "\r\n") != 0)
    {
        return -2;
    }

    /* Do a CRC-32 check */
    msg_crc = parse_msg_crc(data, len);
    actual_crc = crc32_calculate((uint8_t *)data, len - 10);
    if (msg_crc != actual_crc)
    {
        return -3;
    }

    /* Parse slave address*/
    msg_address = parse_msg_address(data);

    /* Parse payload length and data  */
    msg_length = parse_msg_data_length(data);
    msg_data = parse_msg_payload(data, msg_length);

    /* Fill message structure */
    message->address = msg_address;
    message->size = msg_length;
    message->data = msg_data;
    message->crc = msg_crc;

    return 0;
}

/**
 * @brief Dump protocol message to terminal
 * 
 * @param message Protocol message pointer
 */
void protocol_dump_message(protocol_message_t *message)
{
    printf("--------------------------------\n");
    printf("\tAddress: 0x%08x\n", message->address);
    printf("\tPayload size: %d\n", message->size);
    printf("\tPayload: %s\n", message->data);
    printf("\tMsg CRC32: 0x%08x\n", message->crc);
    printf("--------------------------------\n");
}

/*==============================================================================
                                PRIVATE FUNCTIONS
==============================================================================*/

uint32_t parse_msg_address(const char *data)
{
    uint32_t address;
    char address_text[9] = {0};
    memmove(address_text, data + 1, 8);
    sscanf(address_text, "%08x", &address);
    return address;
}

int parse_msg_data_length(const char *data)
{
    uint32_t len;
    char len_text[5] = {0};
    memmove(len_text, data + 9, 4);
    sscanf(len_text, "%04x", &len);
    return len;
}

const char *parse_msg_payload(const char *data, int length)
{
    static char *payload;
    payload = calloc(length + 1, sizeof(char));
    memmove(payload, data + 13, length);
    return payload;
}

uint32_t parse_msg_crc(const char *data, int length)
{
    uint32_t crc;
    char crc_text[9] = {0};
    memmove(crc_text, data + length - 10, 8);
    sscanf(crc_text, "%08x", &crc);
    return crc;
}
