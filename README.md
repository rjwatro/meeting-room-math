meeting-room-math
=================

Anthropomorphic version of the Erdos-Faber-Lovasz conjecture,
as stated by Haddad and Tardif in 2004:
"Suppose that, in a university department, there are k committees,
each consisting of k faculty members, and that all committees
have their meetings in the same room, which has k chairs. Suppose
also that at most one person belongs to the intersection of any
two committees.  Is it possible to assign the committee members
to chairs in such a way that each member sits in the same chair
for all the different committees to which he or she belongs?"

This repo contains C++ code that generates test cases for the
conjecture.  More specifically, we implemented a simple greedy
algorithm for assigning professors to chairs and then exhaustively
tested it, hoping that the counterexamples generated might provide
some insight into creating a correct algorithm.
