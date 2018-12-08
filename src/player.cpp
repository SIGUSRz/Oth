#include <player.hpp>

// Returns time point
std::chrono::time_point<std::chrono::system_clock> Player::startTimer() {
    return std::chrono::system_clock::now();
}

// Returns time elapsed in seconds
float Player::stopTimer(std::chrono::time_point<std::chrono::system_clock>
                        startTime) {
    std::chrono::time_point<std::chrono::system_clock> endTime =
            std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedSeconds = endTime - startTime;
    return elapsedSeconds.count();
}

Board::Move Player::FindMove(Board board, bool &pass) {
    Board::Move move;
    if (this->isAI) {
        move = AIMove(board, pass);
    } else {
        move = HumanMove(board, pass);
    }
    return move;
}

Board::Move Player::HumanMove(Board board, bool &pass) {
    int moveNum = -1;
    string in;
    vector<Board::Move> m = board.FindLegalMoves(board.currentPlayer);
    Board::Move res = Board::Move();
    if (!m.empty()) {
        for (size_t i = 0; i < m.size(); i++) {
            cout << i << ": [" << m[i].grid.y << "," << m[i].grid.x << "]" << endl;
        }

        //make sure the user inputs a valid move
        while (!(moveNum < (int) m.size() && moveNum >= 0)) {
            cout << "Your Move: " << endl;
            cin >> in;
            if (isdigit(in.c_str()[0])) {
                moveNum = stoi(in);
            }
        }
        res = m[moveNum];
    } else {
        cout << "Pass" << endl;
        pass = true;
    }
    return res;
}

Board::Move Player::AIMove(Board board, bool &pass) {
    std::chrono::time_point<std::chrono::system_clock> startTime
            = this->startTimer();
    maxPlayer = board.currentPlayer;

    vector<Board::Move> legalMove = board.FindLegalMoves(maxPlayer);

    Board::Move move;
    if (legalMove.empty()) {
        cout << "Pass" << endl;
        pass = true;
        return move;
    }

    for (int i = 0; i < legalMove.size(); i++) {
        cout << i << ": [" << legalMove[i].grid.y << "," << legalMove[i].grid.x << "]" << endl;
    }

    int depth;
    int depthLimit = NUMGRIDS - board.discOnBoard;
    int score = 0;
    int moveIdx = 0;
    for (depth = 0; depth < depthLimit &&
                    this->stopTimer(startTime) < STOPTIME * board.timeLimit; depth++) {
        int alpha = INT_MIN, beta = INT_MAX;
        for (int i = 0; i < legalMove.size(); i++) {
            Board testBoard = board;
            testBoard.UpdateBoard(legalMove[i]);
            score = AlphaBetaPruning(testBoard, depth, startTime, alpha, beta, false);
            if (this->stopTimer(startTime) < STOPTIME * board.timeLimit) {
                break;
            }
            if (score >= alpha) {
                moveIdx = i;
                alpha = score;
            } // Not randomizing when score == alpha
        }
    }
    move = legalMove[moveIdx];
    cout << "Depth: " << depth << " in " <<
         this->stopTimer(startTime) << " seconds" << endl;
    cout << "Choose move " << moveIdx << " ["
         << move.grid.y << ", " << move.grid.x << "]" << endl;
    return legalMove[moveIdx];

}

//Board::Move Player::AIMove(Board board, bool &pass) {
//    randomizer.seed(0);
//    std::chrono::time_point<std::chrono::system_clock> startTime
//            = this->startTimer();
//    maxPlayer = board.currentPlayer;
//
//    vector<Board::Move> legalMove = board.FindLegalMoves(maxPlayer);
//
//    Board::Move move;
//    int depth = 1;
//    int moveIdx = 0;
//    if (legalMove.empty()) {
//        cout << "Pass" << endl;
//        pass = true;
//        return move;
//    } else if (legalMove.size() == 1) {
//        cout << "Only one move" << endl;
//        moveIdx = 0;
//    } else {
//        int depthLimit = NUMGRIDS - board.discOnBoard;
//        if (depthLimit < 10) {
//            // Full search to terminal states
//            std::cout << "Searching remainder of game tree..." << std::endl;
//            std::cout << "\tSearching to depth " << depthLimit;
//            moveIdx = this->AlphaBeta(board, depthLimit, startTime);
//            std::cout << "\t\tSearch complete." << std::endl;
//        } else {
//            cout << "Searching game tree..." << endl;
//            for (depth = 1; depth < depthLimit; depth++) {
//                std::cout << "\tSearching to depth " << depth << endl;
//                moveIdx = this->AlphaBeta(board, depth, startTime);
////                moveIdx = this->AlphaBetaPruning(board, depth, startTime, INT_MIN)
//
//                if (moveIdx == -1) {
//                    cout << "\t\tSearch aborted." << endl;
//                    moveIdx = (int)legalMove.size() - 1;
//                    break;
//                } else {
//                    cout << "\t\tSearch complete." << endl;
//                }
//                if (this->stopTimer(startTime) > STOPTIME * board.timeLimit) {
//                    break;
//                }
//            }
//        }
//    }
//
//    move = legalMove[moveIdx];
//    for (size_t i = 0; i < legalMove.size(); i++) {
//        cout << i << ": [" << legalMove[i].grid.y << "," << legalMove[i].grid.x << "]" << endl;
//    }
//    cout << "Depth: " << depth << " in " <<
//         this->stopTimer(startTime) << " seconds" << endl;
//    cout << "Choose move " << moveIdx << " ["
//         << move.grid.y << ", " << move.grid.x << "]" << endl;
//    return legalMove[moveIdx];
//}

int Player::AlphaBeta(Board board, int depthLimit,
                      std::chrono::time_point<std::chrono::system_clock> startTime) {
    this->nodeStack.reserve(NUMGRIDS);
    // Initialize root node
    this->nodeStack[0].isMaxNode = true;
    this->nodeStack[0].alpha = INT_MIN;
    this->nodeStack[0].beta = INT_MAX;
    this->nodeStack[0].score = INT_MIN;
    this->nodeStack[0].board = board;
    this->nodeStack[0].board.heuristicMoves = board.FindLegalMoves(board.currentPlayer);
    this->nodeStack[0].moveIterator = 0;
    this->nodeStack[0].bestIterator = 0;
    this->nodeStack[0].lastMove = (int) this->nodeStack[0].board.heuristicMoves.size() - 1;

//    for (int i = 0; i <= this->nodeStack[0].lastMove; i++) {
//        cout << this->nodeStack[0].board.heuristicMoves[i].grid.y << " "
//             << this->nodeStack[0].board.heuristicMoves[i].grid.x << endl;
//    }

    int depth = 0;
    int leafScore = 0;
    int bestMove = 0;

    while (true) {
        // If we have evaluated all children
        if (this->nodeStack[depth].moveIterator == this->nodeStack[depth].lastMove) {
            if (depth-- == 0) {
                if (this->nodeStack[1].score > this->nodeStack[0].score
                    || (this->nodeStack[1].score == this->nodeStack[0].score
                        && randomizer() % 2 == 0)) {
                    this->nodeStack[0].score = this->nodeStack[1].score;
                    bestMove = this->nodeStack[0].bestIterator;
                }

                // Update alpha
                if (this->nodeStack[0].score > this->nodeStack[0].alpha) {
                    this->nodeStack[0].alpha = this->nodeStack[0].score;
                }
                break;
            }

            if (this->nodeStack[depth].isMaxNode) {
                if (this->nodeStack[depth + 1].score > this->nodeStack[depth].score
                    || (this->nodeStack[depth + 1].score == this->nodeStack[depth].score
                        && randomizer() % 2 == 0)) {
                    this->nodeStack[depth].score = this->nodeStack[depth + 1].score;
                    if (depth == 0) {
                        bestMove = this->nodeStack[0].bestIterator;
                    }
                }

                if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                    this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                }
            } else {
                if (this->nodeStack[depth + 1].score < this->nodeStack[depth].score) {
                    this->nodeStack[depth].score = this->nodeStack[depth + 1].score;
                }

                if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                    this->nodeStack[depth].beta = this->nodeStack[depth].score;
                }
            }
        } else if (this->nodeStack[depth].beta <= this->nodeStack[depth].alpha) {
            // If we can prune
            if (depth-- == 0) {
                if (this->nodeStack[1].score > this->nodeStack[0].score
                    || (this->nodeStack[1].score == this->nodeStack[0].score
                        && randomizer() % 2 == 0)) {
                    this->nodeStack[0].score = this->nodeStack[1].score;
                    bestMove = this->nodeStack[0].bestIterator;
                }

                if (this->nodeStack[0].score > this->nodeStack[0].alpha) {
                    this->nodeStack[0].alpha = this->nodeStack[0].score;
                }

                break;
            }

            if (this->nodeStack[depth].isMaxNode) {
                if (this->nodeStack[depth + 1].score > this->nodeStack[depth].score
                    || (this->nodeStack[depth + 1].score == this->nodeStack[depth].score
                        && randomizer() % 2 == 0)) {
                    this->nodeStack[depth].score = this->nodeStack[depth + 1].score - 1;
                    if (depth == 0) {
                        bestMove = this->nodeStack[0].bestIterator;
                    }
                }

                if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                    this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                }
            } else {
                if (this->nodeStack[depth + 1].score < this->nodeStack[depth].score) {
                    this->nodeStack[depth].score = this->nodeStack[depth + 1].score + 1;
                }

                if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                    this->nodeStack[depth].beta = this->nodeStack[depth].score;
                }
            }
        } else {
            // Generate next node, increment iterators
            this->nodeStack[depth + 1].board = this->nodeStack[depth].board;
            this->nodeStack[depth + 1].board.SwitchPlayer();
            this->nodeStack[depth + 1].board.UpdateBoard(
                    this->nodeStack[depth].board.heuristicMoves[this->nodeStack[depth].moveIterator]);
            this->nodeStack[depth].bestIterator = this->nodeStack[depth].moveIterator;
            this->nodeStack[depth].moveIterator++;
            // If the next depth is not at the depth limit
            if (depth + 1 < depthLimit) {
                depth++;
                // Initialize next node in stack
                this->nodeStack[depth].isMaxNode = !this->nodeStack[depth - 1].isMaxNode;
                this->nodeStack[depth].score =
                        (this->nodeStack[depth].isMaxNode ? INT_MIN : INT_MAX);
                this->nodeStack[depth].alpha = this->nodeStack[depth - 1].alpha;
                this->nodeStack[depth].beta = this->nodeStack[depth - 1].beta;
                this->nodeStack[depth].board.heuristicMoves =
                        this->nodeStack[depth].board.FindLegalMoves(
                                this->nodeStack[depth].board.currentPlayer);

                this->nodeStack[depth].moveIterator = 0;
                this->nodeStack[depth].bestIterator =
                        this->nodeStack[depth].moveIterator;
                this->nodeStack[depth].lastMove =
                        (int) this->nodeStack[depth].board.heuristicMoves.size() - 1;
            } else {
                // The node is a leaf: evaluate heuristic and update values
                leafScore = this->heuristic.Minimax_Heuristic(
                        this->nodeStack[depth + 1].board);

                if (this->nodeStack[depth].isMaxNode) {
                    if (leafScore > this->nodeStack[depth].score) {
                        this->nodeStack[depth].score = leafScore;
                        if (depth == 0) {
                            bestMove = this->nodeStack[0].bestIterator;
                        }
                    }

                    if (this->nodeStack[depth].score > this->nodeStack[depth].alpha) {
                        this->nodeStack[depth].alpha = this->nodeStack[depth].score;
                    }
                } else {
                    if (leafScore < this->nodeStack[depth].score) {
                        this->nodeStack[depth].score = leafScore;
                    }

                    if (this->nodeStack[depth].score < this->nodeStack[depth].beta) {
                        this->nodeStack[depth].beta = this->nodeStack[depth].score;
                    }
                }
            }
        }
        // If we are almost out of time, failure, return last move
        if (this->stopTimer(startTime) > STOPTIME * board.timeLimit) {
            return -1;
        }
    }

    return bestMove;
}

int Player::AlphaBetaPruning(Board board, int depth,
                             std::chrono::time_point<std::chrono::system_clock> startTime,
                             int alpha, int beta, bool maxPlayer) {
    int a = alpha, b = beta;

    if (this->stopTimer(startTime) > STOPTIME * board.timeLimit) {
        return heuristic.Minimax_Heuristic(board);
    } else if (depth == 0) {
        return heuristic.Minimax_Heuristic(board);
    } else {
        depth--;
    }
    vector<Board::Move> m = board.FindLegalMoves(board.currentPlayer);
    if (m.empty()) {
        if (board.TerminalState()) {
            Board testBoard = board;
            testBoard.SwitchPlayer();
            return heuristic.Minimax_Heuristic(testBoard);
        } else {
            Board testBoard = board;
            testBoard.pass[0] = false;
            testBoard.pass[1] = true;
            return AlphaBetaPruning(testBoard, depth, startTime,
                                    alpha, beta, !maxPlayer);
        }
    }

    if (maxPlayer) {
        int value = INT_MIN;
        for (auto &move : m) {
            Board testBoard = board;
            testBoard.UpdateBoard(move);
            int score = AlphaBetaPruning(testBoard, depth, startTime, a, b, false);
            value = MAX(value, score);

            if (value >= beta) {
                return value;
            }
            a = MAX(a, value);
        }
        return value;
    } else {
        int value = INT_MAX;
        for (auto &move : m) {
            Board testBoard = board;
            testBoard.UpdateBoard(move);
            int score = AlphaBetaPruning(testBoard, depth, startTime, a, b, true);
            value = MIN(value, score);

            if (value <= a) {
                return value;
            }
            b = MIN(b, value);
        }
        return value;
    }
}