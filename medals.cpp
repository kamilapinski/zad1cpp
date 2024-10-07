#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <tuple>
#include <array>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <regex>
using namespace std;

#define AMOUNT_OF_MEDALS 3

bool cmp(pair<int, string> p1, pair<int, string> p2) {
    return p1.first > p2.first;
}

void print_error(int line_number) {
    cerr << "ERROR " << line_number << '\n';
}

pair<string, int> medal_data(string line) {
    string country = line;
    int medal = (int)(country.back() - '0');

    if (medal < 0 || medal > 3) medal = -1;

    country.pop_back();

    if (country.back() == ' ')
        country.pop_back();
    else
        country = "";

    return make_pair(country, medal);
}

bool is_country_name_valid(string name) {
    bool valid = true;
    int letters = 0;

    if (name[0] < 'A' || name[0] > 'Z') valid = false;

    for (auto c : name) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) letters++;
    }

    if (letters < 2) valid = false;

    return valid;
}

bool is_medal_valid(pair <string, int> medal) {
    if (medal.second == -1 || is_country_name_valid(medal.first) == false) return false;
    else return true;
}

void add_medal(string line, int line_number, array <map<string, int>, 3>& Medals, unordered_set<string>& Countries) {
    pair <string, int> para = medal_data(line);

    if (is_medal_valid(para) == false) {
        print_error(line_number);
        return;
    }

    Countries.insert(para.first);

    Medals[para.second - 1][para.first]++;
}

void remove_medal(string line, int line_number, array <map<string, int>, 3> Medals, unordered_set<string> Countries) {
    pair <string, int> para = medal_data(line);

    if (is_medal_valid(para) == false) {
        print_error(line_number);
        return;
    }

    Medals[para.second - 1][para.first]--;
}

vector<pair<int, string>> Rating(array <int, 3> Weights, array <map<string, int>, 3>& Medals, unordered_set<string>& Countries) {

    vector<pair<int, string>> Rating;

    for (string country : Countries) {
        pair<int, string> position;

        position.second = country;
        position.first = 0;
        for (int i = 0; i < AMOUNT_OF_MEDALS; i++) {
            position.first += Weights[i] * Medals[i][country];
        }

        Rating.push_back(position);
    }

    sort(Rating.begin(), Rating.end(), cmp);

    return Rating;
}

void print_rating(string line, int line_number, array <map<string, int>, 3>& Medals, unordered_set<string>& Countries) {

    array <int, 3> Weights;

    stringstream ss = stringstream(line);

    int w;
    if (ss >> Weights[0] >> Weights[1] >> Weights[2] && !(ss >> w)) {
        vector<pair<int, string>> rating = Rating(Weights, Medals, Countries);

        int n = 0, last_score = -1;
        for (pair<int, string> position : rating) {
            if (last_score != position.first) n++;
            cout << n << " " << position.second << "\n";
        }
    }
    else {
        print_error(line_number);
    }

}

int main() {

    string line;
    int line_number = 1;

    array <map<string, int>, 3> Medals;
    unordered_set <string> Countries;

    regex Rating("=[0-9]+[:space:][0-9]+[:space:]+[0-9]+");
    regex Add("[A-Z][A-Za-z[:space:]]*[0-9]");
    regex Minus("-[A-Z][A-Za-z[:space:]]*[0-9]");

    while (getline(cin, line)) {

        if (regex_match(line, Rating)) {
            line.erase(0, 1);
            print_rating(line, line_number, Medals, Countries);
        }   
        else if (regex_match(line, Minus)) {
            line.erase(0, 1);
            remove_medal(line, line_number, Medals, Countries);
        }
        else if (regex_match(line, Add)){
            add_medal(line, line_number, Medals, Countries);
        }
        else {
            print_error(line_number);
        }  

        line_number++;
    }

}

// std::variant<T1, T2, ...> 
// nie można struct, union ani class

// std::ranges::sort(v)
// ranges - biblioteka, dużo rzeczy można robić