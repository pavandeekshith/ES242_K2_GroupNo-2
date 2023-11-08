//FINAL VERSION
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <string>

using namespace std;

vector<int> vertices = {4, 1, 5, 3};

enum Direction { LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };

struct State {
    vector<int> board;
    string moves;

    bool operator==(const State& other) const {
        return board == other.board;
    }

    // Calculate a hash for the state
    size_t hash() const {
        size_t result = 0;
        for (int i : board) {
            result = result * 10 + i;
        }
        return result;
    }
};

int findIndex(const vector<int>& board, int value) {
    for (int i = 0; i < 9; ++i) {
        if (board[i] == value) {
            return i;
        }
    }
    return -1; // Value not found in the board
}

void printBoard(const vector<int>& board) {
    for (int i = 0; i < 9; ++i) {
        if (board[i]==6) { cout << 'E' << " ";}
        else{
        cout << board[i] << " ";}
        if ((i + 1) % 3 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

vector<int> moveleft(const vector<int>& board, int index) {
    if (index == 2 || index == 5 || index == 8) {
        return board;
    }

    int i = board[index + 1];
    if (i >= 0 && i <= 3) {
        i = (i - 1 + 4) % 4;
    }
    vector<int> result = board;
    result[index + 1] = 6;
    result[index] = i;

    return result;
}

vector<int> moveright(const vector<int>& board, int index) {
    if (index == 0 || index == 3 || index == 6) {
        return board;
    }

    int i = board[index - 1];
    if (i >= 0 && i <= 3) {
        i = (i + 1) % 4;
    }
    vector<int> result = board;
    result[index - 1] = 6;
    result[index] = i;

    return result;
}

vector<int> moveup(const vector<int>& board, int index) {
    if (index == 6 || index == 7 || index == 8) {
        return board;
    }
    int i = board[index + 3];
    if (i == 5) {
        i = 1;
    } else if (i == 4 || i == 1 || i == 3) {
        i = vertices[(i - 1 + 4) % 4];
    }
    vector<int> result = board;
    result[index + 3] = 6;
    result[index] = i;

    return result;
}

vector<int> movedown(const vector<int>& board, int index) {
    if (index == 0 || index == 1 || index == 2) {
        return board;
    }
    int i = board[index - 3];
    if (i == 5) {
        i = 3;
    } else if (i == 4 || i == 1 || i == 3) {
        i = vertices[(i + 1) % 4];
    }
    vector<int> result = board;
    result[index - 3] = 6;
    result[index] = i;

    return result;
}

void performMovements(const vector<int>& start, const string& moves) {
    vector<int> board = start;
    for (char move : moves) {
        switch (move) {
            case 'L':
                cout << "Move LEFT\n"
                     << endl;
                board = moveleft(board, findIndex(board, 6));
                printBoard(board);
                break;
            case 'R':
                cout << "Move RIGHT\n"
                     << endl;
                board = moveright(board, findIndex(board, 6));
                printBoard(board);
                break;
            case 'U':
                cout << "Move UP\n"
                     << endl;
                board = moveup(board, findIndex(board, 6));
                printBoard(board);
                break;
            case 'D':
                cout << "Move DOWN\n"
                     << endl;
                board = movedown(board, findIndex(board, 6));
                printBoard(board);
                break;

            default:
                break;
        }
    }
}

vector<Direction> findPath(const vector<int>& src) {
    const vector<int> dest = {1, 1, 1, 1, 6, 1, 1, 1, 1};

    queue<State> q;
    unordered_set<size_t> visited;

    q.push({src, ""});
    visited.insert(State{src, ""}.hash());

    cout << "Processing..." << endl;
    cout << endl;

    while (!q.empty()) {
        State u = q.front();
        q.pop();

        if (u.board == dest) {
            vector<Direction> moves;
            for (char move : u.moves) {
                switch (move) {
                    case 'L':
                        moves.push_back(LEFT);
                        break;
                    case 'R':
                        moves.push_back(RIGHT);
                        break;
                    case 'U':
                        moves.push_back(UP);
                        break;
                    case 'D':
                        moves.push_back(DOWN);
                        break;

                    default:
                        break;
                }
            }
            return moves;
        }

        int index = findIndex(u.board, 6);
        State a = {moveup(u.board, index), u.moves + "U"};
        State b = {movedown(u.board, index), u.moves + "D"};
        State c = {moveleft(u.board, index), u.moves + "L"};
        State d = {moveright(u.board, index), u.moves + "R"};

        if (visited.find(a.hash()) == visited.end()) {
            q.push(a);
            visited.insert(a.hash());
        }
        if (visited.find(b.hash()) == visited.end()) {
            q.push(b);
            visited.insert(b.hash());
        }
        if (visited.find(c.hash()) == visited.end()) {
            q.push(c);
            visited.insert(c.hash());
        }
        if (visited.find(d.hash()) == visited.end()) {
            q.push(d);
            visited.insert(d.hash());
        }
    }

    return vector<Direction>{LEFT, RIGHT, UP, DOWN};
}

int main() {
    vector<int> board(9);
    int count=0;
    int k;
    int p=0;

    cout << "DECLARING CUBE FACES:\n0: Left, 1: Top, 2: Right, 3: Bottom, 4: Front, 5: Back, -1: None" << endl;
    cout << "Enter the Initial combination in a single line seperated by spaces:" << endl;
    for (int i = 0; i < 9; ++i) {
        cin >> k;
        if (k==-1){ board[i]=6;}
        else{
        board[i]=k;}
    }

    for (int i=0; i<9;++i){
        if (i!=4 && board[i]==1) p++;
    }

    cout << "\nInitial Board Configuration:" << endl;
    printBoard(board);

    if (board[4]==6 && p==8) {
        cout << "Board is already solved in 0 steps" <<endl;
        return 0;
    }


    vector<Direction> path = findPath(board);

    if (path.empty()) {
        cout << "No Solution Possible." << endl;
    } else {
        cout << "Solution Found!" << endl;

        string path_seq;
        for (const auto& direction : path) {
            switch (direction) {
                case LEFT:
                    path_seq = path_seq + "L ";
                    count++;
                    break;
                case RIGHT:
                    path_seq = path_seq + "R ";
                    count++;
                    break;
                case UP:
                    path_seq = path_seq + "U ";
                    count++;
                    break;
                case DOWN:
                    path_seq = path_seq + "D ";
                    count++;
                    break;
            }
        }

        cout << endl;
        performMovements(board, path_seq);
        cout << "Solution found in : " << count <<" moves \n";
        cout << "Solution : " << path_seq << endl;
    }

    return 0;
}

