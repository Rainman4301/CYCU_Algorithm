
// 10722217���t��

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>


using namespace std;

typedef struct node
{
    // �s����n�����
    int data;
    string majname;
    string nighday;
    string level;
    // �h�~���~�ͤH��
    int ngrade;

} nodetype;

typedef nodetype *bitree_ptr;

vector<nodetype> minheap;
int n = 0;

















// min_max_heap�R���̤p��

void min_max_delete(int k)
{

    for (int i = 1; i <= k; i++)
    {



        int parent = 1, child = 2;

        nodetype temp;
        temp = minheap[1]; // ��X�̤p��

        cout <<"Top     "<<i<<':'<<"    ["<<temp.data<<"]  "<< temp.majname <<", "<<temp.nighday<<", "<<temp.level<<", "<<temp.ngrade<< endl;

        swap(minheap[1], minheap[n]);
        int h = n;
        minheap[n].ngrade = INT_MAX;
        n--;
        

        if (n >= child)
        {


            if (minheap[child].ngrade > minheap[child + 1].ngrade)
            {
                child++;
            }

            if (minheap[parent].ngrade > minheap[child].ngrade)
            {
                swap(minheap[parent].ngrade, minheap[child].ngrade);
            }

            child = 4;
            while (n >= child)
            {

                vector<int> v{minheap[(4 * parent )>n ? h : 4*parent].ngrade, minheap[(4 * parent + 1)>n ? h : 4*parent+1].ngrade, minheap[(4 * parent + 2)>n ? h : 4*parent+2].ngrade, minheap[(4 * parent + 3)>n ? h : 4*parent+3].ngrade};


                // vector<int>::iterator itr = find(v.begin(), v.end(), (*min_element(v.begin(), v.end())));
                int minValue = *min_element(v.begin(),v.end());
                int minPosition = min_element(v.begin(),v.end()) - v.begin();
                child = child + minPosition;
                

                // child = child + distance(v.begin(), itr);

                



                if (minheap[parent].ngrade > minheap[child].ngrade)
                {
                    swap(minheap[parent], minheap[child]);

                }

                else
                {
                    parent = child;
                    child = 2 * parent;
                    if (n < 4 * parent && n >= child)
                    {
                        if (minheap[child].ngrade > minheap[child + 1].ngrade)
                        {
                            child++;
                        }

                        if (minheap[parent].ngrade > minheap[child].ngrade)
                        {
                            swap(minheap[parent], minheap[child]);
                        }
                    }
                    break;
                }

                parent = child;
                child = 4 * parent;
            }



            child = 2 * parent;
            if(n>=child){
                if(minheap[child].ngrade > minheap[child + 1].ngrade){
                    child++;
                }

                if(minheap[parent].ngrade > minheap[child].ngrade){
                    swap(minheap[parent], minheap[child]);
                }
                
            }


        }
    }
}

// min_max_heap ���ظm

void insert_min_max_heap(nodetype element)
{

    int i = ++n;
    minheap.push_back(element);

    if (i > 3)
    {

        // �ΨӧP�_�b���h
        int min_max = int(floor(log2(i))) % 2;

        // min�h
        if (min_max == 0)
        {
            if (minheap[i].ngrade > minheap[i / 2].ngrade)
            {
                swap(minheap[i], minheap[i / 2]);
                // �qmin�h��max�h
                i = i / 2;
                while ((i / 2) > 3)
                {
                    // �p�Gnode����>grandparent����
                    if (minheap[i].ngrade > minheap[i / 4].ngrade)
                    {
                        swap(minheap[i], minheap[i / 4]);
                    }
                    i = i / 4;
                }
            }
            else
            {
                while ((i / 2) > 1)
                {
                    if (minheap[i].ngrade < minheap[i / 4].ngrade)
                    {
                        swap(minheap[i], minheap[i / 4]);
                    }
                    i = i / 4;
                }
            }
        }
        // max�h
        else
        {
            if (minheap[i].ngrade < minheap[i / 2].ngrade)
            {
                swap(minheap[i], minheap[i / 2]);
                // �qmax�h��min�h
                i = i / 2;
                while ((i / 2) > 1)
                {
                    // �p�Gnode����<grandparent����
                    if (minheap[i].ngrade < minheap[i / 4].ngrade)
                    {
                        swap(minheap[i], minheap[i / 4]);
                    }
                    i = i / 4;
                }
            }
            else
            {
                while ((i / 2) > 3)
                {
                    if (minheap[i].ngrade > minheap[i / 4].ngrade)
                    {
                        swap(minheap[i], minheap[i / 4]);
                    }
                    i = i / 4;
                }
            }
        }
    }

    // �e3�h
    else
    {
        if (i == 1)
        {
        }
        else
        {
            if (element.ngrade < minheap[i / 2].ngrade)
            {
                swap(minheap[i], minheap[i / 2]);
            }
        }
    }
}

// �R���̤p��
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

// min_heap���ظm
void insert_minimum_heap(nodetype element)
{

    int i = ++n;
    minheap.push_back(element);

    // �p�G���O�Ĥ@�ӭ� && �s�J��<���`�I
    while ((i > 1) && element.ngrade < minheap[i / 2].ngrade)
    {

        // minheap[i] = minheap[i / 2];
        swap(minheap[i], minheap[i / 2]);
        i = i / 2;
    }
}

void GetStringFromCSV(string line, int nIdx, string &str)
{
    int nSPos = 0;
    for (int i = 0; i < nIdx - 1; ++i)
    {
        nSPos = line.find('\t', nSPos); 
        ++nSPos;                        
    }

    int nEPos = line.find('\t', nSPos); 
    if (nEPos != string::npos)          // ���TAB�r�Ū�
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

    int chose;
    vector<nodetype> sugardady;

    cout << "**** Heap Construction *****" << endl;
    cout << "* 0. QUIT                  *" << endl;
    cout << "* 1. Build a min heap      *" << endl;
    cout << "* 2. Build a min-max heap" << endl;
    cout << "* 3: Top-K minimums from min-max heap *" << "    (�Х����� �{��2)" <<endl;
    cout << "*************************************" << endl;
    cout << "Input a choice(0, 1, 2):";
    cin >> chose;



    

    while (chose != 0)
    {
        bool button = NULL;
        n = 0;
        // vector�q1�}�l�s����Ƥ�K�p��
        // 0225--update
        nodetype fake;
        minheap.push_back(fake);

        // ��ƪ�Ū��
        string file_number = "";
        cout << "Input a file number ([0] Quit): ";
        cin >> file_number;

        if (stoi(file_number) == 0)
        {
            break;
        }

        file_number = "input" + file_number + ".txt";
        std::ifstream ifs;

        ifs.open(file_number); // ifs.open ??

        if (!ifs.is_open())
        {
            cout << "Failed to open file.\n";
            return 1; // EXIT_FAILURE
        }

        string tp;

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
                if (g == 2 || g == 4)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.majname.append(sTarget);
                }
                else if (g == 5)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.nighday.append(sTarget);
                }
                else if (g == 6)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.level.append(sTarget);
                }
                else if (g == 9)
                {
                    GetStringFromCSV(s, g, sTarget);
                    temp.ngrade = stoi(sTarget);
                }
                else
                {
                    temp.data = ++flow;
                    sugardady.push_back(temp);
                }
            }
        }


        

        
        int l = 0,k;
        switch (chose)
        {

        case (1):

            for (int k = 0; k < sugardady.size(); k++)
            {
                insert_minimum_heap(sugardady[k]);
            }

            cout << "<min heap>" << endl;
            // �]���ڱq�Ĥ@���}�l�s��ƩҥHroot�|�b�Ĥ@��
            cout << "root: " << '[' << minheap[1].data << ']' << minheap[1].ngrade << endl;
            cout << "bottom: " << '[' << minheap[n].data << ']' << minheap[n].ngrade << endl;

            // ���F���̥���`�I�����

            for (int j = pow(2, l); j <= n; l++, j = pow(2, l))
            {
            }
            l--;

            cout << "leftmost bottom: " << '[' << minheap[pow(2, l)].data << ']' << minheap[pow(2, l)].ngrade << endl;

            printf("\n\n");
            break;

        case (2):

            for (int i = 0; i < sugardady.size(); i++)
            {
                insert_min_max_heap(sugardady[i]);
            }

            cout << "<min heap>" << endl;

            // �]���ڱq�Ĥ@���}�l�s��ƩҥHroot�|�b�Ĥ@��
            cout << "root: " << '[' << minheap[1].data << ']' << minheap[1].ngrade << endl;
            cout << "bottom: " << '[' << minheap[n].data << ']' << minheap[n].ngrade << endl;

            // ���F���̥���`�I�����

            for (int j = pow(2, l); j <= n; l++, j = pow(2, l))
            {
            }
            l--;

            cout << "leftmost bottom: " << '[' << minheap[pow(2, l)].data << ']' << minheap[pow(2, l)].ngrade << endl;
            printf("\n\n");

            button = true;

            break;


        default:

            cout << "please enter the required number 0 1 2" << endl;
            printf("\n\n");
            break;
        }



        int t = 0;
        int contain = sugardady.size();
        while(button){
            int judge;
            
            cout << "\n\n*************************************" << endl;
            cout<< "* 0: eject the function" << endl;
            cout << "* 3: Top-K minimums from min-max heap *" << endl;
            cout << "*************************************" << endl;
            cout << "Input a choice(0,3):";
            cin >> judge;
            
            if(judge != 3){
                button = NULL;
                break;
            }
            else{
                
                
                cout <<" Enter the value of K in[1,"<< contain-t<<"] : ";
                contain = contain - t;
                cin >> t;
                cout << endl;

                if(t>contain){
                    t = contain;
                }
                min_max_delete(t);

                if(contain-t ==0){
                    cout<<"\nData Empty\n"<<endl;
                    break;
                }
                
            }

        }




        cout << "**** Heap Construction *****" << endl;
        cout << "* 0. QUIT                  *" << endl;
        cout << "* 1. Build a min heap      *" << endl;
        cout << "* 2. Build a min-max heap" << endl;
        cout << "*************************************" << endl;
        cout << "Input a choice(0, 1, 2):";
        cin >> chose;
        // ��ƲM�Ŭ��F�U���{���B��
        sugardady.clear();
        minheap.clear();
        tp.clear();
        

        


    }

    
    
    cout << "programming ending" << endl;

    return 0;
}
