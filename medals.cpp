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

using id = int32_t;
using integer = int64_t;

#define AMOUNT_OF_MEDALS 3


// int używam dla 
//      medali id
//      numeru linii integer
//      liczby liter w stringu (trza zmienić)
//      ilości medali integer
//      wagi medali integer

bool cmp(pair<int, string> p1, pair<int, string> p2) {
    if (p1.first == p2.first) {
        if (p1.second.compare(p2.second) < 0)
            return true;
        else return false;
    }
    else return p1.first > p2.first;
}

void print_error(int line_number) {
    cerr << "ERROR " << line_number << '\n';
}

pair<string, int> medal_data(string& line) {
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

bool is_country_name_valid(string& name) {
    bool valid = true;
    int letters = 0;

    if (name[0] < 'A' || name[0] > 'Z') valid = false;

    for (auto c : name) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) letters++;
    }

    if (letters < 2) valid = false;

    return valid;
}

bool is_medal_valid(pair <string, int>& medal) {
    if (medal.second == -1 || is_country_name_valid(medal.first) == false) return false;
    else return true;
}

// sprawdzona i poprawna funkcja
void update_medals(string& line, array <map<string, integer>, AMOUNT_OF_MEDALS>& Medals, unordered_set<string>& Countries, integer amount) {
    // w tym momencie dzięki regex wiemy już, że line jest poprawny
    pair <string, id> medal = medal_data(line);

    if (amount > 0) Countries.insert(medal.first);

    Medals[medal.second - 1][medal.first]++;
}

vector<pair<int, string>> Rating(array <integer, 3>& Weights, array <map<string, integer>, 3>& Medals, unordered_set<string>& Countries) {

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

void print_rating(string& line, integer line_number, array <map<string, integer>, 3>& Medals, unordered_set<string>& Countries) {

    array <integer, 3> Weights;

    stringstream ss = stringstream(line);

    int w;
    if (ss >> Weights[0] >> Weights[1] >> Weights[2] && !(ss >> w)) {
        vector<pair<int, string>> rating = Rating(Weights, Medals, Countries);

        int n = 0, last_score = -1;
        for (pair<int, string> position : rating) {
            if (last_score != position.first) n++;
            cout << n << " " << position.second << "\n";
            last_score = position.first;
        }
    }
    else {
        print_error(line_number);
    }

}

int main() {

    string line;
    integer line_number = 1;

    array <map<string, integer>, 3> Medals;
    unordered_set <string> Countries;

    regex Rating("=[0-9]+[[:space:]][0-9]+[[:space:]]+[0-9]+");
    regex Add("[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]][0-9]+");
    regex Minus("-[A-Z][A-Za-z[:space:]]*[A-Za-z][[:space:]][0-9]+");

    while (getline(cin, line)) {

        if (regex_match(line, Rating)) {
            line.erase(0, 1);
            print_rating(line, line_number, Medals, Countries);
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