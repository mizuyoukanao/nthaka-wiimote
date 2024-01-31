#ifndef TEST_MOCK_PROTOCOL_H_
#define TEST_MOCK_PROTOCOL_H_

#include "nthaka.h"

typedef enum mock_state_t
{
    MOCK_STATE_INITIAL,
    MOCK_STATE_0,
    MOCK_STATE_1,
    MOCK_STATE_FINAL
} mock_state_t;

/**
 * It accepts only {0, 1, 2}.
 */
typedef struct mock_protocol_t
{
    nthaka_protocol_handler_t parent;
    mock_state_t s;
} mock_protocol_t;

void mock_protocol_init(mock_protocol_t *ph);

#endif // TEST_MOCK_PROTOCOL_H_