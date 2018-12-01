#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "game.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    string board_file;
    int gameType, timeLimit;
    Game game;
    try {
        po::options_description desc("Othello Usage");
        desc.add_options()
                ("help", "produce help message")
                ("type,t", po::value<int>(&gameType)->required(),
                 "game type: 0 - player first, 1 - opponent first")
                ("limit,l", po::value<int>(&timeLimit)->required(),
                 "time limit for each steps");
        po::options_description hidden;
        hidden.add_options()
                ("include,I", po::value<string>(),
                 "board include path");
        po::options_description all;
        all.add(desc);
        all.add(hidden);

        po::positional_options_description p;
        p.add("include", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                options(all).positional(p).run(), vm);

        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }

        if (vm.count("type")) {
            gameType = vm["type"].as<int>();
        }
        if (vm.count("limit")) {
            timeLimit = vm["limit"].as<int>();
        }
        if (vm.count("include")) {
            board_file = vm["include"].as<string>();
        }
        po::notify(vm);
    } catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    } catch (...) {
        cerr << "Unknown Error" << endl;
        return -1;
    }
    if (!(gameType == 0 || gameType == 1)) {
        cerr << "Wrong game type" << endl;
        return -1;
    }
    game.setup(gameType, timeLimit, board_file);

    game.Play();
    return 0;
}