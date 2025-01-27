
// 10722217���t��

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include<cmath>

#include <cstdio>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_EMPTY(ptr) (!(ptr));
#define SWAP(a, b, temp) ((temp = a), (a = b), (b = temp));

using namespace std;

typedef struct node
{

    int data;
    int ngrade;

} nodetype;
typedef nodetype *bitree_ptr;


vector<nodetype> minheap;
int n = 0;









//min_max_heap ���ظm

void insert_min_max_heap(nodetype element){

    nodetype temp;
    int k=0;
    int i = ++n;
    minheap.push_back(element);
    if(i>3){

        //�ΨӧP�_�b���h
        int min_max = int(floor(log2(i)))%2;

        // min�h
        if (min_max == 0)
        {
            
            if(minheap[i].ngrade>minheap[i/2].ngrade){
                SWAP(minheap[i], minheap[i / 2], temp);
                // �qmin�h��max�h
                i = i / 2;
                while((i/2)>3){
                    if(minheap[i].ngrade>minheap[i/4].ngrade){
                        SWAP(minheap[i],minheap[i/4],temp);
                    }
                    i = i / 4;
                }
            }
            else{
                while((i/2)>1){
                    if(minheap[i].ngrade<minheap[i/4].ngrade){
                        SWAP(minheap[i],minheap[i/4],temp);
                    }
                    i = i / 4;
                }
            }
            
            
        }
        //max�h
        else{

            
            if(minheap[i].ngrade<minheap[i/2].ngrade){
                SWAP(minheap[i], minheap[i / 2], temp);
                // �qmax�h��min�h
                i = i / 2;
                while((i/2)>1){
                    if(minheap[i].ngrade<minheap[i/4].ngrade){
                        SWAP(minheap[i],minheap[i/4],temp);
                    }
                    i = i / 4;
                }
            }
            else{
                while((i/2)>3){
                    if(minheap[i].ngrade>minheap[i/4].ngrade){
                        SWAP(minheap[i],minheap[i/4],temp);
                    }
                    i = i / 4;
                }
            }
            
        }

    }

    //�e3�h
    else{
        if(i==1){
        }
        else{
            if(element.ngrade<minheap[i/2].ngrade){
                SWAP(minheap[i], minheap[i / 2], temp);
            }

        }

    }


}







//�R���̤p��
/*
nodetype delete_max_heap(int *n)
{
    int parent, child;

    nodetype item, temp;

    if(HEAP_EMPTY(*n)){
        fprintf(stderr, "The heap is empty\n");
        exit(1);
    }

    //�R�����̤p��
    item = minheap[1];

    //�̫�@�Ӥ���
    temp = minheap[(*n)--];

    parent = 1; child = 2;

    //�����l�`�I���ӳ̤p��
    while (*n >= child) {
        if((*n > child) && (minheap[child].ngrade > minheap[child + 1].ngrade)){
            child++;
        }

    //�b�P�_ �̫� ��Ӹ`�I��ݽ֤j �M�w�W�� or �ɸ`�I�ƭ�
        if(temp.ngrade < minheap[child].ngrade ){
            break;
        }

    //�p�G�S�i�J�W�h�禡���ܥN���ɪ� heap�̫�@����(temp) > ��L���l�`�I(child)
        minheap[parent] = minheap[child];  
        parent = child;            
        child = child*2;           
    }

    
    minheap[parent] = temp;

    return item;


}

*/






//min_heap���ظm
void insert_minimum_heap(nodetype element)
{
    
    int i = 0;
    i = ++n;
    //cout << i << endl;
    while ((i != 1) && element.ngrade < minheap[i / 2].ngrade)
    {
        //cout << "123" << endl;
        minheap[i] = minheap[i / 2];
        i = i / 2;
    }
    //cout << "123" << endl;
    minheap[i] = element;
}









int main()
{

    int chose;
    vector<nodetype> sugardady;

    cout << "**** Heap Construction *****" << endl;
    cout << "* 0. QUIT                  *" << endl;
    cout << "* 1. Build a min heap      *" << endl;
    cout << "* 2. Build a min-max heap" << endl;
    cout << "*************************************" << endl;
    cout << "Input a choice(0, 1, 2):";
    cin >> chose;
    cout << "mychoice" << chose << endl;
    while (chose != 0)
    {
        cout << " top choice " << chose << endl;
        n = 0;
        //vector�q1�}�l�s����Ƥ�K�p��
        //0225--update 
        nodetype fake;
        fake.data = 0;
        fake.ngrade = 0;
        minheap.push_back(fake);

        //��ƪ�Ū��

        string file_number = "";
        cout << " before input file " << endl;
        cout << "Input a file number ([0] Quit): ";
        cin >> file_number;
        cout << "after input file " << endl;
        if(stoi(file_number)==0){
            cout << "file number break" << endl;
            break;
        }

        file_number = "input" + file_number + ".txt";
        cout << "file_+number " << file_number << endl;

        //
        // std::ifstream ifs;
        std::ifstream ifs(file_number.c_str() ,ios::in);            //�}��
        //

        cout << "after ifs" << endl;
        cout << "filenumber " << file_number << endl;
        try{
            //ifs.open(); // ifs.open ??
        }
        catch(const std::exception& ex)
        {
            cout << "entering err" << endl;
            std::cerr << "Error occurred: " << ex.what() << std::endl;
        }
        catch(const std::runtime_error& re)
        {
            cout << "entering rE" << endl;
            // speciffic handling for runtime_error
            std::cerr << "Runtime error: " << re.what() << std::endl;
        }
        cout << "before checking fail" << endl;
        if(ifs.fail())
        {
            cout << "entering" << endl;
            cerr << "Error: " << strerror(errno);
        }
        cout << "after opend" << endl;
        if (!ifs.is_open())
        {
            cout << "Failed to open file.\n";
            return 1; // EXIT_FAILURE
        }

        
        
        int num;
        int count = 0;
        node temp;
        int column_index = 0;
        int count_ = 0;
        string tp;
        cout << "before 265 :" << chose << endl;
        while (ifs >> tp)
        {
            // �q��15���}�l�~�O�ڭ̻ݭn�����
            if (count_ >= 15)
            {

                if (column_index % 15 == 0)
                {
                    temp.data = stoi(tp);
                }
                else if (column_index % 15 == 10)
                {
                    num = std::stoi(tp);
                    temp.ngrade = num;
                }

                else if (column_index % 15 == 14)
                {
                    sugardady.push_back(temp);
                }
                column_index++;
            }
            count_++;
        }

        tp.clear();

        ifs.close();

        switch(chose)
        {

            case (1):{

                for (int i = 0; i < sugardady.size(); i++)
                {
                    insert_minimum_heap(sugardady[i]);
                }

                cout << "<min heap>" << endl;
                cout << "root: " <<'['<<minheap[1].data<<']'<<minheap[1].ngrade<< endl;
                cout << "bottom: " <<'['<<minheap[n].data<<']'<<minheap[n].ngrade << endl;

                //���F���̥���`�I�����
                int i = 0;
                for (int j = pow(2,i); j <= n;i++,j=pow(2,i)){
                }
                i--;

                cout << "leftmost bottom: "<<'['<<minheap[pow(2,i)].data<<']'<<minheap[pow(2,i)].ngrade << endl;
                printf("\n\n");
                break;
            }
            case (2):{

                for (int i = 0; i < sugardady.size(); i++)
                {
                    insert_min_max_heap(sugardady[i]);
                }

                cout << "<min heap>" << endl;
                cout << "root: " <<'['<<minheap[1].data<<']'<<minheap[1].ngrade<< endl;
                cout << "bottom: " <<'['<<minheap[n].data<<']'<<minheap[n].ngrade << endl;

                //���F���̥���`�I�����
                int i = 0;
                for (int j = pow(2,i); j <= n;i++,j=pow(2,i)){
                }
                i--;

                cout << "leftmost bottom: "<<'['<<minheap[pow(2,i)].data<<']'<<minheap[pow(2,i)].ngrade << endl;
                printf("\n\n");
                break;


            }
            default:{
                
                cout << "please enter the required number 0 1 2" << endl;
                printf("\n\n");
                break;
            }
        }

        
        cout << "**** Heap Construction *****" << endl;
        cout << "* 0. QUIT                  *" << endl;
        cout << "* 1. Build a min heap      *" << endl;
        cout << "* 2. Build a min-max heap" << endl;
        cout << "*************************************" << endl;
        cout << "Input a choice(0, 1, 2):" ;
        cin >> chose;
        cout << "bottom choice :" << chose << endl;
        sugardady.clear();
        minheap.clear();
        cout << "after clear" << endl;
    }
    cout << "outter while choice" << chose << endl;

    return 0;
}
