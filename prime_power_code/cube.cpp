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

std::vector<uint64_t> primeBitmaps;
std::vector<int> primes;
ll primeBitmapsLength;


//Uses an efficient C implentation of Miller-Rabin primality test that is proven to be accurate up to 2^61
extern "C" {
int efficient_mr64(const uint64_t bases[], const int cnt, const uint64_t n);
}
uint64_t bases[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
int cnt = 9;
bool isPrime(ll n){
    if(n==2) return true;
    if(n%2==0) return false;
    uint64_t unsigned_n = static_cast<uint64_t>(n);
    return efficient_mr64(bases, cnt, unsigned_n);
}


//binary search for x such that x^power <= n < (x+1)^power
ll cubeRootFloor(ll n) {
    if (n == 0 || n == 1) {
        return n;
    }

    ll low = 1;
    ll high = 1<<19;
    ll ans = 1;

    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (mid*mid*mid == n) { // Use long long to prevent overflow for mid*mid
            return mid;
        }
        if (mid*mid*mid < n) {
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

//check for exceptions between in the range [128*k+1, 128*(k+1)]
vector<ll> checkRange(ll k) {
    ll startEven = 128 * k + 2;
    ll startOdd = 128 * k + 1;
    
    uint64_t evenBitmap = 0ULL;
    uint64_t oddBitmap = 0ULL;

    ll largestRoot = cubeRootFloor(128*(k+1));
    ll evenRoot = 2*(largestRoot/2);
    ll oddRoot = 2*((largestRoot+1)/2)-1;

    //apply the prime bitmap plus nextPower to evenBitmap for all odd nextPowers that are greater than 128*k+1
    //also, cross out nextPower from the oddBitmap
    ll nextPower = oddRoot*oddRoot*oddRoot;
    while (nextPower >= startOdd && oddRoot > 0){
        int l = (nextPower-startOdd+1)/2;
        oddBitmap |= (1ULL<<((nextPower-startOdd)/2));
        if(l!=64){
            evenBitmap |= (primeBitmaps[0] << l);
        }
        oddRoot -= 2;
        nextPower = oddRoot*oddRoot*oddRoot;
    }
    
    //apply the prime bitmap plus nextPower to oddBitmap for all even nextPowers that are greater than 128*k+1
    //also, cross out nextPower from the evenBitmap
    nextPower = evenRoot*evenRoot*evenRoot;
    while (nextPower >= startEven && evenRoot > 0){
        int l = (nextPower-startOdd+1)/2;
        evenBitmap |= (1ULL<<((nextPower-startEven)/2));
        if(l!=64){
            oddBitmap |= (primeBitmaps[0] << l);
        }
        evenRoot -= 2;
        nextPower = evenRoot*evenRoot*evenRoot;
    }

    //apply both relevant prime bitmaps added to each odd power to the evenBitmap (while there exist exceptions)
    bool ranOut = false;
    while (oddRoot > 0 && ~evenBitmap!=0){
        nextPower = oddRoot*oddRoot*oddRoot;
        ll dif = (startEven-nextPower-1)/(128);
        if(dif>=primeBitmapsLength){
            ranOut = true;
            break;
        }
        ll offset = ((nextPower+1+128*(dif+1)-startEven)/2);
        evenBitmap |= (primeBitmaps[dif] >> (128-offset));
        if(offset!=64){
            evenBitmap |= (primeBitmaps[dif+1] << (offset));
        }
        oddRoot -= 2;
    }

    //apply both relevant prime bitmaps added to each even power to the oddBitmap (while there exist exceptions)
    while (evenRoot > 0 && ~oddBitmap!=0){
        nextPower = evenRoot*evenRoot*evenRoot;
        ll dif = (startOdd-nextPower-1)/(128);
        if(dif>=primeBitmapsLength){
            ranOut = true;
            break;
        }
        ll offset = ((nextPower+1+128*(dif+1)-startOdd)/2);
        oddBitmap |= (primeBitmaps[dif] >> (64-offset));
        //cout <<sq << " " <<  bitset<64>(oddBitmap) << endl;
        if(offset!=64){
            oddBitmap |= (primeBitmaps[dif+1] << (offset));
            //cout << sq << " " << bitset<64>(oddBitmap) << endl;
        }
        evenRoot -= 2;
    }

    //count up all the possible exceptions
    vector<ll> exceptions;
    if(~evenBitmap!=0){
        for(int i=0; i<64; i++){
            if(!((evenBitmap>>i)&1ULL)){
                exceptions.push_back(startEven+2*i);
            }
        }
    }
    if(~oddBitmap!=0){
        for(int i=0; i<64; i++){
            if(!((oddBitmap>>i)&1ULL)){
                exceptions.push_back(startOdd+2*i);
            }
        }
    }

    //if we ran out of bitmaps, then we use deterministic Miller-Rabin to detect larger primes
    if(ranOut){
        vector<ll> realExceptions;
        for(ll e: exceptions){
            ll maxT = cubeRootFloor(e);
            bool isException = true;
            for(ll t=1; t<maxT;t++){
                if(isPrime(e-t*t*t)){
                    isException = false;
                    break;
                }
            }
            if(isException){
                realExceptions.push_back(e);
            }
        }
        return realExceptions;
    }
    return exceptions;
}

int main(int argc, char *argv[]) {
    /*
    arg1 (int) is the power of 10 to compute up to (ie 6 means compute up to 10^6)
    arg2 (int) tells upper bounds from prime bitmaps (ie 6 means use all primes up to 10^6) 
        the prime bitmaps must be generated before hand
    arg3 (str) gives the name of the desired output file. Default is "out.txt"
    */

    auto start = chrono::high_resolution_clock::now();
    ll N = pow(10, stoi(argv[1]));
    string bitMapLevel = argv[2];
    ll n = N/128+1; //number of calls to checkRange needed
    ll sep = n/100;  //used for the progress bar
    string destinationFile = "out.txt";
    if(argc == 4){
        destinationFile = argv[3];
    }

    load_primes("../resources/prime_bitmaps/bitmaps"+bitMapLevel+".txt");
    primeBitmapsLength = primeBitmaps.size();

    //to keep track of exceptions with parallelization, we write the exceptions to disc in a temp folder
    fs::create_directory("temp");


    #pragma omp parallel for schedule(dynamic)
    for(ll i=0; i<n; i++){
        if(i%sep == 0){
            cout << "\rProgress: "<< i/sep << "%    " << flush;
        }
        vector<ll> exceptions = checkRange(i);
        if(!exceptions.empty()){
            ofstream out("temp/out_" + std::to_string(i) + ".txt");
            for(ll e: exceptions){
                out << e << " ";
            }
        }
    }

    //after the parallelization, we collect all the exceptions from the temp folder
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
    cout << "\nNumber of exceptions: " << all_exceptions.size() << endl;
    sort(all_exceptions.begin(), all_exceptions.end());
    
    ofstream out;
    out.open(destinationFile);
    for(auto x:all_exceptions){
        out << x << "\n";
    }
    cout << "Exceptions printed to \"" << destinationFile << "\"" <<  endl;
    fs::remove_all("temp");

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << duration.count() << " seconds" << endl;

    return 0;
}