#include <string>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])

{

    string g_str = "Wj Cy Beijing";

    string tmp = g_str;



    char *p_str = (char *)tmp.c_str();

    char *saveptr = NULL;

    

    int id = 0;

    bool bCheck = true;




    cout << "g_str: " << g_str << endl;

    cout << "tmp:   " << tmp << endl;

    cout << "****************" << endl;

    char *p = strtok_r(p_str, " ", &saveptr);

    cout << "g_str: " << g_str << endl;

    cout << "tmp:   " << tmp << endl;

    cout << "****************" << endl;

    strtok_r(NULL, " ", &saveptr);

    cout << "g_str: " << g_str << endl;

    cout << "tmp:   " << tmp << endl;









    string str1 ="hello world";

    string str2 = str1;



    printf ("Sharingthe memory:\n");

    printf ("\tstr1's address:%x\n", str1.c_str() );

    printf ("\tstr2'saddress: %x\n", str2.c_str() );

    

    str1[1]='q';

    str2[1]='w';



    printf ("AfterCopy-On-Write:\n");

    printf ("\tstr1'saddress: %x\n", str1.c_str() );

    printf ("\tstr2'saddress: %x\n", str2.c_str() );







    return 1;
}









/*

#include <iostream> 
#include <string> 
using namespace std; 

int main() { 
    string str1; 
    string str2("caterpillar"); 
    string str3(str2); 

    // assign: 指定字串 
    str1 = str1.assign(str2, 0, 5); 
    cout << "str1: " << str1 << endl; 
    str1 = str1.assign("caterpillar", 5, 6); 
    cout << "str1: " << str1 << endl; 

    str1 = ""; 

    // append: 字串串接 
    str1 = str1.append(str2, 0, 5); 
    str1 = str1.append(str3, 5, 6); 
    cout << "str1: " << str1 << endl; 

    // find: 尋找字串位置 
    cout << "尋找str1中的第一個pill: " 
         << str1.find("pill", 0) << endl; 

    // insert: 插入字串 
    cout << "在str1插入字串***: " 
         << str1.insert(5, "***") << endl; 

    cout << "str1長度: " << str1.length() << endl; 
 
    return 0; 
}
*/



/*


int main(){


    vector<nodetype> sugardady;

    
    

    //資料的讀取
    string file_number = "";
    cout << "Input a file number ([0] Quit): ";
    cin >> file_number;
    file_number = "input" + file_number + ".txt";
    std::ifstream ifs(file_number,ios::in);

    
    if (!ifs.is_open())
    {
        cout << "Failed to open file.\n";
        return 1; // EXIT_FAILURE
    }

    string space = " ";
    string s;
    while (getline(ifs, s)) {

        continue;
        continue;
        continue;
        
        vector<string> result;
        string::size_type begin, end;
        end = s.find(space);
        begin = 0;

        while (end != string::npos) {
            if (end - begin != 0) {
                result.push_back(s.substr(begin, end-begin)); 
            }    
            begin = end + space.size();
            end = s.find(space, begin);

            if (begin != s.length()) {
            result.push_back(s.substr(begin));
            }
        }
        for(int i = 0; i < sizeof(result); i++){
            cout << result[i] << endl;
        }  
    }
}

*/