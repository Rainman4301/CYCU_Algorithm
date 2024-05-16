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




typedef struct code{
    int num;
    int gra_num;
    string school_name;
}code;

typedef code *code_ptr;

//**********************************************************************************************


#define Maxkids 2
#define maxptr 3



typedef struct tree2_3
{
    // vector<nodetype> file[2];
    vector<code> key[2];
    tree2_3 *ptr[maxptr];
    tree2_3 *parent;
 
} tree2_3;
typedef tree2_3 *tree2_3_ptr;


typedef struct temp{
    vector<code> key[3];
    tree2_3 *ptr[4];
    tree2_3 *parent;
}tempdata ;



//**********************************************************************************************



typedef struct Avltree
{
    vector<code> key;
    Avltree *lef_ptr;
    Avltree *rig_ptr;
}avltree;
typedef Avltree *avl_ptr;


//**********************************************************************************************





int treeheight(tree2_3_ptr root){
    if(root == NULL){
        return 0;
    }
    else{
        int lefttree = treeheight(root->ptr[0]);
        int medium = treeheight(root->ptr[1]);
        int righttree = treeheight(root->ptr[2]);

        if(lefttree>righttree){
            return lefttree + 1;
        }
        else if(medium >= lefttree && medium <= righttree){
            return medium + 1;
        }
        else{
            return righttree + 1;
        }       
    }
}


int treeheight(avl_ptr root){
    if(root == NULL){
        return 0;
    }
    else{
        int lefttree = treeheight(root->lef_ptr);
        int righttree = treeheight(root->rig_ptr);

        if(lefttree>righttree){
            return lefttree + 1;
        }
        else
            return righttree + 1;
    }
}



void memory_relaese( avl_ptr root){
    if(root){
        memory_relaese(root->lef_ptr);
        memory_relaese(root->rig_ptr);
        free (root);
    }
}

void memory_relaese( tree2_3_ptr root){
    if(root){
        memory_relaese(root->ptr[0]);
        memory_relaese(root->ptr[1]);
        memory_relaese(root->ptr[2]);
        free (root);
    }
}




//AVL tree 的建立
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&









// find successor point

void successor_node( avl_ptr point){

    avl_ptr temp;
    point = point->rig_ptr;
    while(point){
        temp = point;
        point = point->lef_ptr;
    }

    swap(point->key,temp->key);
    delete(temp);
}




avl_ptr rr_rotation(avl_ptr point){
    avl_ptr temp = point->rig_ptr;
    point->rig_ptr = temp->lef_ptr;
    temp->lef_ptr = point;

    return temp;
}

avl_ptr ll_rotation(avl_ptr point){
    avl_ptr temp = point->lef_ptr;
    point->lef_ptr = temp->rig_ptr;
    temp->rig_ptr = point;

    return temp;
}



avl_ptr rl_rotation(avl_ptr point){
    point->rig_ptr = ll_rotation(point->rig_ptr);

    return rr_rotation(point);
}

avl_ptr lr_rotation(avl_ptr point){
    point->lef_ptr = rr_rotation(point->lef_ptr);

    return ll_rotation(point);
}


//上面的return是為了讓point上的父可以連到temp





void check_height(avl_ptr &real_root, vector<avl_ptr> track){

    avl_ptr up=NULL, mediator=NULL ,down=NULL;

    

    int position = (int)track.size();
    for (int i = position-2;i>=0;i--)
    {

        if(i-1==-1){
            up = NULL;
        }else{
            up = track[i-1];
        }
        mediator = track[i];
        down = track[i+1];
        
        if(treeheight(mediator->lef_ptr)-treeheight(mediator->rig_ptr) == 2 ){

            //ROOT
            if(i==0){
                //LL
                if(treeheight(down->lef_ptr) - treeheight(down->rig_ptr) >= 0){
                    real_root = ll_rotation(mediator);
                }
                //LR
                else{
                    real_root = lr_rotation(mediator);
                }

            }

            //非ROOT
            else{
                //LL
                if(treeheight(down->lef_ptr) - treeheight(down->rig_ptr) >= 0){
                    //左邊
                    if (up->lef_ptr == mediator)
                    {
                        up->lef_ptr = ll_rotation(mediator);
                    }
                    //右邊
                    else{
                        up->rig_ptr = ll_rotation(mediator);
                    }
                }
                //LR
                else{
                    //左邊
                    if (up->lef_ptr == mediator)
                    {
                        up->lef_ptr = lr_rotation(mediator);
                    }
                    //右邊
                    else{
                        up->rig_ptr = lr_rotation(mediator);
                    }

                }

            }

                     
        }
        


        else if(treeheight(mediator->lef_ptr) - treeheight(mediator->rig_ptr) == -2){

            //ROOT
            if(i==0){
                //RR
                if(treeheight(down->lef_ptr) - treeheight(down->rig_ptr) <= 0){
                    real_root = rr_rotation(mediator);
                }
                //RL
                else{
                    real_root = rl_rotation(mediator);
                }

            }

            //非ROOT
            else{
                //RR
                if(treeheight(down->lef_ptr) - treeheight(down->rig_ptr) <= 0){
                    //左邊
                    if (up->lef_ptr == mediator)
                    {
                        up->lef_ptr = rr_rotation(mediator);
                    }
                    //右邊
                    else{
                        up->rig_ptr = rr_rotation(mediator);
                    }
                }
                //RL
                else{
                    //左邊
                    if (up->lef_ptr == mediator)
                    {
                        up->lef_ptr = rl_rotation(mediator);
                    }
                    //右邊
                    else{
                        up->rig_ptr = rl_rotation(mediator);
                    }

                }

            }
            
        }
        
    }
}







// findenode


avl_ptr findenode(avl_ptr node , code element,vector<avl_ptr> &track){


    avl_ptr p=NULL;

    while(node){

        p = node ;
        track.push_back(p);

        if(element.gra_num < p->key[0].gra_num){
            node = node->lef_ptr;
        }
        else if(element.gra_num > p->key[0].gra_num){
            node = node->rig_ptr;
        }
        else if(element.gra_num == p->key[0].gra_num){
            p->key.push_back(element);
            p = NULL;
            return p;
        }

    }

    return p;


}



// AVL_Tree_insert


void AVL_Tree_insert( avl_ptr &root,nodetype element,int *num ){


    avl_ptr temp, point;
    //////
    code maga;
    maga.num = element.data;
    maga.gra_num = element.ngrade;
    //////

    vector<avl_ptr> track;
    track.clear();
    point = findenode(root, maga, track);
   
    if( !(root) || point){

        temp = new avltree;
        (*num)++;
        temp->lef_ptr = NULL;
        temp->rig_ptr = NULL;
        
        temp->key.push_back(maga);

        if(!temp){
            fprintf(stderr,"The memory is FULL!!\n");
            exit(1);
        }

        if(root){

            if(element.ngrade > point->key[0].gra_num){
                // temp->parent = point;
                point->rig_ptr=temp;
            }
            else if(element.ngrade < point->key[0].gra_num ){
                // temp->parent = point;
                point->lef_ptr=temp;
            }
            
        }

        else{
            root=temp;
        }

        track.push_back(temp);
        check_height(root,track);
        // check_height1(temp,root,root,root);
    }

    
}



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//2_3 tree 的建立
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&






void comp(tempdata *patient){
    if((*patient).key[2][0].school_name < (*patient).key[0][0].school_name){
        swap((*patient).key[2],(*patient).key[0]);
        if((*patient).key[2][0].school_name < (*patient).key[1][0].school_name){
            swap((*patient).key[2],(*patient).key[1]);
        }
    }
    else if((*patient).key[2][0].school_name < (*patient).key[1][0].school_name && (*patient).key[2][0].school_name > (*patient).key[0][0].school_name){
        swap((*patient).key[2],(*patient).key[1]);
    }

}






tree2_3_ptr node_create()
{

    tree2_3_ptr temp = new tree2_3;

    // temp->key = NULL;

    temp->parent = NULL;
    temp->ptr[0] = NULL;
    temp->ptr[1] = NULL;
    temp->ptr[2] = NULL;

    return temp;
}











void split( vector<code> data,tree2_3_ptr &pivot, vector<tree2_3_ptr> track , int *num){

    tempdata container;
    vector<tree2_3_ptr> new_node;
    // new_node.push_back(NULL);


    
    container.key[2] = data;
    for (int i = track.size() - 1,j=0; i >= 0;i--,j++)
    {
        container.key[0] = track[i]->key[0];
        container.key[1] = track[i]->key[1];    
        comp(&container);

        container.ptr[0] = track[i]->ptr[0];
        container.ptr[1] = track[i]->ptr[1];
        container.ptr[2] = track[i]->ptr[2];

        tree2_3_ptr temp = node_create();
        (*num)++;
        temp->key[0] = container.key[2];
        new_node.push_back(temp);

        track[i]->key[1].clear();
        track[i]->key[0].clear();
        track[i]->key[0] = container.key[0];
        track[i]->ptr[2] = NULL;
        

        
        if(i-1 >= 0){

            if(track[i-1]->key[1].empty()){

                track[i - 1]->key[1] = container.key[1];

                if(track[i-1]->ptr[0] == track[i]){                     //left
                    
                    swap(track[i - 1]->key[1],track[i - 1]->key[0]);
                    track[i - 1]->ptr[2] = track[i - 1]->ptr[1];
                    track[i - 1]->ptr[1] = temp;
                }
                else if(track[i-1]->ptr[1] == track[i]){                //right
                    
                    track[i - 1]->ptr[2] = temp;
                }



                if(track.size()-1 > i){


                    //要考慮是在左中右
                    if(track[i+1] == container.ptr[0]){         //left

                        track[i]->ptr[1] = new_node[j - 1];     // 上層新增
                        temp->ptr[0] = container.ptr[1];        // 當層分裂
                        temp->ptr[1] = container.ptr[2];        // 當層新增

                    }
                    else if(track[i+1] == container.ptr[1]){    //medium
                        new_node[j]->ptr[0] = new_node[j - 1];  // 上層新增
                        new_node[j]->ptr[1] = container.ptr[2]; // 當層新增

                    }
                    else if(track[i+1]  == container.ptr[2]){    //right
                        temp->ptr[0] = track[i + 1];            // 上層分裂
                        temp->ptr[1] = new_node[j-1];           // 上層新增
                    }

                }


                break;
            }

        }
        

        if(track.size()-1 > i){


            //要考慮是在左中右
            if(track[i+1] == container.ptr[0]){         //left

                track[i]->ptr[1] = new_node[j - 1];
                temp->ptr[0] = container.ptr[1];        // 當層分裂
                temp->ptr[1] = container.ptr[2];        // 當層新增

            }
            else if(track[i+1] == container.ptr[1]){    //medium
                new_node[j]->ptr[0] = new_node[j - 1];  // 上層新增
                new_node[j]->ptr[1] = container.ptr[2]; // 當層新增

            }
            else if(track[i+1]  == container.ptr[2]){    //right
                temp->ptr[0] = track[i + 1];            // 上層分裂
                temp->ptr[1] = new_node[j-1];           // 上層新增
            }

        }


        if(i == 0){

            tree2_3_ptr root = node_create();
            (*num)++;
            root->key[0] = container.key[1];
            root->ptr[0] = track[i];
            root->ptr[1] = temp;
            pivot = root;

            break;
        }
        
        container.key[2] = container.key[1];

    }

    


}








//string compare
tree2_3_ptr finde_node(tree2_3_ptr node, code data, vector<tree2_3_ptr> &track){

    tree2_3_ptr temp ;
    while(node){


        temp = node;
        track.push_back(temp);

        
        if(temp->key[0][0].school_name == data.school_name ){
            temp->key[0].push_back(data);
            return NULL;
        }
        else if(!(temp->key[1].empty()) ){

            if(temp->key[1][0].school_name == data.school_name){
                temp->key[1].push_back(data);
            return NULL;
            }
        }
        
        

        
        if(!(temp->key[1].empty())){
            if(data.school_name < temp->key[0][0].school_name ){
            node = node->ptr[0];
            }
            else if(data.school_name >temp->key[0][0].school_name && data.school_name < temp->key[1][0].school_name ){
                node = node->ptr[1];
            }
            else if(data.school_name > temp->key[1][0].school_name){
                node = node->ptr[2];
            }
        }
        else{
            if(data.school_name < temp->key[0][0].school_name){
                node = node->ptr[0];
            }
            else{
                node = node->ptr[1];
            }
        }
    }

    return temp;
    

}





void tree2_3_insert(tree2_3_ptr &ptr, nodetype resource, int *num)
{
    code subscription;
    subscription.num = resource.data;
    subscription.school_name = resource.school_name;
    vector<code> input;
    input.push_back(subscription);

    vector<tree2_3_ptr> track;

    if (!ptr)
    {
        tree2_3_ptr temp = node_create();
        (*num)++;
        temp->key[0].push_back(subscription);
    
        ptr = temp;
    }

    else{

        tree2_3_ptr insert_node = finde_node(ptr, subscription,track);

    
        if(insert_node){

            //分裂插入
            if(!(insert_node->key[1].empty())){

                split( input ,ptr, track, &(*num));
            }

            //直接插入
            else{
                insert_node->key[1].push_back(subscription);
                if(insert_node->key[0][0].school_name > insert_node->key[1][0].school_name){
                    swap(insert_node->key[0],insert_node->key[1]);
                }
            }
        }
    }
    

    input.clear();
    track.clear();
    

}




//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&







void GetStringFromCSV(string line, int nIdx, string &str)
{
    int nSPos = 0;
    for (int i = 0; i < nIdx - 1; ++i)
    {
        nSPos = line.find('\t', nSPos);
        ++nSPos;
    }

    int nEPos = line.find('\t', nSPos);
    if (nEPos != string::npos)
    {
        str = line.substr(nSPos, nEPos - nSPos);
    }
    else
    {
        str = line.substr(nSPos, line.size() - 1 - nSPos);
    }
}









int main()
{
    nodetype random;
    vector<nodetype> sugardady;
    

    int option;
    cout << "*** Search Tree Utilities **" << endl;
    cout << "* 0. QUIT                  *" << endl;
    cout << "* 1. Build 2-3 tree        *" << endl;
    cout << "* 2. Build AVL tree        *" << endl;
    cout << "*************************************" << endl;
    cout << "Input a choice(0, 1, 2):";
    cin >> option;

    while (option != 0)
    {

        sugardady.push_back(random);

        //**********************************************************************************************

        // 資料的讀取
        string file_number = "";
        cout << "Input a file number ([0] Quit): ";
        cin >> file_number;
        file_number = "input" + file_number + ".txt";
        std::ifstream ifs(file_number, ios::in);

        if (!ifs.is_open())
        {
            cout << "Failed to open file.\n";
            return 1; // EXIT_FAILURE
        }

        int flow = 0;
        int match[] = {2, 4, 5, 6, 9, 10};
        string s, sTarget;
        getline(ifs, s);
        getline(ifs, s);
        getline(ifs, s);
        while (getline(ifs, s))
        {
            nodetype temp;
            for (int g : match)
            {
                if(g == 2)
                {
                    GetStringFromCSV(s,g,sTarget);
                    temp.school_name.append(sTarget);
                }
                else if ( g == 4)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.major_name.append(sTarget);
                }
                else if (g == 5)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.nighday.append(sTarget);
                }
                else if (g == 6)
                {
                    GetStringFromCSV(s, g, sTarget);
                    // sTarget.erase(remove(sTarget.begin(), sTarget.end(), '"'), sTarget.end());
                    // sTarget.erase(remove(sTarget.begin(), sTarget.end(), ','), sTarget.end());

                    string::iterator f;
                    char c = '"';
                    char p = ',';
                    for (f = sTarget.begin(); f != sTarget.end(); f++)
                    {
                        if (*f == '"' || *f == ',')
                        {
                            sTarget.erase(f);
                            f--;
                        }
                    }

                    temp.level.append(sTarget);
                }
                else if (g == 9)
                {
                    GetStringFromCSV(s, g, sTarget);
                    if(sTarget == "-"){
                        temp.ngrade = 0;
                    }
                    else{
                        temp.ngrade = stoi(sTarget);
                    }
                    
                }
                else
                {
                    temp.data = ++flow;
                    sugardady.push_back(temp);
                }
            }
        }




        // for (int i = 0; i < sugardady.size(); i++)
        // {
        //     cout<< sugardady[i].data << endl;
        //     cout<< sugardady[i].school_name << endl;
        //     cout<< sugardady[i].major_name << endl;
        //     cout<< sugardady[i].nighday << endl;
        //     cout<< sugardady[i].level << endl;
        //     cout<< sugardady[i].ngrade <<"\n\n"<< endl;
        // }

        ifs.close();

        //**********************************************************************************************
        tree2_3_ptr root1 = NULL;
        avl_ptr root2 = NULL;
        int num = 0;
        switch (option)
        {
            case 1:

                
                for(int i = 1; i < sugardady.size(); i++){

                    tree2_3_insert(root1, sugardady[i], &num);

                }

                cout<<"Tree height = "<<treeheight(root1)<< endl;
                cout<<"Number of nodes = "<<num<< endl;

                for (int i = 1, j = 1; i >= 0; i--)
                {
                    for(int k = 0; k < root1->key[i].size();k++,j++){
                        cout<<j<<" : "<<" ["<<sugardady[root1->key[i][k].num].data<<"] "<<sugardady[root1->key[i][k].num].school_name<<","<<sugardady[root1->key[i][k].num].major_name<<","<<sugardady[root1->key[i][k].num].nighday<<","<<sugardady[root1->key[i][k].num].level<<","<<sugardady[root1->key[i][k].num].ngrade<<endl;
                    }
                }

                // for (int i = 0,j=1; i < root1->key[0].size();i++,j++)
                // {
                //     cout<<j<<" : "<<" ["<<sugardady[root1->key[0][i].num].data<<"] "<<sugardady[root1->key[0][i].num].school_name<<","<<sugardady[root1->key[0][i].num].major_name<<","<<sugardady[root1->key[0][i].num].nighday<<","<<sugardady[root1->key[0][i].num].level<<","<<sugardady[root1->key[0][i].num].ngrade<<endl;
                // }
                // for (int i = 0,j=1; i < root1->key[1].size();i++,j++)
                // {
                //     cout<<j<<" : "<<" ["<<sugardady[root1->key[1][i].num].data<<"] "<<sugardady[root1->key[1][i].num].school_name<<","<<sugardady[root1->key[1][i].num].major_name<<","<<sugardady[root1->key[1][i].num].nighday<<","<<sugardady[root1->key[1][i].num].level<<","<<sugardady[root1->key[1][i].num].ngrade<<endl;
                // }

                memory_relaese(root1);
                break;

            case 2:

                
                for(int i = 1; i < sugardady.size(); i++){

                    AVL_Tree_insert(root2, sugardady[i], &num);

                }
                

                cout<<"Tree height = "<<treeheight(root2)<< endl;
                cout<<"Number of nodes = "<<num<< endl;
                for (int i = 0,j=1; i < root2->key.size();i++,j++)
                {
                    cout<<j<<" : "<<" ["<<sugardady[root2->key[i].num].data<<"] "<<sugardady[root2->key[i].num].school_name<<","<<sugardady[root2->key[i].num].major_name<<","<<sugardady[root2->key[i].num].nighday<<","<<sugardady[root2->key[i].num].level<<","<<sugardady[root2->key[i].num].ngrade<<endl;
                }

                memory_relaese(root2);

                break;

            default:

                break;
            }
        


        cout << "\n\n*** Search Tree Utilities **" << endl;
        cout << "* 0. QUIT                  *" << endl;
        cout << "* 1. Build 2-3 tree        *" << endl;
        cout << "* 2. Build AVL tree        *" << endl;
        cout << "*************************************\n\n\n" << endl;
        cout << "Input a choice(0, 1, 2):";
        cin >> option;

        sugardady.clear();
    }

    return 0;
}
