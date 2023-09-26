#include <inttypes.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define KEYBOARD_INPUG_BUFFER_LEN 1024

const char *get_next(uint8_t *ret_is_special);

void append(const char *);
void append_char(const char new_keystroke);

void reset();


#if defined(__cplusplus)
}
#endif
