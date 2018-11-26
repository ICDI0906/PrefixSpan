#include <iostream>
#include <stdio.h>
#include <fstream>
#include <regex>
#include <time.h>
#include <cctype>
#include <set>
#include <map>
using namespace std;
int min_sup = 2;
int number_of_element = 6;
fstream _file;
bool debug = true;
bool check_data = false;
void checkOrigin(vector<vector<int>> origin){
    for (int i = 0; i < origin.size(); i++) {
        for (int j = 0; j < origin[i].size(); j++) {
            cout << origin[i][j] << " ";
        }
        cout << endl;
    }
}
void  deleteOriginData(vector<vector<int>> &origin,set<int>elems){

    for(int i = 0;i < origin.size();){
        for(int j = 0;j < origin[i].size();){
            int elem = origin[i][j];
            if(elem != -1 && elems.find(elem) == elems.end()){ // delete elements not in elems
                if(elem > number_of_element){
                    if(origin[i][j + 1] != -1){ // 向后传递值!!!
                        origin[i][j + 1] += number_of_element;
                    }
                }
                origin[i].erase(origin[i].begin() + j);
                // 判断前后都是-1，或者j=0后为-1
                if(j == 0){ //2 -1 ......
                    if(origin[i][j] == -1){
                        origin[i].erase(origin[i].begin() + j);
                    }
                }
                else{//判断j-1 和 j 是否都为-1，是的话去掉一个
                    if(origin[i][j - 1] == -1 && origin[i][j] == -1){
                        origin[i].erase(origin[i].begin() + j); // delete element_j as well
                    }
                }
            }
            else{
                j++;
            }
        }
        if((int)origin[i].size() == 0){ // no element in origin data[i]
            origin.erase(origin.begin() + i);
        }else{
            i++;
        }

    }
    if(check_data){
        cout<<"element in set elems"<<endl;
        for(auto tmp:elems){
            cout << tmp<<" ";
        }
        cout<<endl;
        checkOrigin(origin);
    }
}
void output_answer(vector<int> prefix,set<int> elems){
    int len = prefix.size();   //doesn't use prefix.size() - 1 this is a huge number
    for(auto elem : elems){
        bool flag = 0;
        if (elem > number_of_element) { //为中间结果
            flag = 1;
        }
//        cout << len-1 << endl;
        for(int  i = 0; i < len - 1 ;i++){
            cout << prefix[i]<<" ";
        }
        if(flag){
            cout<< elem % number_of_element<< " " << -1<<endl;
        }
        else{
            if (prefix.size() > 0) {
                cout << prefix[prefix.size() - 1] << " "<<elem <<" " << -1 << endl;
            }
            else{
                cout << " "<<elem <<" " << -1 << endl;
            }
        }
    }
}
vector<vector<int>> rebuildOriginData(vector<vector<int>> origin,vector<int> prefix,bool flag){

    vector<vector<int>> result;
    int elem = prefix[prefix.size()-2],elem_pre = -1;
    if(flag){
        elem = elem + number_of_element;
        elem_pre = prefix[prefix.size() - 3]; //前面一个元素是什么
    }
    for(int i =0 ;i<origin.size();i++){
        vector<int> tmp;
        // 根据前缀来选择最先出现的节点
        bool bk = 0;
        for(int j = 0; j < origin[i].size(); j++){
            if(flag && origin[i][j] == elem){ // _b
                if (origin[i][j + 1] != -1) {
                    tmp.push_back(origin[i][j + 1] + number_of_element);
                }
                for (int k = j + 2; k < origin[i].size(); k++) {
                    tmp.push_back(origin[i][k]);
                }
                break;
            }
            else if(flag &&  origin[i][j] + number_of_element == elem){
                int tmp_j = j - 1;
                while (tmp_j >= 0 && origin[i][tmp_j] != -1) { //
//                        cout<< origin[i][tmp_j] << " ---- "<< elem_pre <<endl;
                    if (origin[i][tmp_j] == elem_pre) {

//                            cout<< elem + number_of_element<< " ------ "<<endl;
                        if (origin[i][j + 1] != -1) {
                            tmp.push_back(origin[i][j + 1] + number_of_element);
                        }
                        for (int k = j + 2; k < origin[i].size(); k++) {
                            tmp.push_back(origin[i][k]);
                        }
                        bk = 1;
                    }
                    tmp_j--;
                }
                if(bk ==1)
                    break;
            }
            else if(!flag && origin[i][j] == elem){
                if (origin[i][j + 1] != -1) {
                    tmp.push_back(origin[i][j + 1] + number_of_element);
                }
                for (int k = j + 2; k < origin[i].size(); k++) {
                    tmp.push_back(origin[i][k]);
                }
                break;
            }
        }
        if(!tmp.empty()) {
            result.push_back(tmp);
        }
    }
//    if(false && prefix.size() == 2  && prefix[prefix.size() - 2] ==0){//  && prefix[prefix.size() - 2] == 5){
//        cout<<"prefix "<<endl;
//        for(auto pre:prefix){
//            cout<<pre<<" ";
//        }
//        cout<< endl<< "prefix.size() "<<prefix.size()<<endl;
//        cout<< "origin data"<<endl;
//        for(int i=0;i<origin.size() ;i++){
//            for(int j=0;j<origin[i].size();j++){
//                cout<< origin[i][j]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<"result data : "<<endl;
//        for(int i=0 ;i <result.size() ;i++){
//            for(int j =0 ;j< result[i].size();j++){
//                cout<< result[i][j]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<< "size : " << result.size() << endl;
//    }
    return result;
}
void startPrefixSpan(vector<vector<int>> origin,vector<int> prefix){
    set<int> elements; // include elements
    //traversal
    map<int,int>elements_count;
     //get last element
    for(int origin_i = 0; origin_i < origin.size(); origin_i++){
//        int * vis = new int[number_of_element + 1]{};
        vector<int>vis(2 * number_of_element + 1);
        for(int origin_i_j = 0; origin_i_j < origin[origin_i].size();origin_i_j++){
            int elem = origin[origin_i][origin_i_j];
            if(elem != -1) {
                if(elements.find(elem) == elements.end()) {
                    elements.insert(elem);
                    elements_count[elem] = 1;
                    vis[elem] = 1;
                    if(elem < number_of_element && prefix.size() > 0){ // 有没有可能转换为_elem

                        int last_element = prefix[prefix.size() - 2];
                        int tmp_i = origin_i_j - 1;
                        while(tmp_i>= 0 && origin[origin_i][tmp_i] != -1){
                           // cout<< origin[origin_i][tmp_i] << " ---- "<< last_element<<endl;
                            if(origin[origin_i][tmp_i] == last_element || origin[origin_i][tmp_i] > number_of_element){ // 判断条件改一下 !!
//                                cout<< elem + number_of_element<< " ------ row :"<<origin_i<<endl;
                                if(elements.find(elem + number_of_element) == elements.end()){ // not fond
                                    elements.insert(elem + number_of_element);
                                    elements_count[elem + number_of_element] = 1;
                                }else{
                                    elements_count[elem + number_of_element] += 1;
                                }
                                vis[elem + number_of_element]= 1;
//                                break;
                            }
                            tmp_i --;
                        }
                    }
                }
                else{
                    if(vis[elem] == 0) {
                        elements_count[elem] += 1;
                        vis[elem] = 1;
                    }
                }
            }

        }
//        delete []vis;
    }

    map<int,int>::iterator mp_iterator = elements_count.begin();
    for(;mp_iterator != elements_count.end(); mp_iterator++){
        if(mp_iterator->second < min_sup){      //delete the element which count < min_sup
            if (elements.find(mp_iterator->first) != elements.end())
                elements.erase(mp_iterator->first);
        }
    }
    int size = (int)elements.size();

    if (size == 0){
        return;
    }
    deleteOriginData(origin,elements);
    output_answer(prefix,elements);
    bool flag = false;
    for(auto elem : elements){
        flag = false; // 该元素是不是大于的元素
        if (elem > number_of_element){
            flag = true;
            prefix.pop_back();
            elem = elem % number_of_element;
        }
        prefix.push_back(elem);
        prefix.push_back(-1);
        vector<vector<int>> origin_tmp = rebuildOriginData(origin, prefix, flag);
        startPrefixSpan(origin_tmp,prefix);
        prefix.pop_back();
        prefix.pop_back();
        if(elem > number_of_element){
            prefix.push_back(-1); // 回溯记得要加上去啊 ！！！
        }

    }
}
int main(int argc,char **argv) { //
    clock_t start,end;
    start = clock();
    if(!debug) {
        if (argc == 1 || argc == 2 || argc == 3) {
            cout << "command should be PrefixSpan filename support max_element" << endl;
            exit(0);
        } else {
            _file.open(argv[1]);
            if (!_file) {
                cout << "file " << argv[1] << "doesn't exist!" << endl;
                exit(0);
            } else {
                regex rx("[1-9]+");
                bool has = regex_match(argv[2], rx);  // use regex to match number
                if (!has) {
//            if(!isdigit(stoi(argv[2]))){ // change string to int similar stol(long) stof(float)
                    cout << "support should be number" << endl;
                    exit(0);
                } else {
                    min_sup = stoi(argv[2]);
                    has = regex_match(argv[3], rx);
                    if(!has){
                        cout << "max_element should be number" << endl;
                        exit(0);
                    }
                    else{
                        number_of_element = stoi(argv[3]);
                    }
                    freopen("data.in","r",stdin);
                    freopen("data.out","w",stdout);
                }
            }
        }
    }
    else{
        _file.open("data.in");
        freopen("data.in","r",stdin);
        freopen("data.out","w",stdout);
    }
    vector<vector<int>> origin;
    vector<int>prefix;
    vector<int> data_tmp;
    int n;
    while(scanf("%d",&n)==1){
        if(n==-2){
            origin.push_back(data_tmp);
            data_tmp.clear();
        }
        else{
            data_tmp.push_back(n);
        }
    }
    if(check_data) {
        checkOrigin(origin);
    }
    //cout<<"start PrefixSpan"<<endl;
    startPrefixSpan(origin,prefix);
    end = clock();
    double interval = (double)(end - start) / CLOCKS_PER_SEC;
    cout<<"cost time : " <<interval<<endl;
    return 0;
}