#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <cstdint>
#include <omp.h>

using namespace std;
typedef long long ll;

const ll W = 64;
std::vector<uint64_t> primeBitmaps;
std::vector<int> primes;
std::vector<int> exceptions;
ll primeBitmapsLength;


// Load primes from a file
void load_primes(const std::string& filename) {
    std::ifstream file(filename);
    ll p;
    file >> p;
    cout << "excluded " << p << endl;
    uint64_t currentPrimeBitmap = 0ULL;
    ll counter = 0;
    while (file >> p) {
        if(p > 2*(counter+1)*W){
            primeBitmaps.push_back(currentPrimeBitmap);
            currentPrimeBitmap &= 0;
            counter++;
        }
        currentPrimeBitmap+=1ULL<<(p/2-counter*W);
    }
}

int main(int argc, char *argv[]) {
    /*
    arg1 (string) is the input file (list of primes)
    arg2 (string) is the output file (list of bitmaps) 
    */
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    load_primes(argv[1]);

    ofstream out;
    out.open(argv[2]);
    for(auto x:primeBitmaps){
        out << x << "\n";
    }

    return 0;
}