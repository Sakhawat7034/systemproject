#include <bits/stdc++.h>
using namespace std;
int main()
{

    // Initialize set
    set<int> s;


    s.insert(1);
    s.insert(4);
    s.insert(2);
    s.insert(5);
    s.insert(3);
    s.insert(6);
    s.insert(8);

    // iterator pointing t
    // position where 2 is
   int it=s.find(2)-s.begin();

    cout<< (it-)<<endl;



}
