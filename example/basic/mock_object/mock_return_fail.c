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

static void test_will_return_fails_for_no_calls(void **state)
{
    (void) state;

    will_return(mock_function, 32);
    assert_int_equal(mock_function(), 32); /* if not call once, will return fail */
    //assert_int_equal(mock_function(), 32); /* call twoice, will return fail */
}

static void test_will_return_count_fails_for_unreturned_items(void **state)
{
    int value;
    size_t numberOfCalls;

    (void) state;

    value = rand();
    numberOfCalls = (size_t) ((rand()) % 20 + 2);

    will_return_count(mock_function, value, numberOfCalls);
    mock_function_call_times(numberOfCalls - 1u, value);
}

static void test_will_return_always_fails_for_no_calls(void **state)
{
    int value;

    (void) state;

    value = rand();

    will_return_always(mock_function, value);
    assert_int_equal(mock_function(), value); /* if no call, will return fail */
}

static int teardown(void **state) {
    free(*state);

    return 0;
}

int main(void) {
    const struct CMUnitTest alloc_tests[] = {
        cmocka_unit_test_teardown(test_will_return_fails_for_no_calls, teardown)
        ,cmocka_unit_test_teardown(test_will_return_count_fails_for_unreturned_items, teardown)
        ,cmocka_unit_test_teardown(test_will_return_always_fails_for_no_calls, teardown)
    };

    return cmocka_run_group_tests(alloc_tests, NULL, NULL);
}

