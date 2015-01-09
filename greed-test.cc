//
//  Erdos-Faber-Lovasz conjecture testing
//  This is version 0.5 of the software
//  The goal is to see if the greedy algorithm always solves the conjecture
//  Warning: The EFL conjecture is known to be a disease
//

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#define DEFAULT_NUM_PROFS 32
#define DEFAULT_CMTESIZE 6
using namespace std;
//const unsigned int NumProfs=32; //  Each professor is numbered from 0 to NumProfs-1

//const unsigned int CmteSize=6;

unsigned int zz=0;  // This is a counter used in the GetRandom function

// unsigned int cmte_array [CmteSize] [CmteSize]; // The committee array
// Has to satisfy the at-most-one intersection property (and the set property)
// Each row is a committee!

// bool cmte_inter [CmteSize] [CmteSize] ;  // data structure used to
// track current committee intersections while we build cmte_array
// initialize to all false
// cmte_inter[a][b] = true means cmte a has nonempty intersection with cmte b
// only used for a > b
// will be re_used to track chair assignments once the committees are built

// bool temp_inter [CmteSize] ;  // data structure used to record the
// intersections that a new element would create if added to a cmte
// gets ORed into cmte_inter if the element in question is indeed added to a cmte

//unsigned int cmte_cnt [NumProfs] ;  // counts number of committees each prof is on

//bool pieces [NumProfs] [CmteSize] ;  //  For each prof, this is his list of committee
//  assignments ;  These are the pieces when you play the game.

// unsigned int NumPieces = 0 ; // number of nonzero pieces in the puzzle game
// that results from a committee selection


// unsigned int MaxCmteCnt = 0;  // given a committee selection, this
//  is the max number of committees for any one prof -- this is max
//  value from cmte_cnt


// largest number of committee assignments
// used in implementing one of the chair assignment algorithms
//  (eg assign chairs starting with the prof with max committees)

bool hit ;  // used for detecting overlapping squares in the puzzle

// ofstream file1;
// ofstream file2;

int GetRandom(int NumProfs, int CmteSize);
int randomInt(int max);
bool duplicated (int k1, int k2, int k3, int ** cmte_array, bool ** cmte_inter, int CmteSize); // cmte_array, cmte_inter
// DO NOT NEED FOR TEST: cmte_cnt cmte_inter pieces cmte_array
bool test(int NumProfs, int CmteSize, int MaxCmteCnt, int NumPieces); // NumProfs, CmteSize
//  MAIN will run a bunch of tests

int main(int argc, char ** argv) {
  bool def = false;
  int NumProfs, CmteSize; //=32;
  //int =6;
  if(argc != 3) {
    cout << "Incorrect command line args, using default values" << endl;
    cout << "Usage: " << argv[0] << " <Number of Profs> <cmte size>" << endl;
    def = true;
  }
  if(def) {
    NumProfs = DEFAULT_NUM_PROFS;
    CmteSize = DEFAULT_CMTESIZE;
  }
  else {
    NumProfs = atoi(argv[1]);
    CmteSize = atoi(argv[2]);
  }
  bool successful = true ; // used to track status of tests
  
  int k = 0;
  int n , m ;
  cout << "\n"  << "Welcome to EFL testing!"  << "\n";
  srand(536339);
  // file1.open("out1");
  // file2.open("out2");
  while (successful)  {
    zz = 0;
    cout << "This is test number " << k << "\n";
    
    successful = test(NumProfs,CmteSize,0,0) ;
    k++;
  }
  // file1.close();
  // file2.close();
}

//Function GetRandom
//Generate a random number between 0 and NumProfs (inclusive)
//Revised to use some randomness but to focus on smaller prof numbers
//so as to build test data with more overlap between the committees
int GetRandom(int NumProfs, int CmteSize) {
  if (zz <= CmteSize) {return (zz++) ; }
  if (zz == NumProfs) {zz = 2 ; }
  return (rand() % NumProfs % zz++) ;
}

int randomInt(int max) {
  return rand() % max;
}

//  Duplicated is used to test new profs as we build committees
//  Prof is called duplicated if she is already on the committee under construction
//     or he would be 2nd intersection with an existing committee
//  k1 is a prof, k2 and k3 are the indices in cmte_array she is proposed for
bool duplicated (int k1, int k2, int k3, int ** cmte_array, bool ** cmte_inter, int CmteSize)  {
  unsigned int a, b, c;
  bool * temp_inter = new bool[k2];
  for ( b = 0 ; b < k3 ; b++ ) {
    if (k1 == cmte_array[k2][b]) {
      return (true);
    }
  }
  // next check for intersections with already defined cmtes
  for ( c = 0 ; c < k2 ; c++ ) {
    temp_inter[c] = false;
  }  // init temp_inter
  for ( a = 0 ; a < k2 ; a++ ) {
    for  ( b = 0 ; b < CmteSize ; b++ ) {
      if ((k1 == cmte_array[a][b]) & cmte_inter[k2][a]) {
	       return (true);
      }
      if (k1 == cmte_array[a][b]) {
	       temp_inter[a] = true;
      }
    }
  }
  //  proposed element k3 is not dup so update cmte_inter
  for (c = 0 ; c < k2 ; c++ ) {
    cmte_inter[k2][c] = (cmte_inter[k2][c] || temp_inter[c] );
  }
  return (false);
}

//  Procedure test builds one random committee array that meets the two constraints
//     first,  that each row is a set (no dups in any row) and
//     second, that the intersection constraint of the conjecture
//  THEN it tries to build a chair assignment using the greedy algorithm

bool test(int NumProfs, int CmteSize, int MaxCmteCnt, int NumPieces) {
  int * cmte_cnt = new int[NumProfs];
  bool ** cmte_inter = new bool * [CmteSize];
  int ** pieces = new int * [NumProfs];
  int ** cmte_array = new int * [CmteSize];
  int * sorted_profs = new int[NumProfs];  // sorts the profs starting with those with
  // init
  for (int i = 0 ; i < NumProfs ; i++ ) {
    pieces[i] = new int[CmteSize];
    cmte_cnt[i] = 0;
  }
  for(int i = 0; i < CmteSize; i++) {
    cmte_inter[i] = new bool[CmteSize];
    cmte_array[i] = new int[CmteSize];
  }
  // init cmte_inter to all false
  for (int i = 0; i < CmteSize; i++) {
    for (int j = 0; j < CmteSize; j++ ) {
      cmte_inter[i][j] = false;
    }
  }
  // init pieces to all false
  for (int i = 0; i < NumProfs; i++) {
    for (int j = 0; j < CmteSize; j++ ) {
      pieces[i][j] = false;
    }
  }
  // now we build the commitee array
  for (int i = 0; i < CmteSize; i++) {
    for (int j = 0; j < CmteSize; j++ ) {
      // temp = GetRandom(NumProfs,CmteSize);
      // file1 << GetRandom(NumProfs,CmteSize) << endl;
      // file2 << randomInt(NumProfs) << endl;
      int temp = randomInt(NumProfs); // more uniformly distributed
      while (duplicated(temp,i,j,cmte_array,cmte_inter,CmteSize)) { // get random prof and check if duplicated (makes array)
         // make check for bail count 
         // make array all 0s of size numProfs if you have picked all profs bail
         // add array[professors] = 1
	       temp = GetRandom(NumProfs,CmteSize);
      }
      cmte_array[i][j] = temp;
      pieces[temp][i] = true;
      cmte_cnt[temp]++ ;
      if ( cmte_cnt[temp] == (MaxCmteCnt + 1)) {
	       MaxCmteCnt++;
      }
    }
  }
  
  // we no longer need cmte_inter
  // so we reuse it to build the chair assignments
  
  // init cmte_inter to all false
  for (int i = 0; i < CmteSize; i++) {
    for (int j = 0; j < CmteSize; j++ ) {
      cmte_inter[i][j] = false;
    }
  }
  
  // first count the number of nonzero pieces
  
  for (int i = 0; i < NumProfs; i++ ) {
    if ( cmte_cnt[i] > 0 )  {
      NumPieces++ ;
    }
  }
  cout << "The number of nonzero pieces in the puzzle is: " ;
  cout << NumPieces << "\n" ;
  
  cout << "The max number of committees for any one prof is: " ;
  cout << MaxCmteCnt << "\n" ;
  
  // now build the sorted list of profs
  int counter = 0;
  for (int i = MaxCmteCnt ; i > 0 ; i--) {
    for (int j = 0; j < NumProfs; j++) {
      if (cmte_cnt[j] == i) {
	       sorted_profs[counter] = j; 
         counter++;
      }
    }
  }
  
  cout << "The first two profs with highest cmte counts are: " ;
  cout << sorted_profs[0] << " and " << sorted_profs[1] << "\n" ;
  cout << "The last prof by order is: " << sorted_profs[NumPieces-1] << "\n" ;
  
  // now implement one solution algorithm
  // the first chair assignment is easy
  int current_prof = 0;
  for (int i = 0; i < CmteSize; i++) {
    cmte_inter[current_prof][i] = pieces[sorted_profs[current_prof]][i] ;
  }
  cout << "Prof " << sorted_profs[current_prof] << " is in chair " << 0 << "\n" ;
  
  //now compute the rest of the chair assignments
  //process profs in the order defined by sorted_profs
  //give each prof the next available chair
  //if no chair is available then fail
  //this is a greedy algorithm!
  current_prof++;
  int current_chair = 0;
  while(current_prof < NumPieces) { 
    current_chair = 0;
    while (current_chair < CmteSize) { 
        hit = false; // cant put chair because 0f an overlap
        for (int i = 0; i < CmteSize; i++ ) {
          hit = (hit || (cmte_inter[current_chair][i] && pieces[sorted_profs[current_prof]][i])) ;
        }
        // if hit is false then we can make a chair assignment
        if (!hit) {
      	for (int i = 0; i < CmteSize; i++ ) {
      	  cmte_inter[current_chair][i] = (cmte_inter[current_chair][i] || pieces[sorted_profs[current_prof]][i]) ;
      	}
      	cout << "Prof " << sorted_profs[current_prof] << " is in chair " <<  current_chair << endl;
      	current_chair = CmteSize + 1;
      }
      // if hit is true we have to keep looking
      if (hit) {
        current_chair++;
      }
    }
    // if we exited exactly at CmteSize we failed
    if (current_chair == CmteSize) {
      //n = NumPieces ;
      cout << "FAILED" << endl;
      //successful = false;
      cout << "We had a failure!"    << endl;
      cout << endl << "Print the list of committees"  << endl;
      // lets print out the committee set to see if it looks right
      for (int i = 0; i < CmteSize; i++) {
        cout << "\n";
        for (int j = 0; j < CmteSize; j++ ) {
          cout << cmte_array[i][j] << " " ;
        }
      }
      cout << "\n"  << "\n" << "Next print committee count for each prof"  << endl;
      for (int i = 0; i < NumProfs; i++) {
        cout << i << ":" << cmte_cnt[i] << " ";
        if (((i+1) % 10) == 0) { 
          cout << endl ; 
        }
      }
      cout << endl  << endl << "Next print the list of pieces"  << endl;
      for (int i = 0; i < NumProfs ; i++) {
        if ( cmte_cnt[i] > 0 ) { 
          cout << i << ": " ;
          for (int j = 0; j < CmteSize; j++) { 
            cout << pieces[i][j]; 
          }
          cout << endl;
        }
      }
      cout << endl  << "That's the counterexample"  << endl;
      return false;
    }
    // if we exited exactly at CmteSize+1, we succeeded and go on to the next piece
    if (current_chair==CmteSize+1) {
      current_prof++ ;
    }
  }
  cout << endl  << "That's the end of one test!"  << endl;
  return true;
}
