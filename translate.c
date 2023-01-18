#include "translate.h"

#include <stddef.h>
#include <stdlib.h>

// Helper function for interpreting escapes. Given the character that
// follows a backslash, returns the character that the combination
// should be replaced with. Recognizes the seven standard C escapes, and
// maps every other character to itself.
char interpret_escape(char c)
{
    switch (c) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    default:  return c;
    }
}

size_t charseq_length(const char* src)
{   
    
    size_t result = 0;

    //
    // TODO: Your code goes here
    //

    size_t index = 0;
    while(src[index] != '\0') {
        if (src[index + 1] == '-' && src[index + 2] != '\0') 
        {
            char end = src[index + 2];
            char start = src[index];
            if(start > end) 
            {
                index = index + 3;
            } 
            else 
            {
                result = end - start + 1 + result;
                index = index + 3;
            }
        }
        else if (src[index] == '\\' && src[index + 1] != '\0') 
        {
            result++;
            index = index + 2;
        }
        else 
        {
            result++;
            index++;
        }
    }
    
    return result;
}

char* expand_charseq(const char* src)
{
    char* const dst = malloc(charseq_length(src) + 1);
    if (!dst) {
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;

    //
    // TODO: Your code goes here
    //
    
    while(src[i] != '\0')
    {
        if(src[i + 1] == '-' && src[i + 2] != '\0')
        {
            int start_ = src[i];
            int end_ = src[i+2];
            while(start_ <= end_)
            {
                dst[j] = start_;
                start_++;
                j++;
            }
            i = i + 3;
        }
        else if (src[i] == '\\' && src[i + 1] != '\0')
        {
            dst[j] = interpret_escape(src[i + 1]);
            i = i + 2;
            j++;
        }
        else
        {
            dst[j] = src[i];
            i++;
            j++;
        }
    }
    dst[j] = '\0';
    return dst;
}

char translate_char(char c, const char* from, const char* to)
{
    //
    // TODO: Your code goes here
    //
    size_t index = 0;
    while(from[index] != '\0')
    {
        if(from[index] == c)
        {
            return c = to[index];
        }
        index++;
    }
    return c;
}

void translate(char* s, const char* from, const char* to)
{
    //
    // TODO: Your code goes here
    //
    
    size_t index = 0;
    while(s[index] != '\0')
    {
        s[index] = translate_char(s[index], from, to);
        index++;
    }
}

