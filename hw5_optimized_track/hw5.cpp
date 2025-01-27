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
#include <stack>
#include <set>





using namespace std;


typedef struct studentid{
    char putID[10];
    char getID[10];
    float weight;
}studentid;


typedef struct trunk{
    char ID[10];
    float weight;

    int label;
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
    float weight;
    bool fixed;
} con;

string format(float f, int digits) {
    ostringstream ss;
    ss.precision(digits);
    ss << f;
    return ss.str();
}



//////////////////////////////////////////////////////////////////////////////////////////////////



void Kruskal(map<string, trunk> &main_chain,string fileName,float threshold){



}


//////////////////////////////////////////////////////////////////////////////////////////////////


// namespace first_template{
//     template<class T,class B>
//     bool compare(const pair<T, B> left,const pair<T, B> right){
//         return left.second < right.second;
        
//     }
// }




bool second_cmp(const pair<string, float> left,const pair<string, float> right){
    return left.second < right.second;
    
}

bool first_cmp(const pair<string, float> a,const pair<string, float>b){
    return a.first < b.first;
}



bool compareFirst(const std::pair<string,float>& p, string value) {
    return p.first == value;
}





void dijkstra(map<string, trunk> &main_chain,string fileName,float threshold){

    map<string, float> path={};
    //為了value值可以排序用
    vector<pair<string,float>> sure_path={};


    string origin = "";
    do{

        path.clear();
        

        int tab = 0;
        // map<string, trunk> temp = main_chain;
        for (const auto& i : main_chain)
        {
            path[i.first] = float(2147483647);
            tab++;
            cout << i.first<<"  ";
            if(tab % 8 ==0){
                cout << "\n";
            }
        }
        cout << "\n";
        cout << "Input a student ID [0: exit] ";
        cin >> origin;
        cout << "\n";
        if(origin=="0"){
            break;
        }
        else if(main_chain.find(origin) == main_chain.end()){
            cout << "### the student id does not exist! ###" << endl;
            continue;
        }

        float cornerstone = 0;
        

        while(true){
            
            sort(sure_path.begin(),sure_path.end(),first_cmp);
            

            for (trunk *p = main_chain[origin].point;p!=NULL;p = p->point)
            {
                bool judge = true;
                for(auto i = sure_path.begin(); i != sure_path.end();i++){
                    if(i->first == p->ID){
                        judge = false;
                        break;
                    }

                }
                

                if(judge){

                    if(path[p->ID]+cornerstone < path[p->ID]){
                        path[p->ID] = path[p->ID] + cornerstone;
                    }
                    else{
                        path[p->ID] = p->weight;
                    }
                    
                }
                
            }

            

            auto i = min_element(path.begin(),path.end(),second_cmp);
            sort(sure_path.begin(),sure_path.end(),first_cmp);

            while(i->second != float(2147483647)){

                bool judge = true;
                for(auto k = sure_path.begin(); k != sure_path.end();k++){
                    if(k->first == i->first){
                        judge = false;
                        break;
                    }

                }


                if( judge  ){
                    cornerstone = i->second;
                    origin = i->first;
                    sure_path.push_back(make_pair(i->first,i->second));
                    break;
                }
                i++;

            }

            if(i->second == float(2147483647)){
                break;
            }

            

        }
    


        sort(sure_path.begin(),sure_path.end(),first_cmp);

        for (int i = 0; i< sure_path.size(); i++)
        {
            cout << sure_path[i].first << " " << sure_path[i].second << endl;
        }
       

    }while(true);


    

}







//////////////////////////////////////////////////////////////////////////////////////////////////

bool cmp(pair<int, vector<string>>a, pair<int, vector<string>>b)
{
    return a.second > b.second; // 根据second的值降序排序
}


void DFS(map<string, trunk> temp,stack<string> receptacle, string stID,set<vector<string>> &p_set){

    int amount = 0;
    vector<string> sub_set={};

    temp[stID].visit = true;
    receptacle.push(stID);
    while(receptacle.size()!=0){
        trunk *t = temp[receptacle.top()].point;
        amount++;
        sub_set.push_back(receptacle.top());
        receptacle.pop();
        while(t!=NULL){
            if(temp[t->ID].visit == false){
                temp[t->ID].visit = true;
                receptacle.push(t->ID);
            }
            t = t->point;
        }
    }

    // sort(sub_set.begin(),sub_set.end(),less<string>());
    sort(sub_set.begin(), sub_set.end(),greater<string>());

    p_set.insert(sub_set);
}



void connection(map<string, trunk> &connection,string fileName,float threshold){

    
    stack<string> container;
    set<vector<string>> p_set = {};
    vector< pair< int,vector<string> > > pair_set={};

    for(const auto& c :connection){
        map<string, trunk> temp = connection;
        DFS(temp,container,c.first,p_set);
        
    }



    ofstream out_fil;
    out_fil.open("pairs"+ fileName +"_" + format(threshold,4) +".cc",fstream::out);
    if(out_fil.is_open()){
        out_fil << "<<< There are "<< p_set.size() <<" connected components in total. >>>\n";

        int out_len = 0;

        for(const auto& vec : p_set){
            pair_set.push_back({vec.size(), vec});
        }
        sort(pair_set.begin(), pair_set.end(), cmp);

        for(const auto& vec : pair_set){

            int in_len = 1;
            out_fil<<"{"<<setw(2)<<++out_len<<"} "<<"Connected Component: size = "<<vec.second.size()<<"\n";
            for (auto r = vec.second.rbegin(); r != vec.second.rend();r++)
            {
                out_fil <<"\t"<<"("<<setw(3)<<in_len++<<")"<<setw(9)<<*r;
            }
            out_fil << "\n";
        }
        
    }
    out_fil.close();
}




//////////////////////////////////////////////////////////////////////////////////////////////////





void DFS_influence_out(map<string,trunk> &inf_files,string filename,float threshold){

    fstream out_inf;
    out_inf.open("pairs"+ filename +"_" + format(threshold,4) +".adj",fstream::out);

    

    int node_num = 0;
    if(out_inf.is_open()){

        out_inf << "<<< There are " << inf_files.size() << " IDs in total. >>>\n";

        int i = 1;
        for (const auto& m : inf_files)
        {
            out_inf << "["<<setw(3) << i  << "] " << string(m.first) << ":\n";

            int k =1;
            for ( trunk *j = m.second.point   ; j != NULL ; j = j->point , k++)
            {
                out_inf << "\t(" <<setw(2)<< k << ") "<<string(j->ID)<<", "<<setw(5)<<j->weight;
                node_num++;
                if(k%10==0){
                    out_inf << "\n";
                }
            }
            out_inf << "\n";
            i++;
        }
        

        out_inf << "<<< There are " << node_num <<" nodes in total. >>>\n";
    }



    out_inf.close();
}   








map<string,trunk> influence(map<string,trunk> &main_chain,string filename,float &threshold ){


    
    

    map<string, trunk> influence = main_chain;

    for( auto &t : influence){

        trunk *pre = &t.second , *post = t.second.point;
        
        
        
        while(post != NULL ){

            if(post->weight > threshold){
                
                pre->point = post->point;
                delete post;
                post = pre->point;
                continue;

            }

            pre = post;
            post = post->point;

        }
    }

    vector <string> kill = {};

    for(const auto& em :influence){
        if(em.second.point==NULL){
            kill.push_back(em.first);
        }
    }
    for (int i = 0;i<kill.size();i++)
    {
        influence.erase(kill[i]);
    }
    kill.clear();


    DFS_influence_out(influence,filename,threshold);
    return influence;
}










map<string,trunk> adjacencylists(vector<studentid> &data  , string filename,float &threshold){

    map< string ,trunk> main_chain = {};
    trunk temp = {};
    int position = 0;


    for(int i=0; i<data.size(); i++)
    {   

        
        
        if(main_chain.empty()){
            strcpy(temp.ID, data[i].putID);
            main_chain.insert(pair<string,trunk>(string(data[i].putID), temp));
            strcpy(temp.ID, data[i].getID);
            main_chain.insert(pair<string,trunk>(string(data[i].getID),temp));
        }
        if(  main_chain.find(string(data[i].putID)) == main_chain.end() ){
            strcpy(temp.ID, data[i].putID);
            main_chain.insert(pair<string,trunk>(string(data[i].putID),temp) );

        }
        if(main_chain.find(string(data[i].getID)) == main_chain.end() ){
            strcpy(temp.ID, data[i].getID);
            main_chain.insert(pair<string,trunk>(string(data[i].getID),temp) );

        }







        trunk *put_space = new trunk();
        put_space->point == NULL;
        strcpy(put_space->ID, data[i].getID);
        put_space->weight = data[i].weight;


        if(main_chain[ string(data[i].putID) ].point==NULL){

            main_chain[ string(data[i].putID) ].point = put_space;
        }
        else{

            for (trunk* j = main_chain[ string(data[i].putID) ].point ; j != NULL; j = j->point)
            {

                if(string(put_space->ID) < string(main_chain[string(data[i].putID)].point->ID)){
                    put_space->point = main_chain[string(data[i].putID)].point;
                    main_chain[string(data[i].putID)].point = put_space;
                    break;
                }
                else if(j->point == NULL && string(put_space->ID) > string(j->ID)){
                    j->point = put_space;
                    break;
                }
                else if(string(put_space->ID) > string(j->ID) && string(put_space->ID) < string(j->point->ID)){

                    put_space->point = j->point;
                    j->point = put_space;
                    break;
                }
                
            }
        }

        trunk *get_space = new trunk();
        get_space->point == NULL;
        strcpy(get_space->ID, data[i].putID);
        get_space->weight = data[i].weight;

        if(main_chain[ string(data[i].getID) ].point==NULL){

            main_chain[ string(data[i].getID) ].point = get_space;
        }
        else{

            for (trunk* j = main_chain[ string(data[i].getID) ].point ; j != NULL; j = j->point)
            {

                if(string(get_space->ID) < string(main_chain[string(data[i].getID)].point->ID)){
                    get_space->point = main_chain[string(data[i].getID)].point;
                    main_chain[string(data[i].getID)].point = get_space;
                    break;
                }
                else if(j->point == NULL && string(get_space->ID) > string(j->ID)){
                    j->point = get_space;
                    break;
                }
                else if(string(get_space->ID) > string(j->ID) && string(get_space->ID) < string(j->point->ID)){

                    get_space->point = j->point;
                    j->point = get_space;
                    break;
                }
                
            }
        }


        
    }



    main_chain = influence(main_chain , filename , threshold);
    return main_chain;
}



////////////////////////////////////////////////////////////////////////////////









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

    string fileName = "" ;
    int choice = 0;
    int data_count = 0;
    vector<studentid> overall_data;
    map<string,trunk> main_train = {};









    cout<<"**** Graph data manipulation ****"<<endl;
    cout<<"* 0. QUIT              *"<<endl;
    cout<<"* 1. Create adjacency lists      *"<<endl;
    cout<<"* 2. Build connected components  *"<<endl;
    cout<<"* 3. Find shortest paths by Dijkstra   *"<<endl;
    cout<<"* 4. Generate minimum spanning trees *"<<endl;
    cout<<"*************************************"<<endl;
    cout<<"Input a choice(0, 1, 2, 3, 4): ";
    cin >> choice;

    float threshold = 0;

    


    while(choice != 0){

        if((main_train.size() != 0 || choice != 2 || choice != 3 || choice != 4 ) && (choice != 2 ) && (choice != 3) && (choice != 4)  ){


            do{
                cout << "Input a real number in (0,1]:" ;
                cin >> threshold;
                if(threshold>=0 && threshold<1){
                    break;
                }
            } while (true);

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

                main_train = adjacencylists(overall_data , fileName,threshold);
                break;

            case 2:

                if(main_train.size()!=0){
                    connection(main_train , fileName,threshold);
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###\n" << endl;
                break;

            case 3: 

                if(main_train.size()!=0){
                    cout << "\n";
                    dijkstra(main_train,fileName,threshold);
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###\n" << endl;

                break;

            case 4:
                if(main_train.size()!=0){
                    
                    break;
                }
                cout << "### There is no graph and choose 1 first. ###\n" << endl;
                break;

            default:
                cout<< "Please enter an appropriate choice\n\n"<<endl;
                break;
        }

        
        

        cout<<"**** Graph data manipulation ****"<<endl;
        cout<<"* 0. QUIT              *"<<endl;
        cout<<"* 1. Create adjacency lists      *"<<endl;
        cout<<"* 2. Build connected components  *"<<endl;
        cout<<"* 3. Find shortest paths by Dijkstra   *"<<endl;
        cout<<"* 4. Generate minimum spanning trees *"<<endl;
        cout<<"*************************************"<<endl;
        cout<<"Input a choice(0, 1, 2, 3, 4): ";
        cin >> choice;


    }



    system("pause");
    return 0;
}
