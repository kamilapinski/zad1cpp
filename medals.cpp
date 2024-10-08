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

using type = int32_t;
using integer = int64_t;

#define AMOUNT_OF_TYPES 3

#define dprint(x) cout << "D: " << x << "\n"


bool cmp(pair<string, integer> p1, pair<string, integer> p2) {
    if (p1.second == p2.second) {
        if (p1.first.compare(p2.first) < 0)
            return true;
        else return false;
    }
    else return p1.second > p2.second;
}

void print_error(integer line_number) {
    cerr << "ERROR " << line_number << '\n';
}

// poprawić, żeby było łatwo zmieniać
bool is_medal_type_correct(const string& line) {
    regex Type("[A-Za-z[:space:]]*[:space:][0-3]");
    return regex_match(line, Type);
}

pair<string, type> medal_data(const string& line) {
    string country = line;
    type medal = (type)(country.back() - '0');

    if (medal < 0 || medal > AMOUNT_OF_TYPES) medal = -1;

    country.pop_back();

    if (country.back() == ' ')
        country.pop_back();
    else
        country = "";

    return make_pair(country, medal);
}

// sprawdzona i poprawna funkcja
void update_medals(const string& line, array <map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries, integer amount) {
    // w tym momencie dzięki regex wiemy już, że line jest poprawny
    pair <string, type> medal = medal_data(line);

    if (amount >= 0) { 
        Countries.insert(medal.first);
    }

    Medals[medal.second - 1][medal.first]++;
}

vector<pair<string, integer>> get_rating(array <integer, AMOUNT_OF_TYPES>& Weights, array <map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries) {

    vector<pair<string, integer>> rating;

    for (string country : Countries) {
        pair<string, integer> position;

        position.first = country;
        position.second = 0;
        for (type i = 0; i < AMOUNT_OF_TYPES; i++) {
            position.second += Weights[i] * Medals[i][country];
        }

        rating.push_back(position);
    }

    sort(rating.begin(), rating.end(), cmp);

    return rating;
}

void print_rating(const string& line, array <map<string, integer>, AMOUNT_OF_TYPES>& Medals, unordered_set<string>& Countries) {
    // teraz też już wiemy, że line jest poprawny dzięki regexowi
    array <integer, AMOUNT_OF_TYPES> Weights;

    stringstream ss = stringstream(line);

    for (type i = 0; i < AMOUNT_OF_TYPES; i++) {
        ss >> Weights[i];
    }
    
    vector<pair<string, integer>> rating = get_rating(Weights, Medals, Countries);

    integer n = 0, last_score = -1;
    for (pair<string, integer> position : rating) {
        if (last_score != position.second) n++;
        cout << n << " " << position.first << "\n";
        last_score = position.second;
    }

}

int main() {

    string line;
    integer line_number = 1;

    array <map<string, integer>, AMOUNT_OF_TYPES> Medals;
    unordered_set <string> Countries;

    regex Rating("=[0-9]+[[:space:]][0-9]+[[:space:]]+[0-9]+");
    regex Add("[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]][0-9]+");
    regex Minus("-[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]][0-9]+");

    while (getline(cin, line)) {

        if (!is_medal_type_correct(line)) {
            print_error(line_number);
        }
        else if (regex_match(line, Rating)) {
            line.erase(0, 1);
            print_rating(line, Medals, Countries);
        }   
        else if (regex_match(line, Minus)) {
            line.erase(0, 1);
            update_medals(line, Medals, Countries, -1);
        }
        else if (regex_match(line, Add)){
            update_medals(line, Medals, Countries, 1);
        }
        else {
            print_error(line_number);
        }  

        line_number++;
    }

}

// co jeżeli będziemy próbować odebrać medal którego nie ma?

// std::variant<T1, T2, ...> 
// nie można struct, union ani class

// std::ranges::sort(v)
// ranges - biblioteka, dużo rzeczy można robić