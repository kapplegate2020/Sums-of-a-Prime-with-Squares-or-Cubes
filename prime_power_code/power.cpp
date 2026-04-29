#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <chrono>
#include <omp.h>

using namespace std;
namespace fs = filesystem;
typedef long long ll;

const ll W = 64;
std::vector<uint64_t> primeBitmaps;
std::vector<int> primes;
ll primeBitmapsLength;

ll rootFloor(ll n, int power) {
    if (n == 0 || n == 1) {
        return n;
    }

    ll low = 1;
    ll high = 1<<(64/power-2);
    ll ans = 1;

    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (pow(mid, power) == n) { // Use long long to prevent overflow for mid*mid
            return mid;
        }
        if (pow(mid, power) < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

// Load primes from a file
void load_primes(const std::string& filename) {
    ifstream file(filename);
    uint64_t currentPrimeBitmap = 0ULL;
    while (file >> currentPrimeBitmap) {
        primeBitmaps.push_back(currentPrimeBitmap);
    }
}

vector<ll> checkRange(ll k, int power) {
    ll startEven = 2 * k * W + 2;
    ll startOdd = 2 * k * W + 1;
    
    uint64_t evenBitmap = 0ULL;
    uint64_t oddBitmap = 0ULL;

    ll cube = rootFloor(2*(k+1)*W, power);
    ll evenCube = 2*(cube/2);
    ll oddCube = 2*((cube+1)/2)-1;

    // cout << bitset<64>(evenBitmap) << endl;


    ll sq = pow(oddCube, power);
    while (sq >= startOdd && oddCube > 0){
        int l = (sq-startOdd+1)/2;
        oddBitmap |= (1ULL<<((sq-startOdd)/2));
        if(l!=64){
            evenBitmap |= (primeBitmaps[0] << l);
        }
        oddCube -= 2;
        sq = pow(oddCube, power);
    }
    //cout << "here" <<  bitset<64>(oddBitmap) << endl;
    
    sq = pow(evenCube, power);
    while (sq >= startEven && evenCube > 0){
        int l = (sq-startOdd+1)/2;
        evenBitmap |= (1ULL<<((sq-startEven)/2));
        if(l!=64){
            oddBitmap |= (primeBitmaps[0] << l);
        }
        evenCube -= 2;
        //cout << sq << " " << l << " " << bitset<64>(oddBitmap) << endl;
        sq = pow(evenCube, power);
    }
    //cout << bitset<64>(oddBitmap) << endl;
    //cout << "switch" << endl;

    while (oddCube > 0 && ~evenBitmap!=0){
        sq = pow(oddCube, power);
        ll dif = (startEven-sq-1)/(2*W);
        if(dif>=primeBitmapsLength){
            cout << "ran out" << endl;
            break;
        }
        ll offset = ((sq+1+2*(dif+1)*W-startEven)/2);
        evenBitmap |= (primeBitmaps[dif] >> (W-offset));
        if(offset!=64){
            evenBitmap |= (primeBitmaps[dif+1] << (offset));
        }
        oddCube -= 2;
        // cout << bitset<64>(evenBitmap) << endl;
    }
    while (evenCube > 0 && ~oddBitmap!=0){
        sq = pow(evenCube, power);
        ll dif = (startOdd-sq-1)/(2*W);
        if(dif>=primeBitmapsLength){
            break;
        }
        ll offset = ((sq+1+2*(dif+1)*W-startOdd)/2);
        oddBitmap |= (primeBitmaps[dif] >> (W-offset));
        //cout <<sq << " " <<  bitset<64>(oddBitmap) << endl;
        if(offset!=64){
            oddBitmap |= (primeBitmaps[dif+1] << (offset));
            //cout << sq << " " << bitset<64>(oddBitmap) << endl;
        }
        evenCube -= 2;
    }

    vector<ll> exceptions;
    if(~evenBitmap!=0){
        for(int i=0; i<W; i++){
            if(!((evenBitmap>>i)&1ULL)){
                exceptions.push_back(startEven+2*i);
            }
        }
    }
    if(~oddBitmap!=0){
        for(int i=0; i<W; i++){
            if(!((oddBitmap>>i)&1ULL)){
                exceptions.push_back(startOdd+2*i);
            }
        }
    }


    return exceptions;
}

int main(int argc, char *argv[]) {
    /*
    arg1 (int) is the power (square, cube, fourth, etc)
    arg2 (int) is the power of 10 to compute up to (ie 6 means compute up to 10^6)
    arg3 (int) tells upper bounds from prime bitmaps (ie 6 means use all primes up to 10^6) 
        the prime bitmaps must be generated before hand
    */

    auto start = chrono::high_resolution_clock::now();

    int power = stoi(argv[1]);
    ll N = pow(10, stoi(argv[2]));
    string bitMapLevel = argv[3];
    ll n = N/128+1;
    ll sep = n/100;
    string destinationFile = argv[4];
    if(argc == 5){
        destinationFile = argv[4];
    }

    load_primes("primes/bitmaps"+bitMapLevel+".txt");
    primeBitmapsLength = primeBitmaps.size();

    fs::create_directory("temp");

    //cout << "to go:" << (n/sep) << endl;

    #pragma omp parallel for schedule(dynamic)
    for(ll i=0; i<n; i++){
        if(i%sep == 0){
            if((i/sep)%10 == 0){
                cout << (i/sep)/10 << " ";
            }
            cout << (i/sep)%10 << flush;
            if((i/sep)%10 == 9){
                cout << endl;
            }
        }
        vector<ll> exceptions = checkRange(i, power);
        if(!exceptions.empty()){
            ofstream out("temp/out_" + std::to_string(i) + ".txt");
            for(ll e: exceptions){
                out << e << " ";
            }
            //cout << endl;
        }
    }

    
    vector<ll> all_exceptions;
    for (const auto &entry : fs::directory_iterator("temp")) {
        if (entry.is_regular_file()) {
            std::ifstream in(entry.path());
            ll val;
            while (in >> val) {
                all_exceptions.push_back(val);
            }
        }
    }

    cout << "\nnumber of exceptions " << all_exceptions.size() << endl;

    sort(all_exceptions.begin(), all_exceptions.end());

    ofstream out;
    out.open(destinationFile);
    for(auto x:all_exceptions){
        out << x << "\n";
    }

    fs::remove_all("temp");

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}