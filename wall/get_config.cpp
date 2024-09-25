/*
    Author: Roy Bebru (RoyBebru@Gmail.Com)
*/

// #define TEST_GET_CONFIG
// g++ -std=c++11 <file>.cpp -lboost_program_options

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <cinttypes>
#include <iostream>
#include <fstream>
#include <iterator>

#include "get_config.hpp"

using namespace std;

// // A helper function to simplify the main part
// template <class T>
// ostream &operator<<(ostream &os, const vector<T> &v)
// {
//     copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
//     return os;
// }

int get_config(config& conf, int argc, char* argv[])
{
    if (argc <= 1)
        cout << "[!] Use -h for help info" << endl;

    try {
        string config_file;
        string proto_version;
        uint8_t proto_ver_major;
        uint8_t proto_ver_minor;
        uint16_t listen_port;
        uint32_t answer_length;
        string path_logfile_prefix;

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
        ("help,h", "produce this help info")
        ("config,c",
            po::value<string>(&config_file)->default_value(DEFAULT_CONFIG_FILENAME),
            "name of configuration file");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
        ("proto-version,v",
            po::value<string>(&proto_version)->default_value(DEFAULT_PROTO_VERSION),
            "used protocol version")
        ("listen-port,p",
            po::value<uint16_t>(&listen_port)->default_value(DEFAULT_LISTEN_PORT),
            "udp port to listen")
        ("answer-length,l",
            po::value<uint32_t>(&answer_length)->default_value(DEFAULT_ANSWER_LENGTH),
            "length of the answer to client")
        ("path-logfile-prefix,l",
            po::value<string>(&path_logfile_prefix)->default_value(""),
            "used protocol version")
        ;

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        po::options_description config_file_options;
        config_file_options.add(config);

        po::options_description allopts("Allowed options");
        allopts.add(generic).add(config);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << allopts << endl;
            return EXIT_SUCCESS;
        }

        ifstream ifs(config_file.c_str());
        if (ifs) {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
        else if (config_file != DEFAULT_CONFIG_FILENAME) {
            cerr << "[E] Cannot open config file: " << config_file << endl;
            return EXIT_FAILURE;
        }

        if (vm.count("listen-port") &&
                !(1 <= listen_port && listen_port <= 65534)) {
            cerr << "[E] Wrong listen-port value: " << listen_port << endl;
            return EXIT_FAILURE;
        }

        if (vm.count("answer-length") && answer_length < 1000) {
            cerr << "[E] Wrong answer-length value: " << answer_length << endl;
            return EXIT_FAILURE;
        }

        size_t found = proto_version.find(".");
        if (!isdigit(proto_version[0]) ||     // wrong major version
            found == string::npos ||          // absent minor version
            !isdigit(proto_version[found+1])) // wrong minor version
        {
            cerr << "[E] Wrong format of proto-version value: " << proto_version << endl;
            return EXIT_FAILURE;
        }
        proto_ver_major = stoi(proto_version);
        proto_ver_minor = stoi(proto_version.substr(found+1, proto_version.length() - found));

        if (path_logfile_prefix.length() >= PATH_LOGFILE_PREFIX_SIZE) {
            cerr << "[E] Very long path-logfile-prefix: " << path_logfile_prefix << endl;
            return EXIT_FAILURE;
        }

        cout << "[C] Work Configuration:" << endl
             << "|      proto-version: " << static_cast<unsigned int>(proto_ver_major) << "."
                                         << static_cast<unsigned int>(proto_ver_minor) << endl
             << "|        listen-port: " << listen_port << endl
             << "|      answer-length: " << answer_length << endl
             << "|path-logfile-prefix: " << path_logfile_prefix << endl;

        conf = {
            .proto_ver_major = proto_ver_major,
            .proto_ver_minor = proto_ver_minor,
            .listen_port = listen_port,
            .answer_length = answer_length,
        };
        strcpy(conf.path_logfile_prefix, path_logfile_prefix.c_str());
    }
    catch (exception &e) {
        string errmsg = e.what();
        errmsg[0] = toupper(errmsg[0]);
        cout << "[E] " << errmsg << endl;
        return 1;
    }

    return EXIT_SUCCESS;
}

#ifdef TEST_GET_CONFIG

int main(int argc, char* argv[])
{
    config myconfig;

    int rc = get_config(myconfig, argc, argv);
    if (EXIT_SUCCESS != rc)
        return rc;

    cout << "[T] myconfig:{"
         << "proto_ver_major=" << static_cast<unsigned int>(myconfig.proto_ver_major) << ","
         << "proto_ver_minor=" << static_cast<unsigned int>(myconfig.proto_ver_minor) << ","
         << "listen_port=" << myconfig.listen_port << ","
         << "answer_length=" << myconfig.answer_length << ","
         << "path_logfile_prefix=" << myconfig.path_logfile_prefix
         << "}" << endl;
}
#endif
