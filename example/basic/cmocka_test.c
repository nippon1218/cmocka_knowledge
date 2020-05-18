#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdbool.h>

typedef struct 
{
    int x;
    int y;
}test_segv_t;

static test_segv_t segv;


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
    (void) state;
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

/*
 * mockfunction 1
 */
static int mock_function(void)
{
  return (int) mock();
}

static void mock_function_test(void **state)
{
    (void) state;
    will_return(mock_function, 12);
    assert_int_equal(mock_function(), 12);
}

/*
 * mockfunction 2
 */
static int mock_function2(char **out)
{
    *out = (char *) mock();
    return (int) mock();
}

static void mock_function_test2(void **state)
{
    char *out = NULL;
    const char * const hel = "hello world";

    will_return(mock_function2, hel);
    will_return(mock_function2, 42);

    assert_int_equal(mock_function2(&out), 42);
    assert_ptr_equal(out, hel);
}

/*
 * except test
 */
static void test_segfault_recovery(void **state)
{
//    test_segv_t *seg = NULL; //if use this will fail
    test_segv_t *seg = &segv;
    
    seg->x = 1;
    seg->y = 2;
}

/*
 * expect_value
 */
static int cal_sum(int a, int b)
{
	check_expected(a);
	check_expected(b);
	return (int)mock();
}

static int cal_mul(int a, int b)
{
	check_expected(a);
	check_expected(b);
    return a * b;
}

static void test_except_value(void **state)
{
    expect_value(cal_sum, a , 4);
    expect_value(cal_sum, b , 3);
    expect_value(cal_mul, a , 2);
    expect_value(cal_mul, b , 5);
    will_return(cal_sum, 6);

    assert_int_equal(cal_sum(4,3), 6);
    assert_int_equal(cal_mul(2,5), 10);
}

int main(void) {
    const struct CMUnitTest assert_tests[] = {
        cmocka_unit_test(assert_test),
        cmocka_unit_test(test_segfault_recovery)
    };

    const struct CMUnitTest mock_fun_tests[] = {
        cmocka_unit_test(mock_function_test),
        cmocka_unit_test(mock_function_test2),
    };

    const struct CMUnitTest mock_except_tests[] = {
        cmocka_unit_test(test_except_value),
    };

    cmocka_run_group_tests(assert_tests, NULL, NULL);
    cmocka_run_group_tests(mock_fun_tests, NULL, NULL);
    cmocka_run_group_tests(mock_except_tests, NULL, NULL);
    return 0;
}

