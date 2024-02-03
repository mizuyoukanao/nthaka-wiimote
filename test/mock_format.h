#ifndef TEST_MOCK_FORMAT_H_
#define TEST_MOCK_FORMAT_H_

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
typedef struct mock_format_t
{
    nthaka_format_handler_t parent;
    mock_state_t s;
} mock_format_t;

void mock_format_init(mock_format_t *fmt);

#endif // TEST_MOCK_FORMAT_H_