#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

int mock_function(void);
void mock_function_call_times(size_t times, int expectedValue);

int mock_function(void)
{
  return (int) mock();
}

void mock_function_call_times(size_t times, int expectedValue)
{
    size_t i;
    for (i = 0u; i < times; ++i)
    {
        assert_int_equal(expectedValue, mock_function());
    }
}

static void test_will_return_maybe_for_no_calls(void **state)
{
    (void) state;

    will_return_maybe(mock_function, 32);
    assert_int_equal(mock_function(), 32);
}

static void test_will_return_maybe_for_one_mock_call(void **state)
{
    int value;

    (void) state;

    value = rand();
    will_return_maybe(mock_function, value);
    mock_function_call_times(1u, value);
}

static void test_will_return_maybe_for_more_than_one_call(void **state)
{
    int value;
    size_t numberOfCalls;
    (void)state;

    value = rand();
    numberOfCalls = (size_t) ((rand()) % 20 + 2);
    will_return_maybe(mock_function, value);
    mock_function_call_times(numberOfCalls, value);
}

static int mock_function_2(char **out)
{
    *out = (char *) mock();
    return (int) mock();
}

static void test_function(void **state)
{
    char *out = NULL;
    const char * const hel = "hello";

    will_return(mock_function_2, hel);
    will_return(mock_function_2, 42);

    assert_int_equal(mock_function_2(&out), 42);
    assert_ptr_equal(out, hel);
}

int main(void) {
    const struct CMUnitTest alloc_tests[] = {
        cmocka_unit_test(test_will_return_maybe_for_no_calls)
        ,cmocka_unit_test(test_will_return_maybe_for_one_mock_call)
        ,cmocka_unit_test(test_will_return_maybe_for_more_than_one_call)
        ,cmocka_unit_test(test_function)
    };

    return cmocka_run_group_tests(alloc_tests, NULL, NULL);
}

