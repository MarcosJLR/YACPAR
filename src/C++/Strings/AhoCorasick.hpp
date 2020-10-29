/**
 * Author: Marcos Lerones
 * Date: 29-10-2020
 * 
 * Description: Aho Corasick Automaton used for multiple pattern matching
 * Initialize with AhoCorasick() and add each pattern with add function.
 * add(s) adds s to the trie and return the final node.
 * feed(s) feeds string s to the automaton and returns a list with 
 * all occurrences of patterns in the form (wordID, position).
 * slink(v) returns suffix link of node v. 
 * olink(v) returns output link of node v.
 * ADD ALL THE PATTERNS BEFORE CREATING SUFFIX LINKS!!!
 * 
 * Time: construction takes O(N), where N = sum of lenghts of patterns. 
 * feed(s) takes O(N + M) where N = |s| and M = number of matches 
 * M is bounded by O(N sqrt N) there are no duplicate patterns.
 * 
 * Status: Tested (no problems solved yet)
 */

#pragma once

struct AhoCorasick {
    struct node{
        int p, pch, slink, olink, wrd;
        map <int, int> nxt, go;

        node(int p = -1, int pch = -1)
            : p(p), pch(pch), slink(-1), olink(-1), 
              wrd(-1), nxt(), go()
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
    vector <pair <int, int>> feed(const string& s){
        vector <pair <int, int>> result;
        int v = 0;
        for(int i = 0; i < (int) s.size(); i++){
            int c = (int) s[i];
            v = go(v, c);
            int u = v;
            while(u){
                int w = nodes[u].wrd;
                if(w != -1){
                    result.push_back({ w, i - dict[w].size() + 1 });
                }
                u = olink(u);
            }
        }
        return result;
    }
};
