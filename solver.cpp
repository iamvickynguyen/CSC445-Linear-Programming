#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <limits>
#include <utility>
#include <unordered_map>
using namespace std;
typedef double ld;

vector<string> basic;
vector<string> nonbasic;
vector<vector<ld>> LP;
const ld INF = numeric_limits<ld>::max();
const ld EPS = 1e-8;

int zero_cmp(ld x) {
    if (x < -EPS) return -1;
    if (x > EPS) return 1;
    return 0;
}

void init_LP(vector<vector<ld>> input) {
    basic.clear();
    nonbasic.clear();
    LP.clear();
    
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

bool comparator(const vector<ld>& a, const vector<ld>& b) {
    for (int i = 0; i < min(a.size(), b.size()); i++) {
        if (zero_cmp(abs(a[i] - b[i])) != 0) {
            return a[i] < b[i];
        }
    }
    return a.size() <= b.size();
}

bool is_optimal() {
    for (int i = 1; i < LP[0].size(); i++) {
        if (zero_cmp(LP[0][i]) > 0) return false;
    }

    for (int i = 1; i < LP.size(); i++) {
        if (zero_cmp(LP[i][0]) < 0) return false;
    }
    return true;
}

bool is_infeasible() {
    for (int i = 1; i < LP.size(); i++) {
        if (zero_cmp(LP[i][0]) < 0) return true;
    }
    return false;
}

void output_optimal_primal() {
    printf("optimal\n");
    printf("%.7g\n", LP[0][0]);

    vector<pair<int, ld>> ans;
    for (int i = 1; i < nonbasic.size(); i++) {
        if (nonbasic[i][0] == 'x') {
            string index = nonbasic[i].substr(1);
            ans.push_back({stoi(index), 0});
        }
    }

    for (int i = 1; i < basic.size(); i++) {
        if (basic[i][0] == 'x') {
            string index = basic[i].substr(1);
            ans.push_back({stoi(index), LP[i][0]});
        }
    }

    sort(ans.begin(), ans.end());
    for (auto item : ans) {
        if (zero_cmp(item.second) == 0) {
            printf("0 ");
        } else {
            printf("%.7g ", item.second);  
        }
    }
    printf("\n");
}

void output_optimal_dual() {
    printf("optimal\n");
    printf("%.7g\n", -LP[0][0]);

    vector<pair<int, ld>> ans;
    for (int i = 1; i < nonbasic.size(); i++) {
        if (nonbasic[i][0] == 'x') {
            string index = nonbasic[i].substr(1);
            ans.push_back({stoi(index), -LP[0][i]});
        }
    }

    for (int i = 1; i < basic.size(); i++) {
        if (basic[i][0] == 'x') {
            string index = basic[i].substr(1);
            ans.push_back({stoi(index), 0});
        }
    }

    sort(ans.begin(), ans.end());
    for (auto item : ans) {
        if (zero_cmp(item.second) == 0) {
            printf("0 ");
        } else {
            printf("%.7g ", item.second);  
        }
    }
    printf("\n");
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

    // find largest coefficient non basic variable
    ld maxval = 0;
    int var_enter = -1;
    for (int i = 1; i < LP[0].size(); i++) {
        if (LP[0][i] > maxval) {
            maxval = LP[0][i];
            var_enter = i;
        }
    }

    // find minimum bound for leaving variable
    ld minval = INF;
    unordered_map<ld, vector<ld>> minrows;
    for (int i = 1; i < LP.size(); i++) {
        if (zero_cmp(LP[i][var_enter]) < 0) {
            ld val = LP[i][0] / (-LP[i][var_enter]);
            if (val <= minval) {
                minval = val;
                minrows[val].push_back(i);
            }
        }
    }

    int var_leave = -1;

    if (minrows.count(minval) == 0) {
        return false; // unbounded
    } else if (minrows[minval].size() == 1) {
        var_leave = minrows[minval][0];
    } else {

        // get compared vector of all the minimum rows, sort them lexicographically
        // the basic variable corresponding to the first vector will be the leaving variable
        vector<vector<ld>> S;
        for (auto r : minrows[minval]) {
            vector<ld> v;
            for (int i = 1; i < LP[r].size(); i++) {
                ld val = LP[r][i] / (-LP[r][var_enter]);
                v.push_back(val);
            }
            v.push_back(r); // keep row reference
            S.push_back(v);
        }

        sort(S.begin(), S.end(), comparator);
        var_leave = S[0][S[0].size() - 1];
    }

    pivot(var_leave, var_enter);
    return true;
}

bool is_dual_feasible() {
    for (int i = 1; i < LP[0].size(); i++) {
        if (zero_cmp(LP[0][i]) > 0) {
            return false;
        }
    }
    return true;
}

void to_dual() {
    vector<vector<ld>> newLP;
    for (int i = 0; i < LP[0].size(); i++) {
        vector<ld> row;
        for (int j = 0; j < LP.size(); j++) {
            row.push_back(-LP[j][i]);
        }
        newLP.push_back(row);
    }
    LP = newLP;

    basic.swap(nonbasic);
}

void reconstruct_objective_row(vector<ld>& original_obj_row) {
    vector<ld> obj_row(LP[0].size(), 0);
    obj_row[0] = original_obj_row[0];

    // find basic rows containing x_i
    unordered_map<string, vector<ld>> rows;
    for (int i = 1; i < basic.size(); i++) {
        if (basic[i][0] == 'x') {
            rows[basic[i]] = LP[i];
        }
    }

    // substitute x_i expansion to the objective row
    for (int i = 1; i < original_obj_row.size(); i++) {
        string xi = "x" + to_string(i);
        if (rows.count(xi) > 0) {
            vector<ld> subrow = rows[xi];
            for (int j = 0; j < LP[0].size(); j++) {
                obj_row[j] += original_obj_row[i] * subrow[j];
            }
        } else {
            obj_row[i] += original_obj_row[i];
        }
    }

    LP[0] = obj_row;
}

bool feasiblize() {
    vector<ld> original_obj_row = LP[0];
    vector<string> basic_tmp = basic;
    vector<string> nonbasic_tmp = nonbasic;

    for (int i = 0; i < LP[0].size(); i++) {
        LP[0][i] = 0;
    }

    to_dual(); 

    while (!is_optimal()) {
        if (!largest_coefficient_rule()) { // no feasible point exists
            return false;
        }
    }

    to_dual();

    reconstruct_objective_row(original_obj_row);
    return true;
}

int main() {
    // read input
    vector<vector<ld>> input;
    string line;
    while (getline(cin, line)) {
        vector<ld> row;
        istringstream ss(line);
        string word;
        while (ss >> word) {
            row.push_back(stod(word));
        }
        input.push_back(row);
    }

    init_LP(input);

    bool is_primal = true;

    // if inital dictionary is infeasible, solve dual dictionary.
    // if both dual and primal are infeasible, transform objective row and find a feasible point
    if (is_infeasible()) {
        if (is_dual_feasible()) {
            is_primal = false;
            to_dual();
        } else {
            if (!feasiblize()) {
                printf("infeasible\n");
                return 0; 
            }
        }
    }

    // solve
    while (!is_optimal()) {
        if (!largest_coefficient_rule()) {
            if (is_primal)
                printf("unbounded\n");
            else
                printf("infeasible\n");
            return 0;
        }
    }

    if (is_primal) {
        output_optimal_primal();
    } else {
        output_optimal_dual();
    }

    return 0;
}