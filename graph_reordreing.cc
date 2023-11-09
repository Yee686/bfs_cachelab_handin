/**
 * @brief 基于度降序的图重排
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
#define MII map<int, int>
#define PII pair<int, int>
#define VPII vector<pair<int, int>>
#define N 500
using namespace std;

MII vd;             // 节点入度统计
VPII old_map;       // 图的旧排布
int EDGE = 0;       // 边数目
string TYPE = "";

/**
 * @param graph_file 图文件地址
 * @brief 用于读取图结构文件,统计顶点度,用于后续排序
 */
void count_graph_degree(string graph_file)
{

    ifstream gfile(graph_file);
    string line;

    int sum = 0;

    if (gfile.is_open())
    {
        int s, t;

        while (getline(gfile, line))
        {
            
            istringstream iss(line);
            iss >> s >> t;
            cout << s << " " << t << endl;

            old_map.emplace_back(make_pair(s, t));
            vd[t] += 1;
            sum += 1;
        }

        cout << "count degree over: " << EDGE << " " << sum << endl;
    }
    else
    {
        perror("Can't open file!");
        return;
    }
}

bool cmp(const PII &a, const PII &b)
{
    return a.second > b.second;
}

/**
 * @brief 根据统计的节点的入度信息,对图节点重新编号,
 * 并将其映射关系输入./data/xxxx.map文件, 
 * 将映射后新的边输入./data/xxxx.txt文件
*/
void reorder_graph()
{
    string map_file_name =  "./data/" + to_string(EDGE) + 
                            "-"+ TYPE + ".map";
    ofstream mapfile(map_file_name);
    
    string edge_file_name = "./data/graph-" + to_string(EDGE) + 
                            "-resort-" + TYPE + ".txt";
    ofstream edgefile(edge_file_name);

    vector<PII> degreePair(vd.begin(), vd.end());

    sort(degreePair.begin(), degreePair.end(), cmp);

    int index = 0;

    MII vv; // 节点旧编号->新编号映射
    if (mapfile != NULL)
    {
        for (auto p : degreePair)
        {
            cout << p.first << "->" << index << " :" << p.second << endl;
            mapfile << p.first << " " << index << endl;
            vv[p.first] = index;
            index += 1;
        }

        for (int i = 0; i < EDGE; i++)
        {
            auto t = vv.find(i);
            if(t == vv.end())
            {
                cout << i << "->" << index << " :" << 0 << endl;
                vv[i] = index;
                index += 1;
            }
        }
    }
    else
    {
        cout << "Open node map file failed!" << endl;
        return;
    }

    if (edgefile != NULL)
    {
        for (auto p : old_map)
        {
            cout << p.first << " " << p.second << 
                "->" << vv[p.first] << " " << vv[p.second] << endl;
            edgefile << vv[p.first] << " " << vv[p.second] << endl;
        }
    }
    else
    {
        cout << "Open new edge file failed!" << endl;
        return;
    }
}

int main(int argc, char *argv[])
{
    string graph_file;
    

    if (argc == 3)
    {
        EDGE = stoi(argv[1]);
        TYPE = string(argv[2]);
        graph_file = "./data/graph-"+to_string(EDGE)+"-"+TYPE+".txt";
        cout<<"Graph file: "<<graph_file<<endl;
    }
    else
    {
        cout << "Invalid args!" << endl;
        return 0;
    }

    count_graph_degree(graph_file);
    reorder_graph();
    return 0;
}