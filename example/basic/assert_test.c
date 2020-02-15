#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdbool.h>

static bool is_single(int num)
{
    return (num % 2 == 0) ? false : true ;
}

static int sum(int a, int b)
{
    return a + b;    
}

static void assert_test(void **state) {
    const unsigned long test_sets[3]={1, 3, 5};

    assert_int_equal(3, 3);
    assert_int_not_equal(2, 4);
    assert_float_equal(2.0, 2.1, 0.4);
    assert_float_not_equal(2.0, 2.5, 0.04);
    assert_in_range(2, 1, 4);
    assert_not_in_range(5, 1, 4);
    assert_true(is_single(3));
    assert_false(is_single(2));
    assert_in_set(1, test_sets, 3);
    assert_not_in_set(2, test_sets, 3);
    assert_memory_equal(test_sets, test_sets, 3); 
    assert_memory_not_equal(&test_sets[0], &test_sets[1], 3); 
    assert_non_null(test_sets);  /* assert the given pointer is non-NULL   */
    assert_null(NULL);  /* assert the given pointer is NULL*/
    assert_ptr_equal(test_sets, test_sets);
    assert_ptr_not_equal(test_sets, &test_sets[1]);
    assert_return_code(sum(2, 3), 0);
    assert_string_equal("hello", "hello");
    assert_string_not_equal("hello", "world");

}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(assert_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

