#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#define bool_(b) ((b) ? "true" : "false")
#define nxamf_buffer_state_t_(s) ((s) == NXAMF_BUFFER_STATE_PENDING    ? "NXAMF_BUFFER_STATE_PENDING"  \
                                  : (s) == NXAMF_BUFFER_STATE_REJECTED ? "NXAMF_BUFFER_STATE_REJECTED" \
                                                                       : "NXAMF_BUFFER_STATE_ACCEPTED")

#endif // TEST_UTIL_H_