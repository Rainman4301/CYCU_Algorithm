#include<iostream>
#include<string>

using namespace std;



int main(){

    string a = "���s��ޤj��";
    string b = "��߲M�ؤj��";
    

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