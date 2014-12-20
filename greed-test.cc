//
//  Erdos-Faber-Lovasz conjecture testing
//  This is version 0.5 of the software
//  The goal is to see if the greedy algorithm always solves the conjecture
//  Warning: The EFL conjecture is known to be a disease
//

#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

const unsigned int NumProfs=32; //  Each professor is numbered from 0 to NumProfs-1

const unsigned int CmteSize=6;

unsigned int zz=0;  // This is a counter used in the GetRandom function

unsigned int cmte_array [CmteSize] [CmteSize]; // The committee array
// Has to satisfy the at-most-one intersection property (and the set property)
// Each row is a committee!

bool cmte_inter [CmteSize] [CmteSize] ;  // data structure used to
// track current committee intersections while we build cmte_array
// initialize to all false
// cmte_inter[a][b] = true means cmte a has nonempty intersection with cmte b
// only used for a > b
// will be re_used to track chair assignments once the committees are built

bool temp_inter [CmteSize] ;  // data structure used to record the
// intersections that a new element would create if added to a cmte
// gets ORed into cmte_inter if the element in question is indeed added to a cmte

unsigned int cmte_cnt [NumProfs] ;  // counts number of committees each prof is on

bool pieces [NumProfs] [CmteSize] ;  //  For each prof, this is his list of committee
//  assignments ;  These are the pieces when you play the game.

unsigned int NumPieces = 0 ; // number of nonzero pieces in the puzzle game
// that results from a committee selection


unsigned int MaxCmteCnt = 0;  // given a committee selection, this
//  is the max number of committees for any one prof -- this is max
//  value from cmte_cnt

unsigned int sorted_profs [NumProfs] ;  // sorts the profs starting with those with
// largest number of committee assignments
// used in implementing one of the chair assignment algorithms 
//  (eg assign chairs starting with the prof with max committees)

bool hit ;  // used for detecting overlapping squares in the puzzle

bool successful = true ; // used to track status of tests

//Function GetRandom
//Generate a random number between 0 and NumProfs (inclusive)
//Revised to use some randomness but to focus on smaller prof numbers
//so as to build test data with more overlap between the committees
unsigned int GetRandom()
{
  if (zz <= CmteSize) {return (zz++) ; }
  if (zz == NumProfs) {zz = 2 ; }
  return (rand() % NumProfs % zz++) ;
    }


//  Duplicated is used to test new profs as we build committees
//  Prof is called duplicated if she is already on the committee under construction
//     or he would be 2nd intersection with an existing committee
//  k1 is a prof, k2 and k3 are the indices in cmte_array she is proposed for
bool duplicated (int k1, int k2, int k3)  {
  unsigned int a, b, c;
  for ( b = 0 ; b < k3 ; b++ ) {
    if (k1 == cmte_array[k2][b]) { return (true); } }
  // next check for intersections with already defined cmtes
  for ( c = 0 ; c < k2 ; c++ ) { temp_inter[c] = false; }  // init temp_inter
  for ( a = 0 ; a < k2 ; a++ ) {
    for  ( b = 0 ; b < CmteSize ; b++ ) {
      if ((k1 == cmte_array[a][b]) & cmte_inter[k2][a]) { return (true); }
      if (k1 == cmte_array[a][b]) {temp_inter[a] = true;}
    }}
  //  proposed element k3 is not dup so update cmte_inter
  for (c = 0 ; c < k2 ; c++ ) {cmte_inter[k2][c] = (cmte_inter[k2][c] || temp_inter[c] ); }
  return (false);
}

//  Procedure test builds one random committee array that meets the two constraints
//     first,  that each row is a set (no dups in any row) and
//     second, that the intersection constraint of the conjecture
//  THEN it tries to build a chair assignment using the greedy algorithm

int test()
{
  int n, m, oo, temp ;
 
  // init 
  for ( n=0 ; n < NumProfs ; n++ ) {cmte_cnt[n] = 0; }

  // init cmte_inter to all false
  for ( n=0 ; n < CmteSize ; n++) {
    for ( m=0 ; m < CmteSize ; m++ ) {
        cmte_inter[n][m] = false;
    }}

  // init pieces to all false
  for ( n=0 ; n < NumProfs ; n++) {
    for ( m=0 ; m < CmteSize ; m++ ) {
        pieces[n][m] = false;
    }}

  // now we build the commitee array
  for ( n=0 ; n < CmteSize ; n++) {
    for ( m=0 ; m < CmteSize ; m++ ) {
	temp = GetRandom();
        while (duplicated(temp,n,m)) { temp = GetRandom(); }
        cmte_array[n][m] = temp;
        pieces[temp][n] = true ;
        cmte_cnt[temp]++ ;
        if ( cmte_cnt[temp] == (MaxCmteCnt + 1)) {MaxCmteCnt++ ; }
}}

  // cout << "\n" << "Print the list of committees"  << "\n";

  // lets print out the commitee set to see if it looks right

  // for ( n=0 ; n<CmteSize ; n++) {
  //  cout << "\n";
  //  for ( m=0 ; m<CmteSize ; m++ ) {
  //    cout << cmte_array[n][m] << " " ;
  //  }}

  // cout << "\n"  << "\n" << "Next print committee count for each prof"  << "\n";

  // for ( n=0 ; n < NumProfs ; n++) {
  //   cout << n << ":" << cmte_cnt[n] << " ";
  //  if (((n+1) % 10) == 0) { cout << "\n" ; } 
  //  }

  //cout << "\n"  << "\n" << "Next print the list of pieces"  << "\n";

  // for ( n=0 ; n < NumProfs ; n++) {
  // if ( cmte_cnt[n] > 0 )
  //   { cout << n << ": " ;
  //     for (m=0 ; m<CmteSize ; m++) { cout << pieces[n][m]; }
  //     cout << "\n" ;
  //   }
  //  }

  // cout << "\n"  << "Lets start the solution"  << "\n";

 // we no longer need cmte_inter 
 // so we reuse it to build the chair assignments

 // init cmte_inter to all false
  for ( n=0 ; n < CmteSize ; n++) {
    for ( m=0 ; m < CmteSize ; m++ ) {
        cmte_inter[n][m] = false;
    }}

  // first count the number of nonzero pieces

  for ( n=0 ; n < NumProfs ; n++ ) {
    if ( cmte_cnt[n] > 0 )  { NumPieces++ ; }}
    cout << "The number of nonzero pieces in the puzzle is: " ;
    cout << NumPieces << "\n" ;

    cout << "The max number of committees for any one prof is: " ;
    cout << MaxCmteCnt << "\n" ;

  // now build the sorted list of profs

    oo = 0;
    for ( m=MaxCmteCnt ; m != 0 ; m=(m-1) ) {
    for ( n=0 ; n < NumProfs ; n++) {
      if (cmte_cnt[n] == m) {sorted_profs[oo] = n ; oo++ ; }
    }}

    cout << "The first two profs with highest cmte counts are: " ;
    cout << sorted_profs[0] << " and " << sorted_profs[1] << "\n" ;
    cout << "The last prof by order is: " << sorted_profs[NumPieces-1] << "\n" ;

    // now implement one solution algorithm
    // the first chair assignment is easy
    for (oo=0 ; oo < CmteSize; oo++) { cmte_inter[0][oo] = pieces[sorted_profs[0]][oo] ; }
    cout << "Prof " << sorted_profs[0] << " is in chair " <<  0 << "\n" ; 

    //now compute the rest of the chair assignments
    //process profs in the order defined by sorted_profs
    //give each prof the next available chair
    //if no chair is available then fail
    //this is a greedy algorithm!
    n=1; 
    while  (n < NumPieces)
      { m=0;
      while (m < CmteSize)
        { hit = false;
        for (oo=0 ; oo < CmteSize ; oo++ ) {
          hit = (hit || (cmte_inter[m][oo] && pieces[sorted_profs[n]][oo])) ; }
        // if hit is false then we can make a chair assignment
	if (!hit) {
            for (oo=0 ; oo < CmteSize ; oo++ )
	    {  cmte_inter[m][oo] = (cmte_inter[m][oo] || pieces[sorted_profs[n]][oo]) ; }
            cout << "Prof " << sorted_profs[n] << " is in chair " <<  m << "\n" ; 
	    m = CmteSize + 1; }
        // if hit is true we have to keep looking
        if (hit) { m++ ; } }
      // if we exited exactly at CmteSize we failed
      if (m==CmteSize)   {n = NumPieces ;  cout << "FAILED" << "\n" ; successful = false; }
      // if we exited exactly at CmteSize+1, we succeeded and go on to the next piece
      if (m==CmteSize+1) {n++ ; }
      }
 cout << "\n"  << "That's the end of one test!"  << "\n";
}

//  MAIN will run a bunch of tests

main() {
  unsigned int k = 0;
  unsigned int n , m ;
  cout << "\n"  << "Welcome to EFL testing!"  << "\n";
  srand(536339);
  while (successful)  {
    zz = 0;
    NumPieces = 0;
    MaxCmteCnt = 0 ;
    k++ ;
    cout << "This is test number " << k << "\n";
    test() ;
  }
  if  (successful) {cout << "All tests succeeded!" << "\n" ; }
  if (!successful) {cout << "We had a failure!"    << "\n" ; 


  cout << "\n" << "Print the list of committees"  << "\n";

  // lets print out the committee set to see if it looks right

  for ( n=0 ; n<CmteSize ; n++) {
   cout << "\n";
   for ( m=0 ; m<CmteSize ; m++ ) {
     cout << cmte_array[n][m] << " " ;
    }}

  cout << "\n"  << "\n" << "Next print committee count for each prof"  << "\n";

  for ( n=0 ; n < NumProfs ; n++) {
  cout << n << ":" << cmte_cnt[n] << " ";
   if (((n+1) % 10) == 0) { cout << "\n" ; } 
   }

  cout << "\n"  << "\n" << "Next print the list of pieces"  << "\n";

   for ( n=0 ; n < NumProfs ; n++) {
   if ( cmte_cnt[n] > 0 )
     { cout << n << ": " ;
       for (m=0 ; m<CmteSize ; m++) { cout << pieces[n][m]; }
       cout << "\n" ;
     }
    }

  cout << "\n"  << "That's the counterexample"  << "\n";

  }

}

