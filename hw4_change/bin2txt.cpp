


#include <iostream>

#include <fstream>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>



typedef struct adjacent{
    char putID[10];
    char getID[10];
    float weight;
}adjacent;

using namespace std;

int main(){

    ifstream binfile ;
   

    
    string filename;
    adjacent contain;
    int stno = 0;

    cout << "imput a bin_file"<<endl;
    cin >> filename;

    binfile.open(("pairs"+filename+".bin"),fstream::in | fstream::binary);
    

    if(binfile.is_open()){
        
        binfile.seekg(0, binfile.end);
        stno = binfile.tellg()/sizeof(contain);
        binfile.seekg(0,binfile.beg);


        cout << stno << endl;

        for (int i = 0 ; i < stno ; i++)
        {
            binfile.read((char*)&contain, sizeof(contain));
            cout << contain.putID<<endl;
            cout << contain.getID<<endl;
            cout << contain.weight<<endl;

            cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
        }
    }


    binfile.close();
    

    return 0;
}
