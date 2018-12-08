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
        for (int i = 0; i < static_cast<int>(m.size()); i++) {
            cout << i << ": [" << m[i].grid.y << "," << m[i].grid.x << "]" << endl;
        }

        //make sure the user inputs a valid move
        while (!(moveNum < static_cast<int>(m.size()) && moveNum >= 0)) {
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
    heuristic.maxPlayer = board.currentPlayer;
    heuristic.minPlayer = board.currentPlayer == BLACK ? WHITE : BLACK;
    randomizer.seed(0);

    vector<Board::Move> legalMove = board.FindLegalMoves(heuristic.maxPlayer);

    Board::Move move;
    if (legalMove.empty()) {
        cout << "Pass" << endl;
        pass = true;
        return move;
    }

    for (int i = 0; i < static_cast<int>(legalMove.size()); i++) {
        cout << i << ": [" << legalMove[i].grid.y << "," << legalMove[i].grid.x << "]" << endl;
    }

    int depth;
    int depthLimit = NUMGRIDS - board.discOnBoard;
    int score = 0;
    int moveIdx = 0;
    int prevIdx = 0;
    int randMove;
    int alpha, beta;
    for (depth = 0; depth < depthLimit &&
                    this->stopTimer(startTime) < STOPTIME * board.timeLimit; depth++) {
        alpha = INT_MIN;
        beta = INT_MAX;
        randMove = 1;
#ifdef DEBUG
        cout << "Depth: " << depth << " Limit: " << depthLimit << endl;
#endif
        for (int i = 0; i < static_cast<int>(legalMove.size()); i++) {
            Board testBoard = board;
            testBoard.UpdateBoard(legalMove[i]);
            score = AlphaBetaPruning(testBoard, depth, startTime, alpha, beta, false);
            if (this->stopTimer(startTime) > STOPTIME * board.timeLimit) {
                moveIdx = prevIdx;
                break;
            }
#ifdef DEBUG
            cout << "I: " << i << " Score: " << score << " Alpha: " << alpha << endl;
#endif
            if (score > alpha) {
                moveIdx = i;
                alpha = score;
            } else if (score == alpha) {
                if (((randomizer() % randMove++) - 1) == 0) {
                    moveIdx = i;
                }
            }
        }
        prevIdx = moveIdx;
    }
#ifdef DEBUG
    cout << "Score: " << score << " Alpha: " << alpha << endl;
#endif
    move = legalMove[moveIdx];
    cout << "Depth: " << depth << " in " <<
         this->stopTimer(startTime) << " seconds" << endl;
    cout << "Choose move " << moveIdx << " ["
         << move.grid.y << ", " << move.grid.x << "]" << endl;
    return legalMove[moveIdx];

}

int Player::AlphaBetaPruning(Board board, int depth,
                             std::chrono::time_point<std::chrono::system_clock> startTime,
                             int alpha, int beta, bool maxP) {
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
            cout << " Pass " << endl;
            return heuristic.Minimax_Heuristic(testBoard);
        } else {
            Board testBoard = board;
            testBoard.pass[0] = false;
            testBoard.pass[1] = true;
            return AlphaBetaPruning(testBoard, depth, startTime,
                                    alpha, beta, !maxP);
        }
    }

    if (maxP) {
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