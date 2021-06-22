#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

vector<long double> getline_values(string line) {
    vector<long double> v;
    stringstream ss(line);
    string tok;
    while (getline(ss, tok, ' ')) {
        v.push_back(stold(tok));
    }
    return v;
}

int main() {
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);

    // get input
    string line;
    getline(cin, line);
    vector<long double> obj_function = getline_values(line);

    getline(cin, line);
    vector<vector<long double>> matrix;
    while (!line.empty()) {
        matrix.push_back(getline_values(line));
        getline(cin, line);
    }

    // solve
    // TODO

    return 0;
}