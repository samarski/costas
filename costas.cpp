#include <cstring>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector> 
#include <utility>

int permutation[32];
int counter;

typedef unsigned int inj_map;
inj_map inj[32];
inj_map changes[32][32];

int difference[32][32];
bool print_permutations = false;

void print(unsigned int upto) {
    std::cout << "(";
    for (unsigned int k = 0; k <= upto; k++) {
        std::cout << std::setw(2) << permutation[k];
        std::cout << ((k == upto) ? ')' : ',');
    }
    std::cout << std::endl;
}

bool injective_so_far(int level, const unsigned int upto) {

    auto to = level;
    // if (to > upto/2)
    //    to = upto/2;

    for (int k = 1; k <= to; k++) {
        int poz = difference[permutation[level]][permutation[level - k]];

        if (inj[k] & (1 << (unsigned int)poz))
            return false;
    }

    inj_map* __restrict chg = changes[level];
    inj_map* __restrict src = inj;
    for (unsigned int j = 1; j <= level; j++)
        chg[j] = src[j];


    for (int k = 1; k <= to; k++) {
        int poz = difference[permutation[level]][permutation[level-k]];
        inj[k] |= (1 << (unsigned int)poz);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    }

    if (level == upto) {
        // we have to check all other cases
        for (unsigned int k = 1; k <= level; k++) {
            unsigned int offset = (upto+1) - k; 
            inj_map mask = 0;
            
            for (unsigned int i = 0; i < k; i++) {
                int poz = difference[permutation[i]][permutation[i+offset]];
                mask |= (inj_map)1 << (unsigned int)poz;
            }

            if (inj[k] & mask) {
                inj_map* __restrict dst = inj;
                for (unsigned int j = 1; j <= level; j++)
                    dst[j] = chg[j];

                return false;
            }

            inj[k] |= mask;
        }
    }

    return true;
}

void find_next(unsigned int level, unsigned int upto) {

    unsigned int k = level;
    do {
        std::swap(permutation[level], permutation[k]);

        if (injective_so_far(level, upto)) {

            if (level == upto) {
                counter++;
                if (print_permutations)
                    print(upto);
            }
            else {
                find_next(level+1, upto);
            }

            inj_map* __restrict izm = changes[level];
            for (unsigned int j = 1; j <= level; j++)
                inj[j] = izm[j];
        }

        std::swap(permutation[level], permutation[k]);

    } while (++k <= upto);
}

using FieldDescription = struct {
    int p;
    int n;
    std::vector<int> partial;
};

void prepare_difference_table(FieldDescription& fd) {

    int p = 1;
    for (int i = 1; i <= fd.n; i++)
        p *= fd.p;

    for (int i = 0; i < p; i++) {
       for (int j = 0; j < p; j++) {
            int result = 0;
            int tp = p;
            for (auto k = 1; k <= fd.n; k++) {
                tp = tp / fd.p;
                auto ti = (i / tp) % fd.p;
                auto tj = (j / tp) % fd.p;
                auto tr = ti - tj;
                if (tr < 0)
                    tr += fd.p;

                result = result * fd.p + tr;
            }

            difference[i][j] = result;
        }
    }
}


void find(FieldDescription& fd) {

    prepare_difference_table(fd);

    int p = 1;
    for (int i = 1; i <= fd.n; i++)
        p *= fd.p;

    for (int i = 0; i <= p-1; i++) {
        permutation[i] = i+1;
    }

    for (int k = 1; k <= p-2; k++) {
        // for all k we have \alpha^k (0) - 0 = 0
        inj[k] = 1;
    }

    // partial[0] should always be 1
    for (int level = 1; level < fd.partial.size(); level++) {
        std::swap(permutation[level], permutation[fd.partial[level]-1]);
    }

    for (int level = 1; level < fd.partial.size(); level++) {
        for (int k = 1; k <= level; k++) { 
            int res = difference[permutation[level]][permutation[level-k]];
            inj[k] |= (1 << (unsigned int)res);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
        }
    }

    counter = 0;

    find_next(fd.partial.empty() ? 1 : fd.partial.size(), p-2);

    std::cout << "Total count: " << counter << std::endl;
}


int main() {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    auto listOfFields = std::vector<std::pair<int,int>>{
        {3,1},
        {2,2},
        {5,1},
        {7,1},
        {2,3},
        {3,2},
        {11,1},
        {13,1},
        {2,4},
        {17,1},
        {19,1},
        {23,1},
        {5,2},
        {3,3},
        {29,1}
    };

    for (auto& f: listOfFields) {
        std::cout << "p=" << f.first << ", n=" << f.second << ": ";
        if (print_permutations)
            std::cout << std::endl;
        auto fd = FieldDescription{f.first, f.second, std::vector<int>{}};
        find(fd);
    }
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Execution time: "
              << duration.count()
              << " seconds\n";

    return 0;
}

