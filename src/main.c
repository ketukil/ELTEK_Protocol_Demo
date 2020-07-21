/**
 * @file main.c
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
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "crc32.h"
#include "protocol.h"

/**
 * @brief Main test application for ELTEK protocol demo
 * 
 * @return int Error code
 */
int main(void)
{
    int status;
    protocol_message_t *message;
    char *input;

    /* initialize CRC-32 module */
    crc32_init();
    /* dump generated CRC-32 table for debug purposes */
    crc32_dump_table();

    while(true)
    {
        /* Allocate message memory and initialize some values */
        message = calloc(sizeof(protocol_message_t), 1);
        input = calloc(80, sizeof(char));
        
        printf("Enter HEX address: ");
        while(scanf("%s", input) == 0);
        if (!strcasecmp("exit", input))
        {
            break;
        }
        sscanf(input, "%x", &message->address);

        printf("Enter payload: ");
        while(scanf("%s", input) == 0);
        if (!strcasecmp("exit", input))
        {
            break;
        }
        message->data = strdup(input);

        /* Send message to serialization buffer */
        protocol_message_send(message);
        free(message);
        message = NULL;
        /* Dump message from the buffer*/
        printf("Message: %s\n", serialized_message);

        /* Allocate some new message memory */
        message = calloc(sizeof(protocol_message_t), 1);
        /* Read from serialization buffer to the message structure */
        status = protocol_message_read(message, (const char *)serialized_message);
        printf("Read status: %d\n", status);
        /* Dump message structure content to terminal */
        protocol_dump_message(message);
        free(message);
        message = NULL;

        /* Corrupted message */
        const char *serialized_message = ":deadb3ef001b[SomeDataPayloadOfSomeKind]95c64368\r\n";
        printf("Corrupted message: %s\n", serialized_message);

        /* Allocate some new message memory */
        message = calloc(sizeof(protocol_message_t), 1);
        /* Read from serialization buffer with corrupted data to the message structure */
        status = protocol_message_read(message, (const char *)serialized_message);
        printf("Read status: %d\n", status);
        /* Dump message structure content to terminal */
        protocol_dump_message(message);
        free(message);
        message = NULL;
    }

    exit(0);
}
