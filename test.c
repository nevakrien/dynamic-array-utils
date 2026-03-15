#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

#define TEST(name) \
    do { printf("running %-30s", name); fflush(stdout); } while (0)

#define PASS() \
    do { printf(" ok\n"); } while (0)


static void test_string_view(void)
{
    TEST("StringView basic");

    StringView v;
    SET_FROM_CSTR(v, "hello");

    assert(v.len == 5);
    assert(AT(v,0) == 'h');
    assert(AT(v,4) == 'o');
    assert(PEEK(v) == 'o');

    PASS();
}


static void test_string_slice_remove_unordered(void)
{
    TEST("StringSlice REMOVE_UNORDERED");

    char buffer[] ="abcde";
    StringSlice s;
    SET_FROM_CSTR(s, buffer);

    REMOVE_UNORDERED(s,1);   // remove 'b'

    assert(s.len == 4);

    int seen_a=0,seen_c=0,seen_d=0,seen_e=0;

    for (size_t i=0;i<s.len;i++) {
        if (s.data[i]=='a') seen_a=1;
        if (s.data[i]=='c') seen_c=1;
        if (s.data[i]=='d') seen_d=1;
        if (s.data[i]=='e') seen_e=1;
    }

    assert(seen_a && seen_c && seen_d && seen_e);

    PASS();
}


static void test_stringbuilder_push(void)
{
    TEST("StringBuilder PUSH");

    StringBuilder sb = {0};   // zero-init builder

    PUSH(sb,'h');
    PUSH(sb,'e');
    PUSH(sb,'l');
    PUSH(sb,'l');
    PUSH(sb,'o');

    // printf("len is %d\n",(int)sb.len);
    assert(sb.len == 5);
    assert(AT(sb,0) == 'h');
    assert(AT(sb,4) == 'o');
    assert(PEEK(sb) == 'o');

    assert(POP(sb) == 'o');
    assert(sb.len == 4);

    free(sb.data);

    PASS();
}

static void test_lvalue_behavior(void)
{
    TEST("AT/PEEK lvalue behavior");

    char buffer[] = "abcde";
    StringSlice s;
    SET_FROM_CSTR(s, buffer);

    // AT as lvalue
    AT(s,1) = 'X';
    assert(s.data[1] == 'X');

    // PEEK as lvalue
    PEEK(s) = 'Z';
    assert(s.data[s.len - 1] == 'Z');

    // combine with POP
    assert(POP(s) == 'Z');
    assert(s.len == 4);

    PASS();
}


int main(void)
{
    test_string_view();
    test_string_slice_remove_unordered();
    test_stringbuilder_push();
    test_lvalue_behavior();

    printf("\nall tests passed\n");
    return 0;
}