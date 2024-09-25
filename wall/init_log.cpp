// #define TEST_LOGGER
// g++ -std=c++11 -DBOOST_ALL_DYN_LINK <this-file>.cpp -lboost_log -lboost_log_setup -lboost_thread

#include <iostream>
#include <string>

#include "init_log.hpp"

#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"

using namespace std;

void init_log(string log_file_prefix = "")
{
    static const string COMMON_FMT("[%TimeStamp%] [%Severity%]:\t%Message%");

    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    if (log_file_prefix.length() > 0)
        // Output message to file, rotates when file reached 1mb or at midnight every day. Each log file
        // is capped at 1mb and total is 20mb
        boost::log::add_file_log (
            boost::log::keywords::file_name = (log_file_prefix + "_%3N.log"),
            boost::log::keywords::rotation_size = 1 * 1024 * 1024,
            boost::log::keywords::max_size = 20 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = COMMON_FMT,
            boost::log::keywords::auto_flush = true
        );

    // Output message to console
    boost::log::add_console_log(
        cout,
        boost::log::keywords::format = COMMON_FMT,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();

    // Only output message with INFO or higher severity in Release
#ifdef NDEBUG
    // DEBUG OFF
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );
#endif
}

#ifdef TEST_LOGGER
int main()
{
    try {
        init_log("default");
        BOOST_LOG_TRIVIAL(info) << "Log init";
    }
    catch (exception &e) {
        string errmsg = e.what();
        errmsg[0] = toupper(errmsg[0]);
        cout << "[E] " << errmsg << endl;
        return EXIT_FAILURE;
    }
    // Output some simple log message
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
    return EXIT_SUCCESS;
}
#endif
