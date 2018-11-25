#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    po::options_description desc("Othello Usage");
    desc.add_options()
            ("help", "produce help message")
            ("include,I", po::value<vector<string> >(),
             "board include path");
    po::positional_options_description p;
    p.add("include", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
    }
    if (vm.count("include")) {
        vector<string> files = vm["include"].as<vector<string> >();
        for(string file : files){
            cout << "Input file " << file << endl;
        }
    }
    return 0;
}