#include "heuristic.hpp"

int Heuristic::Minimax_Heuristic(Board &board) {
    maxPlayer = board.currentPlayer;
    minPlayer = (maxPlayer == WHITE)
                ? BLACK
                : WHITE;

    // At terminal state
    if (board.TerminalState()) {
        return 100000 * this->Utility(board);
    }

    vector<int> res;


    if (board.discOnBoard <= 20) {
        // Opening game
        return 5 * MobilityScore(board)
               + 5 * PotentialMobility(board)
               + 20 * SquareWeights(board)
               + 10000 * StabilityScore(board)
               + 10000 * CornerScore(board)
               + 10000 * CornerLossScore(board);
    } else if (board.discOnBoard <= 58) {
        // Midgame
        return 10 * DiscScore(board)
               + 5 * MobilityScore(board)
               + 5 * PotentialMobility(board)
               + 100 * SquareWeights(board)
               + 100 * Parity(board)
               + 200 * FrontierScore(board)
               + 200 * EdgeScore(board)
               + 10000 * CornerScore(board)
               + 10000 * StabilityScore(board)
               + 10000 * CornerLossScore(board);
    } else {
        // Endgame
        return 500 * DiscScore(board)
               + 500 * Parity(board)
               + 10000 * StabilityScore(board)
               + 10000 * CornerScore(board)
               + 10000 * CornerLossScore(board);
    }
}

int Heuristic::Utility(Board &board) {
    if (maxPlayer == WHITE) {
        return WHITE * board.discOnBoard;
    } else {
        return board.discOnBoard;
    }
}

// Relative disc difference between the two players
int Heuristic::DiscScore(Board &board) {
    if (maxPlayer == BLACK) {
        return 100 * (board.blackScore - board.whiteScore) / (board.blackScore + board.whiteScore);
    } else {
        return 100 * (board.whiteScore - board.blackScore) / (board.blackScore + board.whiteScore);
    }
}

// Computes a lower bound on the number of stable discs
int Heuristic::StabilityScore(Board &board) {
    this->stableDiscs.clear();
    for (int i = 0; i < 4; i++) {
        this->stableDiscs.emplace_back(false);
    }

    return this->StableCorner(board, maxPlayer) - this->StableCorner(board, minPlayer);
}

// Finds the number of stable discs given a corner
int Heuristic::StableCorner(Board &board, int color) {
    int stability = 0;
    if (board.board[0][0] == color) {
        if (board.board[0][BOARDSIZE - 1] == color && !this->stableDiscs[1]) {
            stability++;
            this->stableDiscs[1] = true;
        }
    }
    if (board.board[0][BOARDSIZE - 1] == color) {
        if (board.board[BOARDSIZE - 1][0] == color && !this->stableDiscs[2]) {
            stability++;
            this->stableDiscs[2] = true;
        }
    }
    if (board.board[BOARDSIZE - 1][0] == color) {
        if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == color && !this->stableDiscs[3]) {
            stability++;
            this->stableDiscs[3] = true;
        }
    }
    if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == color) {
        if (board.board[0][0] == color && !this->stableDiscs[0]) {
            stability++;
            this->stableDiscs[0] = true;
        }
    }
    return stability;
}

// Number of possible moves
int Heuristic::MobilityScore(Board &board) {
    vector<Board::Move> maxLegalMove = board.FindLegalMoves(maxPlayer);
    vector<Board::Move> minLegalMove = board.FindLegalMoves(minPlayer);
    int maxMoves = (int) maxLegalMove.size();
    int minMoves = (int) minLegalMove.size();
    return 100 * (maxMoves - minMoves) / (maxMoves + minMoves + 1);
}

int Heuristic::PotentialMobility(Board &board) {
    int myPotentialMobility = PlayerPotentialMobility(board, maxPlayer);
    int opponentPotentialMobility = PlayerPotentialMobility(board, minPlayer);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
           / (myPotentialMobility + opponentPotentialMobility + 1);
}

int Heuristic::NegColor(int color) {
    return color == BLACK ? WHITE : BLACK;
}

int Heuristic::PlayerPotentialMobility(Board &board, int color) {
    int potentialMobility = 0;
    int neg = NegColor(color);

    // Interior
    for (int i = BOARDSIZE / 4; i < 3 * BOARDSIZE / 4; i++) {
        for (int j = BOARDSIZE / 4; j < 3 * BOARDSIZE / 4; j++) {
            if (board.board[i][j] == neg) {
                if (board.board[i - 1][j] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i + 1][j] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i][j - 1] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i][j - 1] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i - 1][j - 1] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i - 1][j + 1] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i + 1][j - 1] == EMPTY) {
                    potentialMobility++;
                }
                if (board.board[i + 1][j + 1] == EMPTY) {
                    potentialMobility++;
                }
            }

        }
    }

    //Second Row
    for (int j = BOARDSIZE / 4; j < 3 * BOARDSIZE / 4; j++) {
        int i = (BOARDSIZE - 1) / 4;
        if (board.board[i][j] == neg) {
            if (board.board[i][j - 1] == EMPTY) {
                potentialMobility++;
            }
            if (board.board[i][j + 1] == EMPTY) {
                potentialMobility++;
            }
        }
    }

    // Second Last Row
    for (int j = BOARDSIZE / 4; j < 3 * BOARDSIZE / 4; j++) {
        int i = BOARDSIZE - (BOARDSIZE - 1) / 4;
        if (board.board[i][j] == neg) {
            if (board.board[i][j - 1] == EMPTY) {
                potentialMobility++;
            }
            if (board.board[i][j + 1] == EMPTY) {
                potentialMobility++;
            }
        }
    }

    //Second Row
    for (int i = BOARDSIZE / 4; i < 3 * BOARDSIZE / 4; i++) {
        int j = (BOARDSIZE - 1) / 4;
        if (board.board[i][j] == neg) {
            if (board.board[i - 1][j] == EMPTY) {
                potentialMobility++;
            }
            if (board.board[i + 1][j] == EMPTY) {
                potentialMobility++;
            }
        }
    }

    //Second Last Column
    for (int i = BOARDSIZE / 4; i < 3 * BOARDSIZE / 4; i++) {
        int j = BOARDSIZE - (BOARDSIZE - 1) / 4;
        if (board.board[i][j] == neg) {
            if (board.board[i - 1][j] == EMPTY) {
                potentialMobility++;
            }
            if (board.board[i + 1][j] == EMPTY) {
                potentialMobility++;
            }
        }
    }

    return potentialMobility;
}

int Heuristic::CornerScore(Board &board) {
    std::vector<int> corners = {0, 7, 56, 63};
    int maxCorner = 0;
    int minCorner = 0;

    if (board.board[0][0] == maxPlayer) {
        maxCorner++;
    } else if (board.board[0][0] == minPlayer) {
        minCorner++;
    }
    if (board.board[0][BOARDSIZE - 1] == maxPlayer) {
        maxCorner++;
    } else if (board.board[0][BOARDSIZE - 1] == minPlayer) {
        minCorner++;
    }
    if (board.board[BOARDSIZE - 1][0] == maxPlayer) {
        maxCorner++;
    } else if (board.board[BOARDSIZE - 1][0] == minPlayer) {
        minCorner++;
    }
    if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == maxPlayer) {
        maxCorner++;
    } else if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == minPlayer) {
        minCorner++;
    }
    return 100 * (maxCorner - minCorner)
           / (maxCorner + minCorner + 1);
}

int Heuristic::CornerLossScore(Board &board) {
    int cornerLoss = 0;
    vector<Board::Move> minLegalMove = board.FindLegalMoves(minPlayer);
    for (auto &minMove : minLegalMove) {
        if (minMove.grid.y == 0 && minMove.grid.x == 0) {
            cornerLoss++;
        } else if (minMove.grid.y == 0 && minMove.grid.x == BOARDSIZE - 1) {
            cornerLoss++;
        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == 0) {
            cornerLoss++;
        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == BOARDSIZE - 1) {
            cornerLoss++;
        }
    }
    return -25 * cornerLoss;
}

// Assigns a weight to every square on the board
int Heuristic::SquareWeights(Board &board) {
    int eW = 1000;
    int neW = -500;
    vector<vector<int>> weights = {
            {eW,  -100, 100, 50,  50,  100, neW, eW},
            {neW, -200, -50, -50, -50, -50, -200, neW},
            {100,  -50,  100, 0,   0,   100, -50,  100},
            {50,   -50,  0,   0,   0,   0,   -50,  50},
            {50,   -50,  0,   0,   0,   0,   -50,  50},
            {100,  -50,  100, 0,   0,   100, -50,  100},
            {neW, -200, -50, -50, -50, -50, -200, neW},
            {eW,  neW, 100, 50,  50,  100, neW, eW}
    };

    if (board.board[0][0] != EMPTY) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                weights[i][j] = 0;
            }
        }
    }

    if (board.board[0][BOARDSIZE - 1] != EMPTY) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                weights[i][BOARDSIZE - 1 - j] = 0;
            }
        }
    }

    if (board.board[BOARDSIZE - 1][0] != EMPTY) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                weights[BOARDSIZE - 1 - i][j] = 0;
            }
        }
    }

    if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] != EMPTY) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                weights[BOARDSIZE - 1 - i][BOARDSIZE - 1 - j] = 0;
            }
        }
    }

    int score = 0;
    vector<int> tmp1, tmp2;
    for (int i = 0; i < BOARDSIZE; i++) {
        score += inner_product(board.board[i].begin(), board.board[i].end(),
                               weights[i].begin(), 0);
    }
    return score;
}

int Heuristic::Parity(Board &board) {
    int squaresRemaining = NUMGRIDS - board.discOnBoard;
    if (squaresRemaining % 2 == 0) {
        return -1;
    } else {
        return 1;
    }
}

int Heuristic::EdgeScore(Board &board) {
    int maxEdge = 0, minEdge = 0;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (i == 0 || i == BOARDSIZE - 1 || j == 0 || j == BOARDSIZE - 1) {
                if (board.board[i][j] == maxPlayer) {
                    maxEdge++;
                } else if (board.board[i][j] == minPlayer) {
                    minEdge++;
                }
            }
        }
    }
    return 100 * maxEdge / (maxEdge + minEdge + 1);
}

int Heuristic::FrontierScore(Board &board) {
    int maxFrontier = 0, minFrontier = 0;
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (board.board[i][j] != '0') {
                if (board.OnFrontier(i, j)) {
                    if (board.board[i][j] == maxPlayer) {
                        maxFrontier++;
                    } else if (board.board[i][j] == minPlayer) {
                        minFrontier++;
                    }
                }
            }
        }
    }
    return -100 * (maxFrontier - minFrontier);
}

//int Heuristic::Minimax_Heuristic(Board& board) {
//    maxPlayer = board.currentPlayer;
//    int minPlayer = (maxPlayer == WHITE)
//                    ? BLACK
//                    : WHITE;
//
//    int pW = 2 * board.discOnBoard;
//    int cW = 5000;
//    int clW = 2000;
//    int fW = 200;
//    int eW = 50;
//    int mW = 10 * (100 - board.discOnBoard);
//
//    int maxPiece = maxPlayer == BLACK ? board.blackScore : board.whiteScore;
//    double pieceScore = (100.0 * maxPiece) / (board.blackScore + board.whiteScore);
//
//    int maxCorner = 0, minCorner = 0;
//    if (board.board[0][0] == maxPlayer) {
//        maxCorner++;
//    } else if (board.board[0][0] == minPlayer) {
//        minCorner++;
//    }
//    if (board.board[0][BOARDSIZE - 1] == maxPlayer) {
//        maxCorner++;
//    } else if (board.board[0][BOARDSIZE - 1] == minPlayer) {
//        minCorner++;
//    }
//    if (board.board[BOARDSIZE - 1][0] == maxPlayer) {
//        maxCorner++;
//    } else if (board.board[BOARDSIZE - 1][0] == minPlayer) {
//        minCorner++;
//    }
//    if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == maxPlayer) {
//        maxCorner++;
//    } else if (board.board[BOARDSIZE - 1][BOARDSIZE - 1] == minPlayer) {
//        minCorner++;
//    }
//    double cornerScore = 25.0 * (maxCorner - minCorner);
//
//    int maxEdge = 0, minEdge = 0;
//    int maxFrontier = 0, minFrontier = 0;
//    for (int i = 0; i < BOARDSIZE; i++) {
//        for (int j = 0; j < BOARDSIZE; j++) {
//            if (i == 0 || i == BOARDSIZE - 1 || j == 0 || j == BOARDSIZE - 1) {
//                if (board.board[i][j] == maxPlayer) {
//                    maxEdge++;
//                } else if (board.board[i][j] == minPlayer) {
//                    minEdge++;
//                }
//            } else if (board.board[i][j] != '0') {
//                if (board.OnFrontier(i, j)) {
//                    if (board.board[i][j] == maxPlayer) {
//                        maxFrontier++;
//                    } else if (board.board[i][j] == minPlayer) {
//                        minFrontier++;
//                    }
//                }
//            }
//        }
//    }
//
//    double edgeScore = 100.0 * maxEdge / (maxEdge + minEdge);
//    double frontierScore = -100.0 * (maxFrontier - minFrontier); //Do not want frontier piece
//
//    vector<Board::Move> maxLegalMove = board.FindLegalMoves(maxPlayer);
//    vector<Board::Move> minLegalMove = board.FindLegalMoves(minPlayer);
//    double moveScore = 100.0 * maxLegalMove.size() / (maxLegalMove.size() + minLegalMove.size());
//
//    int cornerLoss = 0;
//    for (auto &minMove : minLegalMove) {
//        if (minMove.grid.y == 0 && minMove.grid.x == 0) {
//            cornerLoss++;
//        } else if (minMove.grid.y == 0 && minMove.grid.x == BOARDSIZE - 1) {
//            cornerLoss++;
//        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == 0) {
//            cornerLoss++;
//        } else if (minMove.grid.y == BOARDSIZE - 1 && minMove.grid.x == BOARDSIZE - 1) {
//            cornerLoss++;
//        }
//    }
//    double cornerLossScore = -25.0 * cornerLoss;
//
//    return int(ceil(pW * pieceScore + cW * cornerScore + clW * cornerLossScore +
//                    eW * edgeScore + fW * frontierScore + mW * moveScore - 0.5));
//}