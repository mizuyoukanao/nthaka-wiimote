#ifndef TEST_MOCK_STATE_MACHINE_H_
#define TEST_MOCK_STATE_MACHINE_H_

#include "nxamf.h"

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
typedef struct mock_state_machine_t
{
    nxamf_state_machine_t parent;
    mock_state_t s;
} mock_state_machine_t;

void mock_state_machine_init(mock_state_machine_t *sm);

#endif // TEST_MOCK_STATE_MACHINE_H_