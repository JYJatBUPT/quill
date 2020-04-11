#include "quill/PatternFormatter.h"
#include "quill/detail/misc/Macros.h"
#include <chrono>
#include <gtest/gtest.h>

using namespace quill::detail;
using namespace quill;

TEST(PatternFormatter, default_pattern_formatter)
{
  PatternFormatter default_pattern_formatter;

  std::chrono::nanoseconds ts{1579815761000023021};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {} formatter {}", LogLevel::Info};

  // Format to a buffer
  default_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 1234);

  auto const& formatted_buffer = default_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string =
    "21:42:41.000023021 [31341] PatternFormatterTest.cpp:16 LOG_INFO     test_logger - This the "
    "pattern formatter 1234\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, custom_pattern_message_only)
{
  // Message only
  PatternFormatter custom_pattern_formatter{QUILL_STRING("%(message)"), "%H:%M:%S",
                                            PatternFormatter::TimestampPrecision::NanoSeconds};

  std::chrono::nanoseconds ts{1579815761000023000};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {1} formatter {0}", LogLevel::Debug};

  // Format to a buffer
  custom_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 12.34);

  auto const& formatted_buffer = custom_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string = "This the 12.34 formatter pattern\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, custom_pattern_timestamp_precision_nanoseconds)
{
  // Custom pattern with part 1 and part 3
  PatternFormatter custom_pattern_formatter{
    QUILL_STRING(
      "%(ascii_time) [%(thread)] %(filename):%(lineno) LOG_%(level_name) %(logger_name) - "
      "%(message) [%(function_name)]"),
    "%m-%d-%Y %H:%M:%S", PatternFormatter::TimestampPrecision::NanoSeconds};

  std::chrono::nanoseconds ts{1579815761000023000};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {1} formatter {0}", LogLevel::Debug};

  // Format to a buffer
  custom_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 1234);

  auto const& formatted_buffer = custom_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string =
    "01-23-2020 21:42:41.000023000 [31341] PatternFormatterTest.cpp:72 LOG_DEBUG    test_logger - "
    "This the "
    "1234 formatter pattern [TestBody]\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, custom_pattern_timestamp_precision_microseconds)
{
  PatternFormatter custom_pattern_formatter{
    QUILL_STRING(
      "%(ascii_time) [%(thread)] %(filename):%(lineno) LOG_%(level_name) %(logger_name) - "
      "%(message) [%(function_name)]"),
    "%m-%d-%Y %H:%M:%S", PatternFormatter::TimestampPrecision::MicroSeconds};

  std::chrono::nanoseconds ts{1579815761020123000};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {1} formatter {0}", LogLevel::Debug};

  // Format to a buffer
  custom_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 1234);

  auto const& formatted_buffer = custom_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string =
    "01-23-2020 21:42:41.020123 [31341] PatternFormatterTest.cpp:102 LOG_DEBUG    test_logger - "
    "This the "
    "1234 formatter pattern [TestBody]\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, custom_pattern_timestamp_precision_milliseconds)
{
  PatternFormatter custom_pattern_formatter{
    QUILL_STRING(
      "%(ascii_time) [%(thread)] %(filename):%(lineno) LOG_%(level_name) %(logger_name) - "
      "%(message) [%(function_name)]"),
    "%m-%d-%Y %H:%M:%S", PatternFormatter::TimestampPrecision::MilliSeconds};

  std::chrono::nanoseconds ts{1579815761099000000};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {1} formatter {0}", LogLevel::Debug};

  // Format to a buffer
  custom_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 1234);

  auto const& formatted_buffer = custom_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string =
    "01-23-2020 21:42:41.099 [31341] PatternFormatterTest.cpp:132 LOG_DEBUG    test_logger - This "
    "the "
    "1234 formatter pattern [TestBody]\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, custom_pattern_timestamp_precision_none)
{
  PatternFormatter custom_pattern_formatter{
    QUILL_STRING(
      "%(ascii_time) [%(thread)] %(filename):%(lineno) LOG_%(level_name) %(logger_name) - "
      "%(message) [%(function_name)]"),
    "%m-%d-%Y %H:%M:%S", PatternFormatter::TimestampPrecision::None};

  std::chrono::nanoseconds ts{1579815761099220000};
  char const* thread_id = "31341";
  std::string const logger_name = "test_logger";
  StaticLogRecordInfo log_line_info{QUILL_STRINGIFY(__LINE__), __FILE__, __func__,
                                    "This the {1} formatter {0}", LogLevel::Debug};

  // Format to a buffer
  custom_pattern_formatter.format(ts, thread_id, logger_name.data(), log_line_info, "pattern", 1234);

  auto const& formatted_buffer = custom_pattern_formatter.formatted_log_record();

  // Convert the buffer to a string
  std::string const formatted_string = fmt::to_string(formatted_buffer);

  std::string const expected_string =
    "01-23-2020 21:42:41 [31341] PatternFormatterTest.cpp:162 LOG_DEBUG    test_logger - This the "
    "1234 formatter pattern [TestBody]\n";

  EXPECT_EQ(formatted_buffer.size(), expected_string.length());
  EXPECT_EQ(formatted_string, expected_string);
}

TEST(PatternFormatter, invalid_pattern)
{
  EXPECT_THROW(
    PatternFormatter(
      QUILL_STRING("%(ascii_time) [%(thread)] %(filename):%(lineno) %(level_name) %(logger_name) - "
                   "[%(function_name)]"),
      "%H:%M:%S", PatternFormatter::TimestampPrecision::NanoSeconds),
    std::runtime_error);
}