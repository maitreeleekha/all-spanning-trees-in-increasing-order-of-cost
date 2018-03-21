#include<iostream>
#include<stdlib.h>
#include<queue>
#include<map>
#include<algorithm>
#include<vector>
#define BLACK -1 //excluded;
#define WHITE 0 //open;
#define GRAY 1 //included;

using namespace std;
#include "mainHelper.h"
#include "partition.h"
#include "primsPartition.h"
#include "prims.h"

/*global priority queue for storing the partitions as a min heap*/
priority_queue<Partition, vector<Partition>, compare> pqPartition;

int main(){

        /*FINAL MAIN PROGRAM*/
        int n;
        cout<<"Enter the number of vertices in the graph: ";
        cin>>n;

        node_base ** adjList = new node_base*[n];
        for(int i=0;i<n;i++){
            node_base * temp = new node_base(i,0);
            adjList[i]=temp;
        }

        vector<edge *> allEdges; //stores all the edges as input by the user.
        cout<<"Enter the number of edges: ";
        int e;
        cin>>e;
        cout<<"Enter the edges in the format: <source destination weight>"<<endl;
        cout<<"The various vertices are numbered as 0,1,2,3,..."<<endl;
        cout<<endl<<"Input Edge: "<<endl;

        //graph represented as adjacency list.
        int i=0;
        while(i<e){
            int s,d,w;
            cin>>s>>d>>w;
            addEdgeij(adjList,s,d,w);
            addEdgeij(adjList,d,s,w);
            edge * newEdge = new edge(s,d,w,WHITE);
            allEdges.push_back(newEdge);
            i++;
        }

        //MST_base is the base MST (first one).
        int * MST = PrimsMST(adjList,0,n);
        vector<int> MST_base;
        for(int i=0;i<n;i++){
            MST_base.push_back(MST[i]);
        }

        //calculate cost of base MST.
        int base_cost=0;
        for(int i=1;i<n;i++){
        node_base * temp = adjList[i];
        while(temp!=NULL && temp->key!=MST[i]){
            temp=temp->next;
        }
        base_cost+=temp->weight;
        }

        //answer vector stores the MST's in increasing order of their cost.
        vector<finalAns> answer;
        finalAns a(MST_base,base_cost);
        answer.push_back(a);

         vector<Partition> partitions;
         Partition pbase = formBasePartition(adjList,MST_base,base_cost,true);
         /* pqPartition is a priority queue in which all the partitions are arranged as min heap
         according to their spanning tree costs.*/

         //push the first spanning tree to the priority queue.
         pqPartition.push(pbase);

/*one by one extract the partition of min cost spanning tree (present at the top), push it to the answer vector,
and then partition it by its spanning tree. Then, calculate the minimum spanning tree for each of this partition.
After calculation, push these partitions to the priority queue, where the will be inserted to the min heap according
to the cost of their respective MSTs. Now again, pop the partition whose cost is minimum and find its partitions.
Proceed this way till the priority queue becomes empty, which suggests that all the possible spanning trees have been found*/

         while(!pqPartition.empty()){

          //extracts the min cost partition from the current set of partitions.
          Partition curr = pqPartition.top();
          pqPartition.pop();

          finalAns a(curr.MST, curr.cost);
          if(!notAlreadyAdded(answer,a)){
            answer.push_back(a);
          }
          vector<Partition> subParts =  partitionMST(curr,allEdges,n);
          //push these subParts into the partitions vector.
           for(int i=0;i<subParts.size();i++){
              partitionPrims(subParts[i],n,adjList);
          }

          for(int i=0;i<subParts.size();i++){
            pqPartition.push(subParts[i]);
          }
         }
         cout<<endl<<"Total Number of Spanning Trees: "<<answer.size()<<endl;
         cout<<endl<<"The edge sets and costs of the spanning trees in increasing order of cost are as follows:"<<endl<<endl;
         for(int i=0;i<answer.size();i++){
                cout<<i+1<<"   Spanning Tree- Edge-set: ";
                printEdgesOfMST(answer[i]);
                cout<<"cost:"<<answer[i].cost<<endl;
         }

}
