#include <iostream>

namespace TestHelper {
    unsigned int tests_ran = 0;
    unsigned int tests_failed = 0;
    static void equals(
        std::string_view label, const auto& response, const auto& expected_response) {
        if (response != expected_response) {
            const auto print_lambda = [](const auto& v) {
                std::cerr << v << " ";
            };
            std::cerr << "Test case '" << label << "' failed\n";
            std::for_each(response.begin(), response.end(), print_lambda);
            std::cerr << "== ";
            std::for_each(expected_response.begin(), expected_response.end(), print_lambda);
            std::cerr << '\n';
            ++tests_failed;
        }
        ++tests_ran;
    }

    // Overload for booleans
    static void equals(std::string_view label, const bool response, const bool expected_response) {
        if (response != expected_response) {
            std::cerr << "Test case '" << label << "' failed\n"
                      << response << " == " << expected_response << '\n';
            ++tests_failed;
        }
        ++tests_ran;
    }

    // Overload for uint8_t
    static void equals(
        std::string_view label, const uint8_t response, const uint8_t expected_response) {
        if (response != expected_response) {
            std::cerr << "Test case '" << label << "' failed\n"
                      << response << " == " << expected_response << '\n';
            ++tests_failed;
        }
        ++tests_ran;
    }
    static void starts_with(
        std::string_view label, const auto& response, const auto& expected_start) {
        if (!response.starts_with(expected_start)) {
            const auto print_lambda = [](const auto& v) {
                std::cerr << v << " ";
            };
            std::cerr << "Test case '" << label << "' failed\n";
            std::for_each(response.begin(), response.end(), print_lambda);
            std::cerr << "starts_with ";
            std::for_each(expected_start.begin(), expected_start.end(), print_lambda);
            std::cerr << '\n';
            ++tests_failed;
        }
        ++tests_ran;
    }

    static auto result(std::string_view label) {
        if (tests_failed == 0) {
            std::cout << label << ": all test cases passed\n";
            return 0;
        } else {
            std::cerr << label << ": " << tests_failed << " test case";
            if (tests_failed != 1) {
                std::cerr << 's';
            }
            std::cerr << " failed\n";
            return 1;
        }
    }
}
