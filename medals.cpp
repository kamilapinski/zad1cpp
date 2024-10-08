#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <array>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <regex>
using namespace std;

using type = int32_t;
using integer = int64_t;

#define AMOUNT_OF_TYPES 4
#define MIN_WEIGHT 1
#define MAX_WEIGHT 999999

bool cmp(pair<string, integer> p1, pair<string, integer> p2) {
    if (p1.second == p2.second) {
        if (p1.first.compare(p2.first) < 0)
            return true;
        else return false;
    }
    else return p1.second > p2.second;
}

void print_error(integer line_number) {
    cerr << "ERROR " << line_number << "\n";
}

// poprawić, żeby było łatwo zmieniać
bool is_medal_type_correct(const string& line) {
    regex Type(".*[[:space:]][0-3]");
    return regex_match(line, Type);
}

pair<string, type> medal_data(const string& line) {
    string country = line;
    type medal = (type)(country.back() - '0');

    country.pop_back();

    if (country.back() == ' ')
        country.pop_back();
    else
        country = "";

    return make_pair(country, medal);
}

bool update_medals(const string& line, array<map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries, integer amount) {
    pair <string, type> medal = medal_data(line);

    if (amount >= 0) { 
        Countries.insert(medal.first);
    }

    Medals[medal.second][medal.first] += amount;

    if (Medals[medal.second][medal.first] < 0) {
        Medals[medal.second][medal.first] -= amount;
        return false;
    }

    return true;
}

vector<pair<string, integer>> get_rating(array<integer, AMOUNT_OF_TYPES>& Weights, array<map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries) {

    vector<pair<string, integer>> rating;

    for (string country : Countries) {
        pair<string, integer> position;

        position.first = country;
        position.second = 0;
        for (type i = 1; i < AMOUNT_OF_TYPES; i++) {
            position.second += Weights[i] * Medals[i][country];
        }

        rating.push_back(position);
    }

    sort(rating.begin(), rating.end(), cmp);

    return rating;
}

bool print_rating(const string& line, array<map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries) {
    array<integer, AMOUNT_OF_TYPES> Weights;

    stringstream ss = stringstream(line);

    bool correct = true;

    for (type i = 1; i < AMOUNT_OF_TYPES; i++) {
        ss >> Weights[i];
        if (Weights[i] < MIN_WEIGHT || Weights[i] > MAX_WEIGHT)
            correct = false;
    }
    
    if (correct) {
        vector<pair<string, integer>> rating = get_rating(Weights, Medals, Countries);

        integer n = 0, last_score = -1, place = 0;
        for (pair<string, integer> position : rating) {
            n++;
            if (last_score != position.second)
                place = n;
            cout << place << ". " << position.first << "\n";
            last_score = position.second;
        }
    }

    return correct;
}

int main() {
    string line;
    integer line_number = 1;

    array<map<string, integer>, AMOUNT_OF_TYPES> Medals;
    unordered_set<string> Countries;

    string rating_syntax = "=";

    for (int i = 1; i < AMOUNT_OF_TYPES - 1; i++) {
        rating_syntax.append("[1-9][0-9]*[[:space:]]");
    }
    rating_syntax.append("[1-9][0-9]*");

    regex Rating(rating_syntax);
    regex Add("[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]](0|[1-9][0-9]*)");
    regex Minus("-[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]](0|[1-9][0-9]*)");

    while (getline(cin, line)) {

        if (regex_match(line, Rating)) {
            line.erase(0, 1);
            if (!print_rating(line, Medals, Countries))
                print_error(line_number);
        }   
        else if (!is_medal_type_correct(line)) {
            print_error(line_number);
        }
        else if (regex_match(line, Minus)) {
            line.erase(0, 1);
            if (!update_medals(line, Medals, Countries, -1))
                print_error(line_number);
        }
        else if (regex_match(line, Add)){
            if (!update_medals(line, Medals, Countries, 1))
                print_error(line_number);
        }
        else {
            print_error(line_number);
        }  

        line_number++;
    }

}