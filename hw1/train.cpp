#include<iostream>
#include<cmath>
#include<vector>

#define SWAP(x,y,temp) ((temp) = (x), (x)=(y), (y)=(temp))

using namespace std;

vector<int> train;

/*
�ڪ��g�k�O�q��Ʋ�2��}�l�s����Ʀ]���o�˦b���p��ɤ����K
�ҥH���0,1�|�O�ŭ�
*/

void descriminate_corresponding(int *n){
    int sup,corresponding_node,temp;
    int key;
    sup = ++(*n);
    

    //�������i�H���U�P�_��einsertation��ƬO�bmin or max heap �� => key = 1(����min) key = 0 (�k��max)   
    (*n) - pow(2,floor(log2(*n))) < pow(2,floor(log2(*n)))/2? key = 1:key=0 ;



    //min
    if(key==1){

        //��Ʀb����(min)�����`�I = (�W�h�Ҧ�node�ƶq==�����ƶq�@�b)�[�W��e�ƶq
        corresponding_node = *n + pow(2, floor(log2(*n)) - 1);



        //�p�G�n�䪺�����`�I�W�X�d��N��L�����`�I
        if(corresponding_node > sup){
            corresponding_node = corresponding_node/2;
        }



        if(train[sup] > train[corresponding_node]){

            SWAP(train[sup], train[corresponding_node], temp);
            while(corresponding_node>3 &&train[corresponding_node]>train[corresponding_node/2]){
                SWAP(train[corresponding_node],train[corresponding_node/2],temp);
                corresponding_node = corresponding_node/2;
            }
        }   

        else {
            while(sup>2 &&train[sup]<train[sup/2]){
                SWAP(train[sup],train[sup/2],temp);
                sup = sup/2;
            }
        }

    }



    //max
    else{
        //�N��P�W���ۤ�
        corresponding_node = sup - pow(2,floor(log2(*n)) - 1);

        if(train[sup] < train[corresponding_node]){

            SWAP(train[sup], train[corresponding_node], temp);
            while(corresponding_node>2 &&train[corresponding_node]<train[corresponding_node/2]){
                SWAP(train[corresponding_node],train[corresponding_node/2],temp);
                corresponding_node = corresponding_node/2;
            }
        }   

        else {
            while(sup>3 &&train[sup]>train[sup/2]){
                SWAP(train[sup],train[sup/2],temp);
                sup = sup/2;
            }
        }
        
    }


    cout<<corresponding_node<<endl;
}



int main(){
    //�o�̪�n�i�H��ݱz�Q�䤰���I
    int n=23, fake = 0;
    vector<int> deap;
    deap.push_back(fake);
    deap.push_back(fake);
    
    descriminate_corresponding(&n);

    
    return 0;
}