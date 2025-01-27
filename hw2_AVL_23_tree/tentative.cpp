// 10722217廖宇傑

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


//**********************************************************************************************

typedef struct node
{
    // 存取第n筆資料
    int data;
    string school_name;
    string major_name;
    string nighday;
    string level;
    // 去年畢業生人數
    int ngrade;

} nodetype;
typedef nodetype *nodetype_ptr;



//**********************************************************************************************



#define maxptr 3



typedef struct tree2_3
{
    // vector<nodetype> file[2];
    vector<nodetype> key[2];
    tree2_3 *ptr[maxptr];
    tree2_3 *parent;
 
} tree2_3;
typedef tree2_3 *tree2_3_ptr;



typedef struct Avltree
{
    vector<nodetype> key[2];
    Avltree *lef_ptr;
    Avltree *rig_ptr;
    Avltree *parent;
}avltree;
typedef Avltree *avl_ptr;

//**********************************************************************************************


typedef struct temp{
    vector<nodetype> key[3];
    temp *ptr[maxptr];
    temp *parent;
}tempdata ;

//**********************************************************************************************




bool cmp(vector<int> v1, vector<int> v2){
    return v1[0] < v2[0];
}

int main(){

    nodetype sss;
    sss.school_name = "HHHHHHHHHH";

    tree2_3_ptr temp = new tree2_3;

    cout<< temp->key->empty()<<endl;
    cout<< temp->key[0].empty()<<endl;
    cout<< temp->key[0].empty()<<endl;
    cout<< temp->key->size()<<endl;

    cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;

    temp->key[0].push_back(sss);
    temp->key[0].push_back(sss);
    temp->key[0].push_back(sss);
    temp->key[0].push_back(sss);
    temp->key[1].push_back(sss);
    temp->key[1].push_back(sss);
    temp->key[1].push_back(sss);
   
    cout<< temp->key->size()<<endl;
    cout<< temp->key[0].size()<<endl;
    cout<< temp->key[1].size()<<endl;
    temp->key[0].clear();
    cout<< temp->key->size()<<endl;
   

    
    cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
    cout<< temp->key->empty()<<endl;
    cout<< temp->key[0].empty()<<endl;
    cout<< temp->key[1].empty()<<endl;

    // tempdata container;

    // container.key[0] = temp->key[0];

    // for (int i=0; i < container.key[0].size();i++)
    // {
    //     cout << i << endl;
    //     cout << container.key[0][i].school_name<<endl;
    //     cout << temp->key[0][i].school_name<< endl;

    // }



    cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
    
    /*
    
    tempdata contain;
    nodetype a, b, c;

    a.school_name.append("abcde");
    b.school_name.append("bcde");
    c.school_name.append("cde");

    contain.key[0].push_back(a);
    contain.key[1].push_back(b);
    contain.key[2].push_back(c);

    for (int i = 0; i < 3;i++)
    {
        cout << contain.key[i][0].school_name << endl;
    }

    // sort(contain.key.begin(), contain.end(), cmp);

    */

    return 0;
}