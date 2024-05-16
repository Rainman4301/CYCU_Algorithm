#include<iostream>
#include<cmath>
#include<vector>

#define SWAP(x,y,temp) ((temp) = (x), (x)=(y), (y)=(temp))

using namespace std;

vector<int> train;

/*
我的寫法是從資料第2比開始存取資料因為這樣在做計算時比較方便
所以資料0,1會是空值
*/

void descriminate_corresponding(int *n){
    int sup,corresponding_node,temp;
    int key;
    sup = ++(*n);
    

    //此公式可以幫助判斷當前insertation資料是在min or max heap 中 => key = 1(左樹min) key = 0 (右樹max)   
    (*n) - pow(2,floor(log2(*n))) < pow(2,floor(log2(*n)))/2? key = 1:key=0 ;



    //min
    if(key==1){

        //資料在左樹(min)對應節點 = (上層所有node數量==此曾數量一半)加上當前數量
        corresponding_node = *n + pow(2, floor(log2(*n)) - 1);



        //如果要找的對應節點超出範圍就找他的父節點
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
        //意思與上面相反
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
    //這裡的n可以改看您想找什麼點
    int n=23, fake = 0;
    vector<int> deap;
    deap.push_back(fake);
    deap.push_back(fake);
    
    descriminate_corresponding(&n);

    
    return 0;
}