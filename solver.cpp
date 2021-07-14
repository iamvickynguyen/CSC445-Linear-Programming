#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <limits>
using namespace std;
typedef long double ld;

vector<string> basic;
vector<string> nonbasic;
vector<vector<ld>> LP;
ld INF = numeric_limits<double>::max();

void init_LP(vector<vector<ld>> input) {
    int n = input[0].size();
    nonbasic.push_back("dummy");
    for (int i = 1; i <= n; i++) {
        nonbasic.push_back("x" + to_string(i));
    }

    basic.push_back("dummy");
    for (int i = 1; i < input.size(); i++) {
        basic.push_back("w" + to_string(i));
    }

    vector<ld> objrow;
    objrow.push_back(0);
    for (int i = 0; i < n; i++) {
        objrow.push_back(input[0][i]);
    }
    LP.push_back(objrow);

    for (int i = 1; i < input.size(); i++) {
        vector<ld> row;
        row.push_back(input[i][n]);
        for (int j = 0; j < n; j++) {
            row.push_back(-input[i][j]);
        }
        LP.push_back(row);
    }
}

bool is_optimal() {
    for (int i = 1; i < LP[0].size(); i++) {
        if (LP[0][i] > 0) return false;
    }

    for (int i = 1; i < LP.size(); i++) {
        if (LP[i][0] < 0) return false;
    }
    return true;
}

bool is_feasible() { // FIXME
    for (int i = 1; i < LP.size(); i++) {
        if (LP[i][0] < 0) return false;
    }
    return true;
}

bool is_unbounded() { //FIXME
    if (!is_feasible())
        return false;

    for (int i = 1; i < LP[0].size(); i++) {
        if (LP[0][i] > 0) {
            bool negative = false;
            for (int j = 1; j < LP.size(); j++) {
                if (LP[j][i] < 0) {
                    negative = true;
                }
            }
            if (!negative) return true;
        }
    }

    return false;
}

void output_optimal() {
    cout << "optimal\n" << LP[0][0] << "\n";

    vector<pair<string, ld>> ans;
    for (int i = 1; i < nonbasic.size(); i++) {
        if (nonbasic[i][0] == 'x') {
            ans.push_back({nonbasic[i], 0});
        }
    }

    for (int i = 1; i < basic.size(); i++) {
        if (basic[i][0] == 'x') {
            ans.push_back({basic[i], LP[i][0]});
        }
    }

    sort(ans.begin(), ans.end());
    for (auto item : ans) {
        cout << item.second << " ";
    }
    cout << "\n";
}

void pivot(int row, int col) {
    swap(nonbasic[col], basic[row]);

    // basic row
    ld denominator = - LP[row][col];
    for (int i = 0; i < LP[row].size(); i++) {
        LP[row][i] = i == col ? (-1.0)/denominator : LP[row][i]/denominator;
    }

    // other rows
    for (int i = 0; i < LP.size(); i++) {
        if (i != row) {
            ld coef = LP[i][col];
            for (int j = 0; j < LP[i].size(); j++) {
                LP[i][j] = LP[row][j] * coef + (j == col ? 0 : LP[i][j]);
            }  
        }
    }
}

bool largest_coefficient_rule() {
    ld maxval = 0;
    int var_enter = -1;
    for (int i = 1; i < LP[0].size(); i++) {
        if (LP[0][i] > maxval) {
            maxval = LP[0][i];
            var_enter = i;
        }
    }

    ld minval = INF;
    int var_leave = -1;
    for (int i = 1; i < LP.size(); i++) {
        if (LP[i][var_enter] < 0) {
            ld val = LP[i][0] / (-LP[i][var_enter]);
            if (val < minval) {
                minval = val;
                var_leave = i;
            }
        }
    }

    // check unbounded
    if (var_leave == -1) {
        return false;
    }

    pivot(var_leave, var_enter);
    return true;
}

void printLP() {
    cout << "nonbasic: ";
    for (auto item : nonbasic) {
        cout << item << " ";
    }
    cout << "\n";

    cout << "basic: ";
    for (auto item : basic) {
        cout << item << " ";
    }
    cout << "\n";

    for (auto row : LP) {
        for (auto item : row) {
            cout << item << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);

    // read input
    vector<vector<ld>> input;
    string line;
    getline(cin, line);
    while (!line.empty()) {
        vector<ld> row;
        istringstream ss(line);
        string word;
        while (ss >> word) {
            row.push_back(stod(word));
        }
        input.push_back(row);
        getline(cin, line);
    }

    init_LP(input);

    // solve
    while (!is_optimal()) {
        if (!largest_coefficient_rule()) { // unbounded
            cout << "unbounded\n";
            return 0;
        }
    }

    output_optimal();

    return 0;
}