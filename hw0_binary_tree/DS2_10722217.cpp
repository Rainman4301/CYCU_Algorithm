
//10722217å»?å®????


#include<iostream>
#include<fstream>
#include<string>
// #include<cstdlib>
// #include<iomanip>
#include<vector>
#include<algorithm>

#include <cstdio>





#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define treeEMPTY(ptr)  (!(ptr))


using namespace std;


//è³??????????

typedef struct node{

    int data;


    string sname;
    string dname;
    string type;
    string level;
    int nstud;
    int nprof;
    int ngrade;


    struct node* lptr;
    struct node* rptr;

    struct node* pare;

} nodetype;

typedef nodetype* bitree_ptr;






void free_tree(bitree_ptr note)
{
    if(note){
        free_tree((note)->lptr);
        free_tree((note)->rptr);
        free(note);
        note = NULL;
    }
}







int treeheight(bitree_ptr root){

    if(root == NULL){
        return 0;
    }
    else{
        int lefttree = treeheight(root->lptr);
        int righttree = treeheight(root->rptr);

        if(lefttree>righttree){
            return lefttree + 1;
        }
        else
            return righttree + 1;
    }


}











bitree_ptr inorderfinde(bitree_ptr root,int limitation){


    bitree_ptr temporary;


    while(root!=NULL){
        if(root->ngrade<limitation){


            if(root->rptr==NULL){
                free_tree(root);
            }
            else if(root->rptr!=NULL){
                root->rptr->pare=root->pare;
                root->pare->lptr=root->rptr;
                root->rptr=NULL;
                free_tree(root);
                root=NULL;
            }


        }
        else if(root->ngrade>limitation){
            root=root->lptr;
        }



    }


}











void inorder(bitree_ptr root){


    if(root){
        inorder(root->lptr);
        cout<<root->sname<<endl;
        inorder(root->rptr);
    }

}






bitree_ptr findenode(bitree_ptr node , int element){


    bitree_ptr p=NULL;

    while(node){


        p = node ;

        if(element < p->ngrade){
            node = node->lptr;
        }
        else if(element > p->ngrade){
            node = node->rptr;
        }
        else
            node = node->rptr;
    }

    return p;


}






void bi_insertTree( bitree_ptr &nodeqq,nodetype element ){





    bitree_ptr temp, point;

    //??¾å?°è?????çµ????ç¯?é»?
    point = findenode((nodeqq), element.ngrade);


        //??¤æ?·æ¨¹??¯å?¦ç©º????????????æ²??????¾å?°å?¯æ????¥é??
    if( treeEMPTY(nodeqq) || point){

        temp = new nodetype;


        if(!temp){
            fprintf(stderr,"The memory is FULL!!\n");
            exit(1);
        }


        temp->lptr = NULL;
        temp->rptr = NULL;
        temp->pare = NULL;
        temp->data = element.data;
        temp -> sname = element.sname;
        temp -> dname = element.dname;
        temp -> type = element.type;
        temp -> level = element.level;
        temp -> nstud = element.nstud;
        temp -> nprof = element.nprof;
        temp -> ngrade = element.ngrade;



        //æ±ºå????°å????¥ç??ç¯?é»?è¦???¾å·¦????????¯å?³é??
        if(nodeqq){

            if(element.ngrade >= point->ngrade){
                temp->pare = point;
                point->rptr=temp;
            }
            else if(element.ngrade < point->ngrade ){
                temp->pare = point;
                point->lptr=temp;
            }
            else{
                temp->pare = point;
                point->rptr=temp;
            }
        }

        else nodeqq = temp;

    }




}







int main()
{

//??ºä????¯ä»¥è¼¸å?¥è????¸æ?????è³????æª?æ¡?
    string file_number ="";

    cout<<"Input a file number: ";
    cin >> file_number;

    file_number = "input"+file_number + ".txt";



        //ifsæª?æ¡?æµ?
    std::ifstream ifs;

    ifs.open(file_number);
    //ç¢ºå??æª?æ¡????è®??????????
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
        return 1; // EXIT_FAILURE
    }


    string tp;
    int num;
    int count = 0;
    vector <nodetype> sugardady;

    node temp;
    int column_index = 0;
    int count_ = 0;
    //ä¸?æ¬¡è???????????æª?æ¡?
    while (ifs >> tp)
    {



        //???ä¸?è¡?ä¸?è®????
        if(count_>=15){


            if(column_index % 15== 0)
            {
                temp.data = stoi(tp);
            }
            else if(column_index % 15== 1)
            {
                temp.sname = tp;
            }
            else if(column_index %15  == 3)
            {
                temp.dname = tp;
            }
            else if(column_index%15 == 4)
            {
                temp.type = tp;
            }
            else if(column_index%15 == 6)
            {
                temp.level = tp;
            }
             else if(column_index%15 == 8)
            {
                num = std::stoi(tp);
                temp.nstud = num;
            }
            else if(column_index%15 == 9)
            {
                num = std::stoi(tp);
                temp.nprof = num;
            }
             else if(column_index%15 == 10)
            {
                num = std::stoi(tp);
                temp.ngrade = num;
            }

            else if(column_index%15 == 14)
            {
                sugardady.push_back(temp);
                //cout<<"-----------------------------------------------------------------------------\n";
            }
            column_index++;

        }


        count_++;


    }




    //Binary Search Tree
    bitree_ptr root = NULL;
    for(int i=0;i<sugardady.size();i++){
        bi_insertTree( root, sugardady[i] );
    }



    inorder(root);

    printf("Tree height: %d\n", treeheight(root));




/*

    for(int i=0;i<sugardady.size();i++)
    {

        cout << "number: " << i << " data" <<endl;

        cout <<"code: "<<sugardady[i].data<<endl;
        cout <<"sname: "<<sugardady[i].sname<<endl;
        cout <<"dname: "<<sugardady[i].dname<<endl;
        cout <<"type: "<<sugardady[i].type<<endl;
        cout <<"level: "<<sugardady[i].level<<endl;

        cout <<"nstud: "<<sugardady[i].nstud<<endl;
        cout <<"nprof: "<<sugardady[i].nprof<<endl;
        cout <<"ngraduate: "<<sugardady[i].ngrade<<endl;
        cout <<"-------------------------------------------"<<endl;
    }
*/




        //??¾å?°ä??å¹´åº¦??¢æ¥­???äººæ?¸ä????¼deletenumä¸¦å?ªé??
    int deletenum;
    cout<<"Imput the number of graduate: ";
    cin>> deletenum;
    inorderfinde(root,deletenum);
    inorder(root);

    printf("Tree height: %d\n", treeheight(root));





    return 0;
}

