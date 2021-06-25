#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
using namespace std;

vector<string> basic;
vector<string> nonbasic;
vector<vector<float>> LP;

void init_LP(vector<vector<float>> input) {
    int n = input[0].size();
    nonbasic.push_back("dummy");
    for (int i = 1; i <= n; i++) {
        nonbasic.push_back("x" + to_string(i));
    }

    basic.push_back("dummy");
    for (int i = 1; i < input.size(); i++) {
        basic.push_back("w" + to_string(i));
    }

    vector<float> objrow;
    objrow.push_back(0);
    for (int i = 0; i < n; i++) {
        objrow.push_back(input[0][i]);
    }
    LP.push_back(objrow);

    for (int i = 1; i < input.size(); i++) {
        vector<float> row;
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
    return true;
}

void output_optimal() {
    cout << "optimal\n" << LP[0][0] << "\n";

    vector<pair<string, float>> ans;
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

int main() {
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);

    // read input
    vector<vector<float>> input;
    string line;
    getline(cin, line);
    while (!line.empty()) {
        vector<float> row;
        istringstream ss(line);
        string word;
        while (ss >> word) {
            row.push_back(stof(word));
        }
        input.push_back(row);
        getline(cin, line);
    }

    init_LP(input);

    // solve
    if (is_optimal()) {
        output_optimal();
    }


    return 0;
}