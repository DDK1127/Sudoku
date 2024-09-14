#include <bits/stdc++.h>
using namespace std;

#define BLANK_TO_ZERO(ch) ((ch) == ' ' ? 0 : ((ch)))

class sudoku
{
private:
    vector<vector<int> > arr;
    bool find;

public:
    sudoku() : arr(9, vector<int>(9)), find(false) {};
    sudoku(vector<vector<int> > &v) { arr = v, find = false; }
    bool input();
    bool check_input();
    void example_input();
    void matrix_output();
    vector<int> possible_list(int row, int col);
    pair<int, int> next_location(int row, int col);
    void DFS(int row, int col);
};

bool sudoku::input(){
    int n;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            cin >> n;
            arr[i][j] = n;
        }
    }
    if (check_input())
        return true;
    else
        return false;
};

bool sudoku::check_input(){
    vector<bool> row_list(10), col_list(10);
    for (int i = 0; i < 9; i++){
        for (int s = 0; s <= 9; s++)
            row_list[s] = false, col_list[s] = false;

        for (int j = 0; j < 9; j++){
            if (arr[i][j] != 0){
                if (row_list[arr[i][j]]){
                    cout << "Error Location: " << i + 1 << ", " << j + 1 << endl;
                    return false;
                }
                else if (!row_list[arr[i][j]])
                    row_list[arr[i][j]] = true;
            }
            if (arr[j][i] != 0){
                if (col_list[arr[j][i]]){
                    cout << "Error Location: [" << i + 1 << ", " << j + 1 << "]" << endl;
                    return false;
                }
                else if (!col_list[arr[j][i]])
                    col_list[arr[j][i]] = true;
            }
        }
    }
    return true;
};

void sudoku::example_input(){
    cout << "input example => " << endl
         << endl;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++)
            cout << " " << (i + 1) * (j + 1) % 10 << " ";
        cout << endl;
    }
    cout << "===========================" << endl;
};

void sudoku::matrix_output(){
    cout << endl;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++)
            cout << " " << arr[i][j] << " ";
        cout << endl;
    }
    cout << "===========================" << endl;
};

vector<int> sudoku::possible_list(int row, int col){
    vector<bool> list(10, true);
    vector<int> possible;
    for (int i = 0; i < 9; i++){ // check rows & cols
        list[arr[row][i]] = false;
        list[arr[i][col]] = false;
    }
    for (int i = 0; i < 3; i++){ // check block.
        for (int j = 0; j < 3; j++)
            list[arr[3 * (row / 3) + i][3 * (col / 3) + j]] = false; // partition 9 block => (row/3, col/3)
    }
    for (int i = 1; i <= 9; i++){
        if (list[i])
            possible.push_back(i);
    }
    return possible;
};

pair<int, int> sudoku::next_location(int row, int col){
    // row major.
    while (row < 9){
        // cout << "while >>> row = " << row << ", col = " << col << endl; 
        if (col > 8)
            ++row, col = 0;
        else if (arr[row][col] == 0)
            return make_pair(row, col);
        else
            col++;
    }
    return make_pair(-1, -1);
};

void sudoku::DFS(int row, int col){
    if (row == -1){
        find = true; // solve.
        cout << "Result Matrix => " << endl;
        matrix_output();
        return;
    }
    pair<int, int> next;
    vector<int> list = possible_list(row, col);

    // for (int i = 0; i < list.size(); i++)
    //     cout << "list = [" << list[i] << "], " ;
    // cout << "row = " << row << ", col = " << col << endl;

    for (int i = 0; i < list.size(); i++){
        arr[row][col] = list[i];
        // matrix_output();
        next = next_location(row, col);
        DFS(next.first, next.second); 
        if (find) // find it stop recursion.
            return;
    }
    arr[row][col] = 0; // not found!, recover it.
};

int main(){
    // vector<vector<int> > input_1 {  // Easy.
    //     {2, 4, 0, 0, 0, 0, 1, 0, 7},
    //     {6, 0, 8, 9, 1, 5, 3, 0, 2},
    //     {9, 0, 0, 0, 2, 7, 0, 6, 0},
    //     {0, 9, 7, 1, 3, 2, 6, 0, 5},
    //     {0, 0, 0, 5, 0, 8, 0, 3, 4},
    //     {5, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {7, 0, 2, 3, 0, 9, 8, 0, 1},
    //     {0, 0, 0, 8, 0, 0, 0, 0, 0},
    //     {0, 1, 9, 0, 0, 0, 4, 7, 0}
    // };
    // sudoku S(input_1);
    // cout << " Original Matrix =>" << endl;
    // S.matrix_output();
    // S.DFS(0, 2);

    vector<vector<int> > input_2 {  // Hard.
        {7, 0, 0, 1, 0, 0, 0, 0, 9},
        {0, 2, 0, 3, 0, 0, 7, 0, 0},
        {4, 0, 9, 0, 0, 0, 0, 0, 0},
        {0, 6, 0, 8, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 0, 1, 0, 5, 0},
        {0, 0, 0, 0, 0, 4, 9, 0, 0},
        {0, 4, 6, 0, 0, 5, 0, 0, 2},
        {0, 1, 0, 0, 0, 6, 8, 0, 0}
    };
    sudoku S2(input_2);
    cout << " Original Matrix =>" << endl;
    S2.matrix_output();
    S2.DFS(0, 1);

    return 0;
}

/*
    2 4 0 0 0 0 1 0 7
    6 0 8 9 1 5 3 0 2
    9 0 0 0 2 7 0 6 0
    0 9 7 1 3 2 6 0 5
    0 0 0 5 0 8 0 3 4
    5 0 0 0 0 0 0 0 0
    7 0 2 3 0 9 8 0 1
    0 0 0 8 0 0 0 0 0
    0 1 9 0 0 0 4 7 0
    =================
    2 4 0 | 0 0 0 | 1 0 7
    6 0 8 | 9 1 5 | 3 0 2
    9 0 0 | 0 2 7 | 0 6 0
    ---------------------
    0 9 7 | 1 3 2 | 6 0 5
    0 0 0 | 5 0 8 | 0 3 4
    5 0 0 | 0 0 0 | 0 0 0
    ---------------------
    7 0 2 | 3 0 9 | 8 0 1
    0 0 0 | 8 0 0 | 0 0 0
    0 1 9 | 0 0 0 | 4 7 0
*/