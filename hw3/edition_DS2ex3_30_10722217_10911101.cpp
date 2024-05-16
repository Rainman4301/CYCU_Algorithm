/*
    Group: 30

       ID       |   Name
    ============+============
      10722217  |   х╗?хо????
    ------------+------------
      10911101  |   ??нч?????

*/

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


#define COLUMNS 6
#define MAX_LEN 10
#define BIG_INT 255


using namespace std;


typedef struct studenttype {

    char sid[MAX_LEN];
    char sname[MAX_LEN];
    unsigned char  score[COLUMNS];
    float mean;


}studenttype;










enum class hash_node_status{
    Unused,
    Deleted,
    Occupied
};






typedef struct{
    studenttype data;
    int key;
    hash_node_status status;
} hash_node;








string formatString(const char* format, ...) {
    char buffer[300];
    va_list vl;
    va_start(vl, format);
    vsnprintf(buffer, sizeof(buffer), format, vl);
    va_end(vl);
    return string(buffer);
}











// convert value to the special format.
string toString(float value, int totalWidth) {
    int aPart = (int)value;
    int bPart = (int)(round(value * 100)) % 100;
    int precision = 2;
    while (precision > 0 && (bPart % 10) == 0) {
        bPart /= 10;
        precision--;
    }
    if (bPart == 0) {
        return formatString("%*d", totalWidth, aPart);
    }
    else {
        return formatString("%*d.%*d", totalWidth - precision - 1,aPart, precision, bPart);
    }
}












void Text2Binary(string &fileName){
    fstream inFile, outFile;
    int stNo = 0;

    inFile.open(("input"+fileName+".txt").c_str(),fstream::in);

    if(inFile.is_open()){

        // fileName = "input" + fileName + ".bin";
        outFile.open(("input" + fileName + ".bin").c_str() , fstream::out|fstream::binary);           // create a binary file
        if(outFile.is_open()){

            cout << "#######  input" + fileName + ".bin  does not exit! #######" << endl;

            char rBuffer[BIG_INT]; //buffer to keep one whole record

            while(inFile.getline(rBuffer,BIG_INT,'\n')){
                string temp;
                studenttype oneSt;
                int cNo = 0, pre = 0, pos = 0;

                stNo++;                 //number of records
                temp.assign(rBuffer);   //make a duplicate of the entire record
                pos = temp.find_first_of('\t', pre);

                while(pos != string::npos){

                    switch (++cNo){
                        case 1:
                            strcpy(oneSt.sid, temp.substr(pre, pos - pre).c_str());
                            //copy a student ID
                            break;
                        case 2:
                            strcpy(oneSt.sname, temp.substr(pre, pos - pre).c_str());
                            //copy a student name
                            break;
                        default:
                            oneSt.score[cNo - 3] = atoi(temp.substr(pre, pos - pre).c_str());
                            // copy each score
                            break;
                    }

                    pre = ++pos;
                    pos = temp.find_first_of('\t', pre);
                }

                pos = temp.find_last_of('\t'); //position of the rightmost tab
                oneSt.mean = atof(temp.substr(pos + 1).c_str());

                outFile.write((char*)&oneSt, sizeof(oneSt)); //write the entire record into a binary file
            }
            outFile.close(); //close
        }
        inFile.close();
    }

}












int readBinary(string fileName , vector<studenttype> &container){

    fstream binFile;
    studenttype oneSt;
    int stNo = 0;





    binFile.open(("input" + fileName + ".bin").c_str(), fstream::in | fstream::binary);
    if(binFile.is_open()) {



        binFile.seekg(0, binFile.end);
        stNo = binFile.tellg()/sizeof(oneSt);
        binFile.seekg(0,binFile.beg);

        for (int i = 0 ; i < stNo ; i++)
        {
            binFile.read((char*)&oneSt, sizeof(oneSt)); // read the entire record from a binary file

            // cout << "[" << i + 1 << ']' << oneSt.sid << "," << oneSt.sname  << endl;

            container.push_back(oneSt);
        }
    }
    binFile.close();
    return stNo;
}











int find_prime(int least){

    for (int i = ceil( least * 1.2) ; ; i++)
    {
        for (int j = 2 ; j <= i ; j++)
        {
            if(j == i && i != least*1.2 ){
                return i;
            }
            if(i % j == 0){
                break;
            }
        }
    }
}










void Quadratic_probing(vector<studenttype> &data , int count,string filename , int least){

    vector<hash_node> quadratic_table(count, hash_node());
    fstream outfile;

    double unsuccessful_search = 0;
    double successful_search = 0;


    for(int i = 0 ; i < data.size() ; i++){
        int key = 1;
        int sidLen = strlen(data[i].sid);
        for (int j = 0 ; j < sidLen; j++)
        {
            key = (key * data[i].sid[j]) % count;
        }


        for (int k = 0 ;  ; k++)
        {
            // avoid infinity loop
            if(k == count){
                cout << "there are no any suitable location to put the item.\n";
                return;
            }
            successful_search++;
            int index = (key + (long long int)k*k) % count;
            if( quadratic_table[index].status == hash_node_status::Unused ||  quadratic_table[index].status == hash_node_status::Deleted){
                quadratic_table[index].key = key;
                quadratic_table[index].data = data[i];
                quadratic_table[index].status = hash_node_status::Occupied;
                break;
            }
        }
    }



    outfile.open(("quadratic"+filename+".txt"),fstream::out);
    outfile << " --- Hash table created by Quadratic probing ---\n";
    for (int i = 0; i < count ;i++){
        outfile<< '['<< setw(3) <<i<<"] ";
        if(quadratic_table[i].status == hash_node_status::Occupied){
            outfile << setw(10) << quadratic_table[i].key << ", ";
            outfile << setw(10) << quadratic_table[i].data.sid << ", ";
            outfile << setw(10) << quadratic_table[i].data.sname << ", ";
            outfile << toString(quadratic_table[i].data.mean, 10);
        }
        outfile<<"\n";
    }
    outfile << " ----------------------------------------------------- \n";
    outfile.close();


    // unsuccessful search

    for (int po = 0 ; po < count; po++)
    {

        for (int i = 0 ; ; i++)
        {
            unsuccessful_search++;
            if(quadratic_table[(po + (i*i))%count].status == hash_node_status::Unused){
                unsuccessful_search--;
                break;
            }
        }
    }






    cout << "\n\nHash table has been successfully created by Quadratic probing" << endl;
    cout << "unsuccessful search: " << fixed << setprecision(4) << ((unsuccessful_search)/count) <<" comparisons on average"<< endl;
    cout << "successful search: " << fixed << setprecision(4) << ((successful_search)/least) << " comparisons on average"<< endl;


}











void Double_hashing(vector<studenttype> &data , int count,string filename , int least){


    vector<hash_node> quadratic_table(count, hash_node());
    fstream outfile;

    double successful_search = 0;
    double unsuccessful_search = 0;

    int highest = 0;

    // highest pace
    for (int i = ceil( least / 3) ; ; i++)
    {
        if(i < 2)
            continue;
        for (int j = 2 ; j <= i ; j++)
        {
            if(j == i && i != least/3){
                highest = j;
                break;
            }
            if(i % j == 0){
                break;
            }
        }


        if(highest == i){
           break;
        }
    }



    for(int i = 0 ; i < data.size() ; i++){

        int temp = 1;
        int key = 1;
        int sidLen = strlen(data[i].sid) ;
        for (int j = 0 ; j < sidLen; j++)
        {
            key = (key * data[i].sid[j]) % count;
            temp = (temp * data[i].sid[j]) % highest;
        }




        for (int k = 0 ; ; k++)
        {
            if(k == count){
                cout << "There are no any space to insert." << endl;
                return;
            }
            int step = ( (k % count)*(highest-temp) );
            int index = ( key + step ) % count ;

            successful_search++;

            if( quadratic_table[ index ].status == hash_node_status::Unused || quadratic_table[ index ].status == hash_node_status::Deleted ){
                quadratic_table[index].key = key;
                quadratic_table[index].data = data[i];
                quadratic_table[index].status = hash_node_status::Occupied;
                break;
            }
        }
    }



    outfile.open(("double"+filename+".txt"),fstream::out);
    outfile << " --- Hash table created by Double hashing    ---\n";
    for (int i = 0; i < count;i++){
        outfile<< '['<< setw(3) <<i<<"] ";
        if(quadratic_table[i].status == hash_node_status::Occupied){
            outfile << setw(10) << quadratic_table[i].key << ", ";
            outfile << setw(10) << quadratic_table[i].data.sid << ", ";
            outfile << setw(10) << quadratic_table[i].data.sname << ", ";
            outfile << toString(quadratic_table[i].data.mean, 10);
        }
        outfile<<"\n";
    }
    outfile << " ----------------------------------------------------- \n";
    outfile.close();







    cout << "\n\nHash table has been successfully created by Double hashing" << endl;
    cout << "successful search: " << fixed << setprecision(4) << ((successful_search)/least) << " comparisons on average"<< endl;






}










int main(){

    string fileName = "";
    int choice = 0;
    int least = 0;
    // bool ending = false;
    vector<studenttype> datatables={};


    cout<<"******* Hash Table ******"<<endl;
    cout<<"* 0. QUIT              *"<<endl;
    cout<<"* 1. Quadratic probing *"<<endl;
    cout<<"* 2. Double hashing    *"<<endl;
    cout<<"*************************************"<<endl;
    cout<<"Input a choice(0, 1, 2): ";
    cin >> choice;



    while(choice != 0){


        fstream binFile , txtfile;

        do{

            cout << "input a file number: ";
            cin >> fileName;

            if(!fileName.compare("0")){
                return false;
            }



            binFile.open(("input" + fileName + ".bin").c_str(),fstream::in);      // open a file

            if(binFile.is_open() ){
                binFile.close();
                least = readBinary(fileName, datatables);
                break;
            }

            Text2Binary(fileName);

            binFile.open(("input" + fileName + ".bin").c_str(),fstream::in);

            if(binFile.is_open() ){
                binFile.close();
                least = readBinary(fileName, datatables);
                break;
            }

        } while (true);





        int hashtable_num = find_prime(least);




        switch(choice){

            case 1:

                Quadratic_probing(datatables , hashtable_num , fileName , least);
                break;

            case 2:

                Double_hashing(datatables , hashtable_num , fileName , least);
                break;
            default:
                cout<< "Please enter an appropriate choice\n\n"<<endl;
                break;
        }

        datatables.clear();
        fileName.clear();

        cout<<"\n\n******* Hash Table ******"<<endl;
        cout<<"* 0. QUIT              *"<<endl;
        cout<<"* 1. Quadratic probing *"<<endl;
        cout<<"* 2. Double hashing    *"<<endl;
        cout<<"*************************************"<<endl;
        cout<<"Input a choice(0, 1, 2): ";
        cin >> choice;


    }



    system("pause");
    return 0;
}
