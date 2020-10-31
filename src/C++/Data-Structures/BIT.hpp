/**
 * Author: Marcos Lerones
 * Date: 31-10-2020
 * Description: Binary Index Tree to compute prefix sums in logN time
 * Time: O(logN) each operation
 * Status: Not Tested
 */

#pragma once

struct BIT{
    vector<long long> s;
    BIT(int n) : s(n+1, 0) {}

    // a[pos] += val
    void upd(int pos, ll val){
        pos++;
        for(; p < (int) s.size(); p += p & -p) s[p] += val;
    }

    // Sum of values in [0, pos]
    ll sum(int pos){
        pos++; 
        ll result = 0;
        for(; p; p -= p & -p) result += s[p];
        return result;
    }

    // Get min pos such that sum of [0, pos] >= x
    // Only works if all a[i] >= 0
    ll lower_bound(ll x){
        int pos = 0;
        for(int pw = 1 << 25; pw; pw >>= 1){
            if(pos + pw < (int) s.size() && s[pos + pw] < x)
                pos += pw, x -= s[pos];
        }
        return pos;
    }
};
