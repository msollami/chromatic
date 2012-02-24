#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
using namespace std;

#define RANDOM_SEED() srandom(time(NULL))

/**
 * The calc_displacement procedure calculates the displacements from a vector of counts.
 * \param _U Input: Vector of counts.
 * \param _W Output: Vector of displacements.
 */
template<class T> inline
void calc_displacement(const vector<T> &_U, vector<T> &_W)
{
	if(_U.size() == 0) return;
	const T *U = &_U[0], *V = &_U[0] + _U.size();
	T *W = &_W[0];
    
	T t = 0;
	while(U != V)
	{
		T s = *U++;
		*W++ = t;
		t += s;
	}
}

/**
 * The binary_read procedure reads binary data from a file into a memory block.
 * \param _First Input: Iterator pointing to the first element.
 * \param _Last Input: Iterator pointing past the last element.
 * \param _Filename Input: Name of the input file.
 * \param _Seek Input: Offset into the data file
 */
template<class _InIt> inline
void binary_read(_InIt _First, _InIt _Last, const string _Filename, const int _Seek, const bool _Seek_Char = false)
{
    ifstream is(_Filename.c_str(), ios::binary);
    if(! is.good()){
        cerr << "ERROR: Cannot read file " << _Filename << endl;
        return;
    }
    
    if(_Seek_Char)
    {
        is.seekg(_Seek);
    }
    else
    {
        is.seekg(_Seek * sizeof(*_First));
    }
    is.read((char*)&*_First, streamsize(_Last - _First) * sizeof(*_First));
    is.close();
}

/**
 * The binary_write procedure writes binary data to a file from a memory block.
 * \param _First Input: Iterator pointing to the first element.
 * \param _Last Input: Iterator pointing past the last element.
 * \param _Filename Input: Name of the output file.
 */
template<class _InIt> inline
void binary_write(_InIt _First, _InIt _Last, const string _Filename)
{
    ofstream os(_Filename.c_str(), ios::binary);
    if(! os.good()){
        cerr << "ERROR: Cannot write to file " << _Filename << endl;
        return;
    }
    os.write((char*)&*_First, streamsize(_Last - _First) * sizeof(*_First));
    os.close();
}


/* Testing commit */

/*
 A...Adjacency matrix
 N[n]...Number of neighbours in i-th row
 D[n]...Displacement of neighbours in i-th row
 P[n]...Permutation of vertices [0,...,n-1]
 C[n]...Vertex colors
 T[n]...Temporary array for fast complement calculation
 n...Number of vertices
*/

bool chromatic(int *A, int *N, int *D, int *P, int *C, int *T, int n)
{
    bool changed = false;
    
    for(int i = 0; i < n; i++)
    {
        int c = 0, p = P[i], b = D[p], s = N[p], e = b + s;
       
        for(int j = b; j < e; j++) T[C[A[j]]] = 1;
        
        while(T[c] == 1) c++;
        
        for(int j = b; j < e; j++) T[C[A[j]]] = 0;
        
        if(C[p] != c)
        {
            C[p] = c;
            changed = true;
        }
    }
    
    return changed;
}

void shuffle(int *P, int n, int k)
{

     srand ( time(NULL) );
    
    //swap pairs of P entries n times...
    for(int i = 0; i < k; i++){
    
        long int j, k, temp;
        do {
            j = rand() % n;
            k = rand() % n;   
            temp = P[j];
            P[j] = P[k];
            P[k] = temp;
        } while(P[j] == P[k]);
    }
}

void combine(int *M, int *F, int *C, int n)
{
 srand(time(NULL));
    for(int i = 0; i < n; i++)
    {
        int j = rand() % 2;
        if(j == 0) C[i] = M[i];
        else C[i] = F[i];
    }
}

int main (int argc, char * const argv[]) {

    cout << "STEP 1: READING GRAPH" <<endl;    

    //Read file-----------------------------------------------------------------------
    //    if (argc != 4)
    //    {fprintf(stderr,"Usage: ./clique file samples clique\n");
    //        exit(1);}
    //	int sample = atoi(argv[2]);
    //	int max_clique = atoi(argv[3]);
    //	ifstream is(argv[1]);
    //	if(!is.good()) {
    //		cout << "Error: file could not be opened" << endl;
    //		exit(1);
    //	}
    
    string root("./");
    vector<int> H(2, 0);
    binary_read(H.begin(), H.end(), root + "Header.bin", 0);

    int n = H[0];//vertices
    int m = H[1];//edges*2
    
    cout << "Done. Graph has " << n << " vertices and " << m << " edges.\n" << endl;   
            
    vector<int> A(m, 0);
    binary_read(A.begin(), A.end(), root + "Column.bin", 0);

    vector<int> N(n, 0);
    binary_read(N.begin(), N.end(), root + "Count.bin", 0);
    vector<int> D(n, 0);
    calc_displacement(N, D);

    vector<int> P(n, 0);
    vector<int> C(n, 0);
    vector<int> B(n, 0);
    vector<int> T(n, 0);
    vector<int> M(n, 0);
    vector<int> F(n, 0);
    int cm = n, cf = n;
    for(int i = 0; i < n; i++)
    {
        P[i] = i; C[i] = i; 
        M[i] = i; F[i] = i; 
    }
    
    RANDOM_SEED();
    
    //STEP 3: ITERATE CHROMATIC ITER TIMES
    cout << "STEP 2: ITERATING" << endl;       
    int min = n;    
    int iteration = 500;
    int local_shuffle = 2;
    int local_loop = 100;
    
    for(int i = 0; i < iteration; i++)
    {
        //combine(M.data(), F.data(), C.data(), n);
        for(int k = 0; k < n; k++) C[i] = B[i];
        
        for(int l = 0; l < local_loop; l++)
        {
            while(chromatic(A.data(), N.data(), D.data(), P.data(), C.data(), T.data(), n));
            
            int max = 0;
            for(int j = 0; j < n; j++) if(max < C[j]) max = C[j];
            max = max + 1;
            
            bool improved = false;
            if(min > max) min = max, improved = true;

            if(improved)
            {
                for(int k = 0; k < n; k++) B[i] = C[i];
            }
            shuffle(P.data(), n, n);
            shuffle(C.data(), n, local_shuffle);
        }
        /*
        if(min <= cm)
        {
            cm = min;
            for(int k = 0; k < n; k++) M[i] = B[i];
        }
        else if(min <= cf)
        {
            cf = min;
            for(int k = 0; k < n; k++) F[i] = B[i];            
        }
        */    
        if(n % 1000 == 0) cout << "current bound: " << min << " colors" << endl;
    }
    
    cout << endl << "Chromatic upper bound: " << min << endl << endl;
 
    return 0;
}
