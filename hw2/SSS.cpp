#include<iostream>
#include<string>

using namespace std;



int main(){

    string a = "明新科技大學";
    string b = "國立清華大學";
    

    if(a>b){
        cout << "A > B"<< endl;
    }
    else if(a == b){
        cout << "A = B"<< endl;
    }
    else if(a < b){
        cout << "A < B"<< endl;
    }

    

    return 0;
}