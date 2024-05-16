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
#include <map>




using namespace std;


typedef struct studentid{
    char putID[10];
    char getID[10];
    float weight;
}studentid;


typedef struct trunk{
    char ID[10];
    float weight;

    
    // calculate how many connections it has
    int index = 0;
    // whether is it traversed
    bool visit = false;
    trunk *point;

   




    bool operator <(const trunk &other)const   //升序排序
    {
        return string(ID) > string(other.ID);
    }

    //或者
    bool operator >(const trunk &r)const   //降序排序
    {
        return string(ID) > string(r.ID);
    }

}trunk;

typedef struct con{


    string ID;
    int index;
    
    /* Sort in the usual nmap-services order. */
    bool operator<(const con& other) const {

        if (this->ID < other.ID)
            return true;
        else if (this->ID > other.ID)
            return false;
        else
            return this->ID < other.ID;  
    }
    
}con;




// struct MyCompare{
// public:
// 	bool operator()(const pair<con,vector<string>> &p1, const  pair<con,vector<string>> &p2) const{
// 		return p1.first.index < p2.first.index;
// 	}
// };










void influence(map<string,trunk> &main_chain,string filename ){
    
}














void output_con_file(map<con,vector<string>> task, string filename){

    fstream out_con_file;
    out_con_file.open("pairs" + filename + ".cnt", fstream::out);

    if(out_con_file.is_open()){

        out_con_file<<"<<< There are "<< task.size() <<" IDs in total. >>>"<<"\n";

        int i = 1;
        for(const auto& p:task){

            out_con_file << "["<<setw(3) << i++  << "] " << string(p.first.ID) <<"("<<p.first.index<<")"<< ":\n";
            for (int i = 0;i<p.second.size();i++)
            {
                out_con_file<<"\t"<<"("<<setw(2)<<i+1<<") "<<p.second[i];
                if(i+1%10==0){
                    out_con_file<<"\n";
                }
            }
            out_con_file<<"\n";
        }
    }
}








string dequeue(vector<trunk> &Queue )
{
    
    string key = Queue[0].ID;
    Queue.erase(Queue.begin());
    return key;
}

void enqueue( vector<trunk> &Queue , trunk &trunk ){

    
    Queue.push_back(trunk);
}



void connection_count(map<string,trunk> &main_chain  , string filename ){

    
    map< con, vector<string> > task={};
    



    for( auto &p: main_chain){


        // for new structure
        vector<string> list={};
        con nod={};


        //for queue and temp_structure
        map<string, trunk> connection = main_chain;
        vector<trunk> queue = {};

        connection[p.second.ID].visit = true;
        enqueue(queue,p.second);
        nod.ID = p.second.ID;
        


        while(queue.size() != 0){

            trunk *pointer = connection[dequeue(queue)].point;

            while(pointer != NULL)
            {
                
                if (connection[pointer->ID].visit == false )
                {

                    nod.index++;
                    list.push_back(string(pointer->ID));

                    connection[pointer->ID].visit = true;
                    enqueue(queue,*pointer);

                }
                pointer = pointer->point;
            }
        }

        sort(list.begin(), list.end(), less<string>());

        task.insert(pair<con, vector<string> >(nod, list));
        list.clear();
        connection.clear();
        queue.clear();

        
        

    }


    // for(const auto& pair: task){

    //     cout << pair.first.index << "\t" << pair.first.ID << endl;
    //     for (int i = 0; i< pair.second.size(); i++)
    //     {
    //         cout << pair.second[i]<<" ";
    //         if(i == 9){
    //             cout<<endl;
    //         }
    //     }
    //     cout << endl;
    // }


    output_con_file(task,filename);

    task.clear();
}









void output_adj_file(map<string,trunk> &main_chain , int data_count , string filename){

    fstream out_adj_file;
    out_adj_file.open("pairs" + filename + ".adj", fstream::out);

    if(out_adj_file.is_open()){

        out_adj_file << "<<< There are " << main_chain.size() << " IDs in total. >>>\n";

        int i = 1;
        for (const auto& m : main_chain)
        {
            out_adj_file << "["<<setw(3) << i  << "] " << string(m.second.ID) << ":\n";

            int k =1;
            for ( trunk *j = m.second.point   ; j != NULL ; j = j->point , k++)
            {
                out_adj_file << "\t(" <<setw(2)<< k << ") "<<string(j->ID)<<", "<<setw(5)<<j->weight;
                
                if(k%10==0){
                    out_adj_file << "\n";
                }
            }
            out_adj_file << "\n";
            i++;
        }

        out_adj_file << "<<< There are " << data_count << " nodes in total. >>>";
    }

    out_adj_file.close();
}








map<string,trunk> adjacencylists(vector<studentid> &data , int data_count , string filename){

    map< string ,trunk> main_chain = {};
    trunk temp = {};
    int position = 0;


    for(int i=0; i<data.size(); i++)
    {   

        strcpy(temp.ID, data[i].putID);
        
        if(main_chain.empty()){
            main_chain.insert(pair<string,trunk>(string(data[i].putID), temp));
            strcpy(temp.ID, data[i].getID);
            main_chain.insert(pair<string,trunk>(string(data[i].getID),temp));
        }



        if(  main_chain.find(string(data[i].putID)) == main_chain.end() ){

            main_chain.insert(pair<string,trunk>(string(data[i].putID),temp) );

            if(main_chain.find(string(data[i].getID)) == main_chain.end() ){

                strcpy(temp.ID, data[i].getID);
                main_chain.insert(pair<string,trunk>(string(data[i].getID),temp) );

            }
        }




        trunk *space = new trunk();
        space->point == NULL;
        strcpy(space->ID, data[i].getID);
        space->weight = data[i].weight;


        if(main_chain[ string(data[i].putID) ].point==NULL){

            main_chain[ string(data[i].putID) ].point = space;
        }
        else{

            for (trunk* j = main_chain[ string(data[i].putID) ].point ; j != NULL; j = j->point)
            {

                if(string(space->ID) < string(main_chain[string(data[i].putID)].point->ID)){
                    space->point = main_chain[string(data[i].putID)].point;
                    main_chain[string(data[i].putID)].point = space;
                    break;
                }
                else if(j->point == NULL && string(space->ID) > string(j->ID)){
                    j->point = space;
                    break;
                }
                else if(string(space->ID) > string(j->ID) && string(space->ID) < string(j->point->ID)){

                    space->point = j->point;
                    j->point = space;
                    break;
                }
                
            }
        }
        
    }

    // sort(main_chain.begin(), main_chain.end() , less<trunk>());

    // for (int i = 0; i < main_chain.size(); i++)
    // {
    //     cout<< main_chain[i].ID<<endl;
    // }

    output_adj_file(main_chain , data_count , filename);
    return main_chain;
}






int readBinary(string filename, vector<studentid> &receptacle){

    ifstream binfile ;
    fstream txt_outfile;

    studentid contain;
    int stno = 0;


    binfile.open(("pairs"+filename+".bin"),fstream::in | fstream::binary);
    txt_outfile.open(("input"+filename+".txt"),fstream::out);

    if(binfile.is_open()){

        

        binfile.seekg(0, binfile.end);
        stno = binfile.tellg()/sizeof(contain);
        binfile.seekg(0,binfile.beg);


        

        for (int i = 0 ; i < stno ; i++)
        {
            binfile.read((char*)&contain, sizeof(contain));

            txt_outfile << contain.putID << "\t";
            txt_outfile << contain.getID << "\t";
            txt_outfile << contain.weight <<"\n";


            receptacle.push_back(contain);
        
        }

        
    }


    binfile.close();
    txt_outfile.close();

    return stno;
}












int main(){

    string fileName = "";
    int choice = 0;
    int data_count = 0;
    vector<studentid> overall_data;
    map<string,trunk> main_train = {};


    cout<<"**** Graph data manipulation ****"<<endl;
    cout<<"* 0. QUIT              *"<<endl;
    cout<<"* 1. Build adjacency lists      *"<<endl;
    cout<<"* 2. Compute connection counts  *"<<endl;
    cout<<"* 3. Estimate influence values   *"<<endl;
    cout<<"* 4. Probability-based influence *"<<endl;
    cout<<"*************************************"<<endl;
    cout<<"Input a choice(0, 1, 2, 3, 4): ";
    cin >> choice;



    while(choice != 0){

        if((main_train.size() != 0 || choice != 2 || choice != 3 || choice != 4 ) && (choice != 2 ) && (choice != 3) && (choice != 4)  ){


            do{


                for(const auto& m : main_train){
                    trunk *forth = NULL , *back=NULL;

                    if(m.second.point != NULL){
                        forth = m.second.point;
                        for (;forth!=NULL;)
                        {
                            back = forth->point;
                            delete forth;
                            forth = back;
                        }
                    }
                    
                }
                main_train.clear();

                fstream binFile ;

                cout << "input a file number: ";
                cin >> fileName;
                cin.ignore();

                if(!fileName.compare("0")){
                    return false;
                }


                
                binFile.open(("pairs"+fileName+".bin").c_str(),fstream::in | fstream::binary);      // open a file
                if(binFile.is_open() ){
                    binFile.close();
                    data_count = readBinary(fileName,overall_data);
                    break;
                }

                cout<<"There is no file called \"pairs" + fileName+".bin\""<<endl;

            } while (true);

        }
        

        switch(choice){

            case 1:

                main_train = adjacencylists(overall_data,data_count , fileName);
                break;

            case 2:
                if(main_train.size()!=0){
                    connection_count(main_train , fileName);
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###" << endl;
                break;

            case 3: 


                if(main_train.size()!=0){
                    
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###" << endl;

                break;

            case 4:
                if(main_train.size()!=0){
                    
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###" << endl;
                break;

            default:
                cout<< "Please enter an appropriate choice\n\n"<<endl;
                break;
        }

        
        fileName.clear();

        cout<<"**** Graph data manipulation ****"<<endl;
        cout<<"* 0. QUIT              *"<<endl;
        cout<<"* 1. Build adjacency lists      *"<<endl;
        cout<<"* 2. Compute connection counts  *"<<endl;
        cout<<"* 3. Estimate influence values   *"<<endl;
        cout<<"* 4. Probability-based influence *"<<endl;
        cout<<"*************************************"<<endl;
        cout<<"Input a choice(0, 1, 2, 3, 4): ";
        cin >> choice;


    }



    system("pause");
    return 0;
}
