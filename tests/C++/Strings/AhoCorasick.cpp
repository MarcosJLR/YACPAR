#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

#include "../../../src/C++/Strings/AhoCorasick.hpp"

string randString(int n){
    string s(n, 'x');
    for(int i = 0; i < n; i++){
        s[i] = 'a' + (rand() % 5);
    }
    return s;
}

vector <pair <int, int>> NaiveMatch(vector<string>& pats, string& text)
{
    vector <pair <int, int>> result;
    for(int i = 0; i < (int) text.size(); i++){
        for(int w = 0; w < (int) pats.size(); w++){
            string &p = pats[w];
            if(text.substr(i, p.size()) == p){
                result.push_back({ w, i });
            }
        }
    }

    return result;
}

bool test(vector<string>& pats, string& text)
{
    AhoCorasick ac;
    for(string& p : pats) ac.add(p); 
    auto result = ac.feed(text);
    auto naive = NaiveMatch(pats, text);

    sort(result.begin(), result.end());
    sort(naive.begin(), naive.end());

    if(result.size() != naive.size()) return false;
    for(int i = 0; i < (int) result.size(); i++){
        if(result[i] != naive[i])
            return false;
    }

    return true;
}

int main(){
    
    srand(time(NULL));

    vector <int> sizes = { 10, 100, 1000, 300000 };

    for(int k = 0; k < 10; k++){
        for(int sz : sizes){
            int acum = 0;
            vector <string> pats;
            set <string> uPats;
            string text = "";
            while(acum < sz){
                string s = randString(rand() % 10 + 1);
                if(uPats.count(s)) continue;
                pats.push_back(s);
                uPats.insert(s);
                acum += pats.back().size();
                if(rand() % 2) text += pats.back();
            }

            if(sz <= 1000) assert(test(pats, text));
            else{
                auto start = high_resolution_clock::now();

                AhoCorasick ac;
                for(string& p : pats) ac.add(p); 
                auto result = ac.feed(text);

                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop-start);
                
                cout << "Test with sum of lengths: " << acum << endl;
                cout << "Number of Matches: " << result.size() << endl;
                cout << "Processed in " << duration.count();
                cout << " milliseconds" << endl << endl;

            }
        }
    }

    int acum = 0;
    vector <string> pats;
    string text = "";
    for(int i = 1; acum < sizes.back(); i++){
        string s = "";
        for(int j = 1; j <= i; j++) s += 'a';
        pats.push_back(s);
        acum += i;
    }
    text = pats.back();

    auto start = high_resolution_clock::now();

    AhoCorasick ac;
    for(string& p : pats) ac.add(p); 
    auto result = ac.feed(text);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop-start);
    
    cout << "Corner case test with sum of lengths: " << acum << endl;
    cout << "Number of Matches: " << result.size() << endl;
    cout << "Processed in " << duration.count();
    cout << " milliseconds" << endl << endl;

    
    cout << "Test passed!" << endl;


    return 0;
}
