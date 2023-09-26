#include <string.h>
#include "keyboard_decode.h"


const char diez[] = "#";
static char keyboard_input_buffer[KEYBOARD_INPUG_BUFFER_LEN];
static uint16_t start_index = 0;
static uint16_t end_index = 0;
static uint8_t next_is_special = 0;
static uint8_t is_special_chunk = 0;
// nothing more compicated is requiered since the MC doen't have parallelism, but is has interupts
static volatile uint8_t in_get = 0;

const char *get_next_old(uint8_t *ret_is_special)
{
    char *start=keyboard_input_buffer+start_index;

    if(start[0] == 0)
        return NULL;


    char *end=strchr(start, '#');

    if(next_is_special)
    {
        ret_is_special[0]=1;
        end[0]=0;
        start_index+=strlen(start)+1;
        next_is_special=0;
        if(start[0] == 0)
        {
            ret_is_special[0]=0;
            return diez;
        }
    }
    else if(start == end)
    {
        start_index++;
        start++;
        uint8_t is_special=next_is_special;
        next_is_special=0;
        if(keyboard_input_buffer[start_index] == '#')
        {
            start_index++;
            ret_is_special[0]=0;
            next_is_special=0;
            return diez;
        }
//        output is known (start++)
        get_next(&is_special);
        next_is_special=0;
        ret_is_special[0]=1;
    }
    else{
        ret_is_special[0]=0;
        if(end)
        {
            end[0]=0;
            next_is_special=1;
        }
        start_index+=strlen(start)+1;
    }

    return start;
}

static const char *actual_get_next(uint8_t *ret_is_special)
{
    char *start=keyboard_input_buffer+start_index;

    if(start[0] == 0)
        return NULL;

    if(is_special_chunk)
    {
        ret_is_special[0]=1;
        if(start[0]=='#')
        {
            ret_is_special[0]=0;
            is_special_chunk=0;
            start_index+=1;
            return diez;
        }

        char *end=strchr(start, '#');
        // wait
        if(end==NULL)
            return NULL;

        end[0]=0;
        start_index+=end-start+1;

        is_special_chunk=0;
        return start;
    }
    else
    {
        ret_is_special[0]=0;
        if(start[0]=='#')
        {
            // has the beginging of a special chunk
            is_special_chunk=1;
            start_index++;
            return get_next(ret_is_special);
        }

        // has normal text
        char *end=strchr(start, '#');
        int extra_add=0;

        if(end && end[0]=='#')
        {
            end[0]=0;
            is_special_chunk=1;
            extra_add=1;
        }
        start_index+=strlen(start)+extra_add;
        return start;
    }

}

const char *get_next(uint8_t *ret_is_special)
{
    in_get=1;
    const char *ret = actual_get_next(ret_is_special);
    in_get=0;
    return ret;
}

inline static void try_to_compress()
{
    if(start_index ==0 || in_get || keyboard_input_buffer[start_index]!=0)
        return;

    keyboard_input_buffer[0] = end_index = start_index = 0;
}

void append(const char *new_keystrokes)
{
//    try_to_compress();
    // not as eficient, but not the clasic mode of operation
    for(int i=0;new_keystrokes[i];i++)
        append_char(new_keystrokes[i]);
}

void append_char(const char new_keystroke)
{
    if(end_index==KEYBOARD_INPUG_BUFFER_LEN)
        return;
    try_to_compress();
    keyboard_input_buffer[end_index]=new_keystroke;
    end_index++;
    keyboard_input_buffer[end_index]=0;
}

void reset()
{
    memset(keyboard_input_buffer, 0, KEYBOARD_INPUG_BUFFER_LEN);
    start_index = 0;
    next_is_special = 0;
    is_special_chunk = 0;
    in_get=0;
    end_index = 0;
}
