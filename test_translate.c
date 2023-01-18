#include "../src/translate.h"

#include <211.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/////
///// TEST HELPER SIGNATURES
/////

/*
 * The purpose of these helper functions is to make your test cases
 * easier to read and write. You should use them, but also be sure to go
 * check out their definitions at the bottom of this file. Try to
 * understand what they're doing for you, because you will very likely
 * want to design testing helpers of your own for future assignments.
 */

// Runs an expand_charseq() test case. Asserts that
// `expand_charseq(input)` will return `expected`.
static void
check_expand(const char* input, const char* expected);

// Runs a translate() test case. Asserts that `translate(input,
// from0, to0)` changes `input` to `expected`. (But uses a copy of
// `input`, since `input` might be a string literal, and changing those
// is UB.)
static void
check_tr(const char* input,
         const char* from0,
         const char* to0,
         const char* expected);

// Clones a string into fresh, `malloc`ed memory. Some C implementations
// provide a function `strdup` to do this, but it's non-standard.
static char*
clone_string(const char* src);


/////
///// TEST CASES FOR charseq_length
/////

static void test_charseq_length(void)
{
    CHECK_SIZE( charseq_length("abc"), 3 );
    CHECK_SIZE( charseq_length("a-z"), 26 );

    //
    // TODO: Add more tests for charseq_length() here
    //
    CHECK_SIZE( charseq_length("a-cdga-c"), 8);
    CHECK_SIZE( charseq_length("d-ca"), 1);
    CHECK_SIZE( charseq_length("\\"), 1);
    CHECK_SIZE( charseq_length("   "), 3);
    CHECK_SIZE( charseq_length(""), 0);
    CHECK_SIZE( charseq_length("A-Ca-c"), 6);
    CHECK_SIZE( charseq_length("\n"), 1);
    CHECK_SIZE( charseq_length("\t"), 1);
    CHECK_SIZE( charseq_length("0 0 AA A-Z"), 33);
    CHECK_SIZE( charseq_length("-11"), 3);
    CHECK_SIZE( charseq_length("c-c-b-b"), 3);

}


/////
///// TEST CASES FOR expand_charseq
/////

static void test_expand_charseq(void)
{
    check_expand( "abc", "abc" );
    check_expand( "a-e", "abcde" );

    //
    // TODO: Add more tests for expand_charseq() here
    //
    //check_expand("", NULL);
    check_expand("\\t", "\t");
    check_expand("d-c", "");
    check_expand("a-cA-C", "abcABC");
    check_expand("c-aA-C3-11-3", "ABC123");
    check_expand("1-4", "1234");
    check_expand("1-44-1", "1234");
    check_expand("\\-_", "\\]^_");
    check_expand("X-\\n","XYZ[\\n");
    check_expand("", "");
    check_expand("z-a-b", "-b");
    check_expand("c-c-b-b", "c-b");


}
    


/////
///// TEST CASES FOR translate_char
/////

static void test_translate_char(void)
{
    CHECK_CHAR( translate_char('a', "xyz", "XYZ"), 'a' );
    CHECK_CHAR( translate_char('y', "xyz", "XYZ"), 'Y' );
    

    //
    // TODO: Add more tests for translate_char() here
    //
    CHECK_CHAR( translate_char('a', "aDz", "AdZ"), 'A');
    CHECK_CHAR( translate_char('g', "a-z", "A-Z"), 'g');
    CHECK_CHAR( translate_char('c', "   c", "123H"), 'H');
    CHECK_CHAR( translate_char('n', "\\ltn", "abcd"), 'd');
    CHECK_CHAR( translate_char('4', "hot4", "hot"), '\0');
    CHECK_CHAR( translate_char('d',"Gerardo Ortiz", "Regulo Caro"), 'o');
    CHECK_CHAR( translate_char('\\', "chi\\/", "chill"), 'l');
    CHECK_CHAR( translate_char('n', "chow town", "12345678\\"), '\\');
    CHECK_CHAR( translate_char('a', "Kevin", "steve"), 'a');
    CHECK_CHAR( translate_char('\\', "Sho\nut", "123456"), '\\');

}


/////
///// TEST CASES FOR translate
/////

static void test_translate(void)
{
    check_tr( "Hello, world!", ",! ",    "___",    "Hello__world_" );
    check_tr( "hELLO, WORLD!", "a-zA-Z", "A-Za-z", "Hello, world!" );

    //
    // TODO: Add more tests for translate() here
    //
    check_tr( "hey", "hey", "1-3", "123");
    check_tr( "c \\ aR\\l", "a-z", "A-Z", "C \\ AR\\L");
    check_tr( "I like Dogs do you like dogs?", "I", "\\", "\\ like Dogs do you like dogs?");
    check_tr( "sI se PudO", "Iu", "13", "s1 se P3dO");
    check_tr( "????gatos", " ", "f", "????gatos");

}

/////
///// main
/////

int main(void)
{
    test_expand_charseq();
    test_translate_char();
    test_translate();
    test_charseq_length();

    return 0;
}


/////
///// HELPER FUNCTION DEFINITIONS
/////

static void check_expand(const char* input, const char* expected)
{
    char* actual = expand_charseq(input);
    CHECK_STRING( actual, expected );
    free(actual);
}

static void check_tr(const char* input,
                     const char* from0,
                     const char* to0,
                     const char* expected)
{
    char* from   = expand_charseq(from0);
    char* to     = expand_charseq(to0);

    // This is a more standard use of `assert` than we saw last week:
    // stopping the program if something is wrong. These assertions will
    // crash our test program if memory allocation fails, rather than
    // keep going and perform a bogus test.
    assert(from);
    assert(to);

    // We need to clone `input` because it probably came from a string
    // literal, and C forbids modifying string literals.
    char* actual = clone_string(input);

    // The actual operation and check:
    translate(actual, from, to);
    CHECK_STRING( actual, expected );

    // Deallocate the memory, since this function allocated it and now
    // we're done with it.
    free(from);
    free(to);
    free(actual);
}

static char* clone_string(const char* src)
{
    char* dst = malloc(strlen(src) + 1);
    assert( dst ); // avoids UB in case of OOM
    strcpy(dst, src);
    return dst;
}
