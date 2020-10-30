#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <math.h>

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

#define FOR(i,n,m) for(int i=n; i<(int)m; i++)
#define ROF(i,n,m) for(int i=n; i>(int)m; i--)
#define pb push_back
#define lb lower_bound
#define ub upper_bound
#define F first
#define S second
#define ri(a) scanf("%d",&a)
#define rii(a,b) ri(a),ri(b)
#define riii(a,b,c) rii(a,b),ri(c)
#define lri(a) scanf("%lld",&a)
#define lrii(a,b) lri(a),lri(b)
#define ms(obj,val) memset(obj,val,sizeof(obj))
#define all(x) x.begin(),x.end()
#define debug true
#define dprintf debug && printf

const int INF = 0x3f3f3f3f;
const ll INFLL = 1e18;
const int MOD = 1e9+7;
const int MAXN = 3e5+5;

struct AhoCorasick {
    struct node{
        int p, pch, slink, olink, wrd;
        map <int, int> nxt, go;
        multiset <int> vals;

        node(int p = -1, int pch = -1)
            : p(p), pch(pch), slink(-1), olink(-1), 
              wrd(-1), nxt(), go(), vals()
        {}
    };

    int sz;
    vector<node> nodes;
    vector<string> dict;

    AhoCorasick() : sz(1), nodes(1, node()), dict() {}

    // Returns final node of string s
    int add(const string& s){
        int v = 0;
        for(int i = 0; i < (int) s.size(); i++){
            int c = (int) s[i];
            if(!nodes[v].nxt.count(c)){
                nodes.push_back(node(v, c));
                nodes[v].nxt[c] = sz++;
            }
            v = nodes[v].nxt[c];
        }
        nodes[v].wrd = dict.size();
        nodes[v].vals.insert(0);
        dict.push_back(s);

        return v;
    }

    // Move from state v using transition c
    int go(int v, int c){
        if(!nodes[v].go.count(c)){
            if(nodes[v].nxt.count(c)) nodes[v].go[c] = nodes[v].nxt[c];
            else nodes[v].go[c] = v == 0 ? 0 : go(slink(v), c);
        }
        return nodes[v].go[c];
    }

    // Get suffix link of state v
    int slink(int v){
        if(nodes[v].slink == -1){
            int p = nodes[v].p, pch = nodes[v].pch;
            nodes[v].slink = v == 0 || p == 0 ? 0 : go(slink(p), pch);
        }
        return nodes[v].slink;
    }

    // Get output link of state v
    int olink(int v){
        if(nodes[v].olink == -1){
            if(v == 0 || nodes[v].p == 0)
                nodes[v].olink = 0;
            else{
                int s = slink(v);
                nodes[v].olink = 
                    nodes[s].wrd != -1 ? s : olink(s);
            }
        }
        return nodes[v].olink;
    }

    // Return vector of pairs (wordID, occurrencePosition)
    int feed(const string& s){
        int result = -1;
        int v = 0;
        for(int i = 0; i < (int) s.size(); i++){
            int c = (int) s[i];
            v = go(v, c);
            int u = v;
            while(u){
                int w = nodes[u].wrd;
                if(w != -1){
                    result = max(result, *nodes[u].vals.rbegin());
                }
                u = olink(u);
            }
        }
        return result;
    }
};

int val[MAXN];
int pos[MAXN];
int n, m;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ms(val, 0);
    AhoCorasick ac;

    cin >> n >> m;
    FOR(i,0,n){
        string s;
        cin >> s;
        pos[i] = ac.add(s);
    }

    while(m--){
        int t;
        cin >> t;
        if(t == 1){
            int i, x;
            cin >> i >> x;
            i--;
            int v = pos[i];
            auto it = ac.nodes[v].vals.find(val[i]);
            assert(it != ac.nodes[v].vals.end());
            ac.nodes[v].vals.erase(it);
            ac.nodes[v].vals.insert(x);
            val[i] = x;
        }
        else{
            string s;
            cin >> s;
            cout << ac.feed(s) << endl;
        }
    }


    return 0;
}
