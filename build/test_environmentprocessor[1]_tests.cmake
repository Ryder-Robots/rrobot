add_test([=[TestEnvironmentProcessor.TestEnvironment]=]  /home/aaron/Projects/rrobot/build/test/test_environmentprocessor [==[--gtest_filter=TestEnvironmentProcessor.TestEnvironment]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestEnvironmentProcessor.TestEnvironment]=]  PROPERTIES WORKING_DIRECTORY /home/aaron/Projects/rrobot/test/resources SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  test_environmentprocessor_TESTS TestEnvironmentProcessor.TestEnvironment)
