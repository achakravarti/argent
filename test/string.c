#include "./test.h"


/* 
 * The following unit tests test out ag_string_new() with statically allocated
 * empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(new_01, "ag_string_new() can create an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("");

        AG_TEST (s && !*s);
}


AG_TEST_CASE(new_02, "ag_string_new() can create an ASCII string") 
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (s && *s && ag_string_eq(s, "Hello, world!"));
}


AG_TEST_CASE(new_03, "ag_string_new() can create a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_TEST (s && *s && ag_string_eq(s, "नमस्ते दुनिया!"));
}


/* This unit test checks whether ag_string_new_empty() behaves as expected. */
AG_TEST_CASE(new_empty_01, "ag_string_new_empty() creates an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_TEST (s && !*s);
}


/* 
 * The following unit tests check whether ag_string_new_fmt() performs as
 * expected with in the case of statically allocated empty and formatted ASCII
 * and Unicode strings.
 */


AG_TEST_CASE(new_fmt_01, "ag_string_new_fmt() can create an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%s", "");
        AG_TEST (s && !*s);
}


AG_TEST_CASE(new_fmt_02,
    "ag_string_new_fmt() can create a formatted ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%d. %s", 111,
            "Hello, world!");

        AG_TEST (s && *s && ag_string_eq(s, "111. Hello, world!"));
}


AG_TEST_CASE(new_fmt_03,
    "ag_string_new_fmt() can create a formatted Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new_fmt("%d. %s", -12, "नमस्ते दुनिया!");

        AG_TEST (s && *s && ag_string_eq(s, "-12. नमस्ते दुनिया!"));
}


/*
 * The following unit tests check whether ag_string_copy() can create shallow
 * copies of empty, ASCII and Unicode string instances.
 */


AG_TEST_CASE(copy_01, "ag_string_copy() can create a copy of an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && !*s2);
}


AG_TEST_CASE(copy_02, "ag_string_copy() can create a copy of an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE(copy_03, "ag_string_copy() can create a copy of a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (s2 && *s2 && ag_string_eq(s, s2));
}


AG_TEST_CASE(copy_04, "ag_string_copy() increases the reference count by 1")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_refc(s) == 2);
}


/*
 * The following unit tests check whether ag_string_release() releases a string
 * correctly, or perform a safe no-op if provided invalid strings.
 */


AG_TEST_CASE(release_01, "ag_string_release() performs a no-op if passed NULL")
{
        ag_string_release(NULL);

        AG_TEST (true);
}


AG_TEST_CASE(release_02,
    "ag_string_release() performs a no-op if passed a handle to NULL")
{
        ag_string *s = NULL;
        ag_string_release(&s);

        AG_TEST (true);
}


AG_TEST_CASE(release_03, "ag_string_release() disposes a single instance of a"
                " string") {
        ag_string *s = ag_string_new("Hello, world!");
        ag_string_release(&s);

        AG_TEST (!s);
}


AG_TEST_CASE(release_04, "ag_string_release() reduces the reference count by 1")
{
        ag_string *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s2);
        ag_string_release(&s);

        AG_TEST (!s && ag_string_refc(s2) == 2);
}


/*
 * The following test cases check whenter ag_string_cmp() behaves as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(cmp_01,
    "ag_string_cmp() returns AG_CMP_EQ when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE(cmp_02,
    "ag_string_cmp() return AG_CMP_LT when comparing an empty string with"
    " another string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_LT);
}


AG_TEST_CASE(cmp_03,
    "ag_string_cmp() return AG_CMP_EQ when comparing two equal ASCII strings")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE(cmp_04,
    "ag_string_cmp() returns AG_CMP_LT when comparing an ASCII string that is"
    " lexicographically less than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_LT);
}


AG_TEST_CASE(cmp_05,
    "ag_string_cmp() returns AG_CMP_GT when comparing an ASCII string that is"
    " lexicographically greater than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_GT);
}


AG_TEST_CASE(cmp_06,
    "ag_string_cmp() returns AG_CMP_GT when comparing an ASCII string to an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_GT);
}


AG_TEST_CASE(cmp_07,
    "ag_string_cmp() returns AG_CMP_EQ when comparing two equal Unicode"
    "strings")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_EQ);
}


AG_TEST_CASE(cmp_08,
    "ag_string_cmp() returns AG_CMP_LT when comparing a Unicode string that is"
    " lexicographically less than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_cmp(s2, s) == AG_CMP_LT);
}


AG_TEST_CASE(cmp_09,
    "ag_string_cmp() returns AG_CMP_GT when comparing a Unicode string that is"
    " lexicographically greater than another")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_cmp(s, s2) == AG_CMP_GT);
}


/*
 * The following unit tests check whether ag_string_lt() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(lt_01, 
    "ag_string_lt() returns false when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (!ag_string_lt(s, s2));
}


AG_TEST_CASE(lt_02,
    "ag_string_lt() returns true when comparing an empty string with a"
    " non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_lt(s, s2));
}


AG_TEST_CASE(lt_03,
    "ag_string_lt() returns false when comparing a string with an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE(lt_04,
    "ag_string_lt() returns true when comparing an ASCII string that is"
    " lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_lt(s, s2));
}


AG_TEST_CASE(lt_05,
    "ag_string_lt() returns false when comparing an ASCII string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_lt(s, s2));
}


AG_TEST_CASE(lt_06,
    "ag_string_lt() returns false when comparing an ASCII string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE(lt_07,
    "ag_string_lt() returns true when comparing a Unicode string that is"
    " lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_lt(s2, s));
}


AG_TEST_CASE(lt_08, 
    "ag_string_lt() returns false when comparing a Unicode string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_lt(s2, s));
}


AG_TEST_CASE(lt_09,
    "ag_string_lt() returns false when comparing a Unicode string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_lt(s, s2));
}


/*
 * The following unit tests check whether ag_string_eq() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(eq_01,
    "ag_string_eq() returns true when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();
        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(eq_02,
    "ag_string_eq() returns false when comparing an empty string with a"
    " non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s, s2));
}


AG_TEST_CASE(eq_03,
    "ag_string_eq() returns false when comparing a string with an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE(eq_04,
    "ag_string_eq() returns false when comparing an ASCII string that is"
    " lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");
        
        AG_TEST (!ag_string_eq(s, s2));
}


AG_TEST_CASE(eq_05,
    "ag_string_eq() returns true when comparing an ASCII string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        
        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(eq_06,
    "ag_string_eq() returns false when comparing an ASCII string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");
        
        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE(eq_07,
    "ag_string_eq() returns false when comparing a Unicode string that is"
    " lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");
        
        AG_TEST (!ag_string_eq(s2, s));
}


AG_TEST_CASE(eq_08,
    "ag_string_eq() returns true when comparing a Unicode string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        
        AG_TEST (ag_string_eq(s2, s));
}


AG_TEST_CASE(eq_09,
    "ag_string_eq() returns false when comparing a Unicode string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");
        
        AG_TEST (!ag_string_eq(s, s2));
}


/*
 * The following unit tests check whether ag_string_gt() works as expected with
 * different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(gt_01,
    "ag_string_gt() returns false when comparing two empty strings")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new_empty();

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE(gt_02,
    "ag_string_gt() returns false when comparing an empty string  with a"
    " non-empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");
        
        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE(gt_03,
    "ag_string_gt() returns true when comparing a string with an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_gt(s2, s));
}


AG_TEST_CASE(gt_04,
    "ag_string_gt() returns false when comparing an ASCII string that is"
    " lexicographically less than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE(gt_05,
    "ag_string_gt() returns false when comparing an ASCII string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_gt(s, s2));
}


AG_TEST_CASE(gt_06,
    "ag_string_gt() returns true when comparing an ASCII string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Goodbye, world!");
        AG_AUTO(ag_string) *s2 = ag_string_new("Hello, world!");

        AG_TEST (ag_string_gt(s2, s));
}


AG_TEST_CASE(gt_07,
    "ag_string_gt() returns false when comparing a Unicode string that is"
    " lexicographically less than another string") 
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (!ag_string_gt(s2, s));
}


AG_TEST_CASE(gt_08,
    "ag_string_gt() returns false when comparing a Unicode string that is"
    " lexicographically equal to another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);

        AG_TEST (!ag_string_gt(s2, s));
}


AG_TEST_CASE(gt_09,
    "ag_string_gt() returns true when comparing a Unicode string that is"
    " lexicographically greater than another string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_new("अभिषेक बहुत बुरा गाता है।");

        AG_TEST (ag_string_gt(s, s2));
}


/*
 * The followint tests check whether ag_string_empty() behaves as expected with
 * empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(empty_01, "ag_string_empty() returns true for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();

        AG_TEST (ag_string_empty(s));
}


AG_TEST_CASE(empty_02, "ag_string_empty() returns false for an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_empty(s));
}


AG_TEST_CASE(empty_03, "ag_string_empty() returns false for a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");

        AG_TEST (!ag_string_empty(s));
}


/*
 * The following unit tests check whether ag_string_len() returns the correct
 * lexicographical length for empty, ASCII and Unicode strings. The len_03 tests
 * seems to indicate that there may be a possible bug when considering combined
 * characters as in the Devenagari script.
 *
 * TODO: Research more on Devanagari string lengths.
 */


AG_TEST_CASE(len_01, "ag_string_len() returns 0 for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();

        AG_TEST (!ag_string_len(s));
}


AG_TEST_CASE(len_02, "ag_string_len() returns the length of an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

        AG_TEST (ag_string_len(s) == 13);
}


AG_TEST_CASE(len_03, "ag_string_len() returns the length of a Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");

        AG_TEST (ag_string_len(s) == 14);
}


/*
 * The following tests check whether ag_string_sz() works as expected with empty,
 * ASCII and Unicode strings.
 */


AG_TEST_CASE(sz_01, "ag_string_sz() returns 1 for an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();

        AG_TEST (ag_string_sz(s) == 1);
}


AG_TEST_CASE(sz_02, "ag_string_sz() determines the size of an ASCII string") {
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_TEST (ag_string_sz(s) == 14);
}


AG_TEST_CASE(sz_03, "ag_string_sz() determines the size of Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");

        AG_TEST (ag_string_sz(s) == 39);
}


/*
 * The following unit tests check whether ag_string_refc() returns the correct
 * reference count depending on the current number of soft copies.
 */


AG_TEST_CASE(refc_01, "ag_string_refc() returns 1 for a single instance")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");

        AG_TEST (ag_string_refc(s) == 1);
}


AG_TEST_CASE(refc_02, "ag_string_refc() detects incremented reference counts") {
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_copy(s);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s2);
        AG_TEST (ag_string_refc(s) == 3);
}


AG_TEST_CASE(refc_03, "ag_string_refc() detects decremented reference counts")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        ag_string *s2 = ag_string_copy(s);
        ag_string_release(&s2);
        AG_AUTO(ag_string) *s3 = ag_string_copy(s);

        AG_TEST (ag_string_refc(s) == 2);
}


/*
 * The following test cases check whether ag_string_has() performs as expected with
 * different combinations of empty, ASCII and Unicode needles and haystacks.
 */


AG_TEST_CASE(has_01,
    "ag_string_has() returns true when both needle and haystack are empty"
    " strings")
{
        AG_AUTO(ag_string) *h = ag_string_new_empty(), 
            *n = ag_string_new_empty();

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE(has_02, 
    "ag_string_has() returns false when haystack is empty and needle is not")
{
        AG_AUTO(ag_string) *h = ag_string_new_empty(),
            *n = ag_string_new("Hello, world!");

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE(has_03,
    "ag_string_has() returns false when haystack is not empty and needle is")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"), 
            *n = ag_string_new_empty();

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE(has_04,
    "ag_string_has() returns true if it finds an ASCII needle in an ASCII"
    " haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"),
            *n = ag_string_new("o, wo");

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE(has_05,
    "ag_string_has() returns false if it doesn't find an ASCII needle in an"
    " ASCII haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("Hello, world!"), 
            *n = ag_string_new("o, w!");

        AG_TEST (!ag_string_has(h, n));
}


AG_TEST_CASE(has_06,
    "ag_string_has() returns true if it finds a Unicode needle in a Unicode"
    " haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *n = ag_string_new("ते दुनि");

        AG_TEST (ag_string_has(h, n));
}


AG_TEST_CASE(has_07,
    "ag_string_has() returns false if it doesn't find a Unicode needle in a"
    " Unicode haystack")
{
        AG_AUTO(ag_string) *h = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *n = ag_string_new("तेदुनि");

        AG_TEST (!ag_string_has(h, n));
}


/*
 * The following test cases check whether the case transformation functions of
 * the string interface perform as expected. Note that these test cases are only
 * for empty and ASCII strings since the case transformation functions are not
 * yet Unicode friendly.
 * 
 * TODO: Add Unicode tests for string case transformation functions.
 */


AG_TEST_CASE(lower_01, "ag_string_lower() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(lower_02,
    "ag_string_lower() converts an ASCII string to lowercase")
{
        AG_AUTO(ag_string) *s = ag_string_new("HElLo, WOrlD!");
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);

        AG_TEST (ag_string_eq(s2, "hello, world!"));
}


AG_TEST_CASE(upper_01, "ag_string_upper() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_upper(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(upper_02,
    "ag_string_upper() converts an ASCII string to uppercase")
{
        AG_AUTO(ag_string) *s = ag_string_new("heLlO, woRLd!");
        AG_AUTO(ag_string) *s2 = ag_string_upper(s);

        AG_TEST (ag_string_eq(s2, "HELLO, WORLD!"));
}


AG_TEST_CASE(proper_01, "ag_string_proper() has no effect on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_proper(s);

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(proper_02,
    "ag_string_proper() converts an ASCII string to proper case")
{
        AG_AUTO(ag_string) *s = ag_string_new("tHIS isN'T.iN pRopER cASe.");
        AG_AUTO(ag_string) *s2 = ag_string_proper(s);

        AG_TEST (ag_string_eq(s2, "This Isn't.In Proper Case."));
}


/*
 * The following test cases check whether ag_string_split() behaves as expected
 * with different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(split_01,
    "ag_string_split() returns an empty string if applied on an empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, " wo");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_02,
    "ag_string_split() returns the original string if the pivot is an empty"
    " string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s, s2));
}

AG_TEST_CASE(split_03,
    "ag_string_split() returns an empty string if both the string and the pivot"
    " are empty") 
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s2, ""));
}


AG_TEST_CASE(split_04,
    "ag_string_split() returns an empty string if the pivot is not found")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "xyz");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_05,
    "ag_string_split() returns an empty string if both the string and pivot are"
    " the same") 
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, s);

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_06,
    "ag_string_split() returns the left side of the pivot if it exists in an"
    " ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, " wo");

        AG_TEST (ag_string_eq(s2, "Hello,"));
}


AG_TEST_CASE(split_07,
    "ag_string_split() returns the left side of the pivot if it exists in a"
    " Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "या");

        AG_TEST (ag_string_eq(s2, "नमस्ते दुनि"));
}


/*
 * The following test cases check whether ag_string_split_right() behaves as
 * expected with different combinations of empty, ASCII and Unicode strings.
 */


AG_TEST_CASE(split_right_01,
    "ag_string_split_right() returns an empty string if applied on an empty"
    " string")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, " wo");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_right_02,
    "ag_string_split_right() returns the original string if the pivot is an"
    " empty string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "");

        AG_TEST (ag_string_eq(s, s2));
}


AG_TEST_CASE(split_right_03,
    "ag_string_split_right() returns an empty string if both the string and the"
    " pivot are empty")
{
        AG_AUTO(ag_string) *s = ag_string_new_empty();
        AG_AUTO(ag_string) *s2 = ag_string_split(s, "");

        AG_TEST (ag_string_eq(s2, ""));
}


AG_TEST_CASE(split_right_04,
    "ag_string_split_right() returns an empty string if the pivot is not found")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "xyz");

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_right_05,
    "ag_string_split_right() returns an empty string if both the string and"
    " pivot are the same")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, s);

        AG_TEST (ag_string_empty(s2));
}


AG_TEST_CASE(split_right_06,
    "ag_string_split_right() returns the right side of the pivot if it exists"
    " in an ASCII string")
{
        AG_AUTO(ag_string) *s = ag_string_new("Hello, world!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, " w");

        AG_TEST (ag_string_eq(s2, "orld!"));
}


AG_TEST_CASE(split_right_07,
    "ag_string_split() returns the right side of the pivot if it exists in a"
    " Unicode string")
{
        AG_AUTO(ag_string) *s = ag_string_new("नमस्ते दुनिया!");
        AG_AUTO(ag_string) *s2 = ag_string_split_right(s, "स्ते");

        AG_TEST (ag_string_eq(s2, " दुनिया!"));
}


extern ag_test_suite *test_suite_string(void)
{
        ag_test *test[] = {
                new_01,         new_02,         new_03,
                new_empty_01,
                new_fmt_01,     new_fmt_02,     new_fmt_03,
                copy_01,        copy_02,        copy_03, 
                copy_04,
                release_01,     release_02,     release_03,
                release_04,
                cmp_01,         cmp_02,         cmp_03,
                cmp_04,         cmp_05,         cmp_06,
                cmp_07,         cmp_08,         cmp_09,
                lt_01,          lt_02,          lt_03,
                lt_04,          lt_05,          lt_06,
                lt_07,          lt_08,          lt_09, 
                eq_01,          eq_02,          eq_03,
                eq_04,          eq_05,          eq_06,
                eq_07,          eq_08,          eq_09,
                gt_01,          gt_02,          gt_03,
                gt_04,          gt_05,          gt_06,
                gt_07,          gt_08,          gt_09,
                empty_01,       empty_02,       empty_03, 
                len_01,         len_02,         len_03, 
                sz_01,          sz_02,          sz_03,
                refc_01,        refc_02,        refc_03, 
                has_01,         has_02,         has_03,
                has_04,         has_05,         has_06, 
                has_07,
                lower_01,       lower_02,
                upper_01,       upper_02,
                proper_01,      proper_02,
                split_01,       split_02,       split_03,
                split_04,       split_05,       split_06, 
                split_07,
                split_right_01, split_right_02, split_right_03,
                split_right_04, split_right_05, split_right_06,
                split_right_07,
        };

        const char *desc[] = {
                new_01_desc,            new_02_desc,
                new_03_desc, 
                new_empty_01_desc,
                new_fmt_01_desc,        new_fmt_02_desc,
                new_fmt_03_desc,        copy_01_desc,
                copy_02_desc,           copy_03_desc,
                copy_04_desc,
                release_01_desc,        release_02_desc,
                release_03_desc,        release_04_desc,
                cmp_01_desc,            cmp_02_desc, 
                cmp_03_desc,            cmp_04_desc, 
                cmp_05_desc,            cmp_06_desc,
                cmp_07_desc,            cmp_08_desc, 
                cmp_09_desc, 
                lt_01_desc,             lt_02_desc,
                lt_03_desc,             lt_04_desc, 
                lt_05_desc,             lt_06_desc, 
                lt_07_desc,             lt_08_desc, 
                lt_09_desc, 
                eq_01_desc,             eq_02_desc, 
                eq_03_desc,             eq_04_desc, 
                eq_05_desc,             eq_06_desc, 
                eq_07_desc,             eq_08_desc,
                eq_09_desc, 
                gt_01_desc,             gt_02_desc, 
                gt_03_desc,             gt_04_desc,                
                gt_05_desc,             gt_06_desc, 
                gt_07_desc,             gt_08_desc, 
                gt_09_desc,
                empty_01_desc,          empty_02_desc, 
                empty_03_desc, 
                len_01_desc,            len_02_desc, 
                len_03_desc, 
                sz_01_desc,             sz_02_desc, 
                sz_03_desc,
                refc_01_desc,           refc_02_desc, 
                refc_03_desc, 
                has_01_desc,            has_02_desc, 
                has_03_desc,            has_04_desc, 
                has_05_desc,            has_06_desc,
                has_07_desc, 
                lower_01_desc,          lower_02_desc, 
                upper_01_desc,          upper_02_desc, 
                proper_01_desc,         proper_02_desc, 
                split_01_desc,          split_02_desc, 
                split_03_desc,          split_04_desc, 
                split_05_desc,          split_06_desc, 
                split_07_desc, 
                split_right_01_desc,    split_right_02_desc, 
                split_right_03_desc,    split_right_04_desc,
                split_right_05_desc,    split_right_06_desc,
                split_right_07_desc,
        };

        ag_test_suite *ctx = ag_test_suite_new("ag_string interface");
        ag_test_suite_push_array(ctx, test, desc, sizeof test / sizeof *test);

        return ctx;
}

