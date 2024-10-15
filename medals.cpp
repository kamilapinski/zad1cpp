#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <array>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

using type = int32_t;
using size_type = size_t;
using medals_type = array<unordered_map<string, size_type>, AMOUNT_OF_TYPES>;
using pos_type = pair<string, size_type>;
using countries_set = unordered_set<string>;

constexpr int AMOUNT_OF_TYPES = 4;
constexpr int MIN_WEIGHT = 1;
constexpr int MAX_WEIGHT = 999999;

bool cmp(pos_type& p1, pos_type& p2) {
    if (p1.second == p2.second) {
        if (p1.first.compare(p2.first) < 0)
            return true;
        else return false;
    }
    else return p1.second > p2.second;
}

void print_error(size_type line_number) {
    cerr << "ERROR " << line_number << "\n";
}

bool is_medal_type_correct(const string& line) {

    stringstream ss(line);

    string word, good_word;
    while (ss >> word) {
        good_word = word;
    }
    
    type medal_type = stoi(good_word);
    
    if (medal_type >= 0 && medal_type < AMOUNT_OF_TYPES) return true;
    else return false;
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

bool update_medals(const string& line, medals_type& Medals, countries_set& Countries, size_type amount) {
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

vector<pos_type> get_rating(array<size_type, AMOUNT_OF_TYPES>& Weights, medals_type& Medals, countries_set& Countries) {

    vector<pos_type> rating;

    for (string country : Countries) {
        pos_type position;

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

bool print_rating(const string& line, medals_type& Medals, countries_set& Countries) {
    array<size_type, AMOUNT_OF_TYPES> Weights;

    stringstream ss = stringstream(line);

    bool correct = true;

    for (type i = 1; i < AMOUNT_OF_TYPES; i++) {
        ss >> Weights[i];
        if (Weights[i] < MIN_WEIGHT || Weights[i] > MAX_WEIGHT)
            correct = false;
    }
    
    if (correct) {
        vector<pos_type> rating = get_rating(Weights, Medals, Countries);

        size_type n = 0, last_score = -1, place = 0;
        for (pos_type position : rating) {
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
    size_type line_number = 1;

    medals_type Medals;
    countries_set Countries;

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

    return 0;
}