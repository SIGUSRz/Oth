#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "game.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int argc, const char *argv[]) {
    string board_file;
    Game game;
    float timeLimit = 0.0;
    int p1, p2;
    try {
        po::options_description desc("Othello Usage");
        desc.add_options()
                ("help", "produce help message")
                ("player,p", po::value<int>(&p1)->required(),
                 "first player: 0 - computer, 1 - human")
                ("opponent,o", po::value<int>(&p2)->required(),
                 "second player: 0 - computer, 1 - human")
                ("limit,l", po::value<float>(&timeLimit)->required(),
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

        if (vm.count("player")) {
            p1 = vm["player"].as<int>();
        }
        if (vm.count("opponent")) {
            p2 = vm["opponent"].as<int>();
        }
        if (vm.count("limit")) {
            timeLimit = vm["limit"].as<float>();
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
    if (!(p1 == 0 || p1 == 1)) {
        cerr << "Wrong game type" << endl;
        return -1;
    }
    if (!(p2 == 0 || p2 == 1)) {
        cerr << "Wrong game type" << endl;
        return -1;
    }
    game.setup(p1, (int)timeLimit, board_file);

    game.Play();
    return 0;
}