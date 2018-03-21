
/*Helper class type object node. The adjacency list representation for finding the spanning tree from the partition type
object stores vertices and edges as node type objects*/
class node
{
public:
    int key;
    int weight;
    int state; //edge state i,e,o.
    node * next;

    node(int k,int w,int s)
    {
        this->key = k;
        this->weight = w;
        this->state=s;
        this->next =  NULL;
    }

};

/*The MST of the spanning tree from a partition type object is stored intermediately as a parentObj class type object.*/
class parentObj
{
public:
    int p;
    bool check;//true if edge is to be included permanently.

    parentObj(int i,bool c)
    {
        p = i;
        check = c;
    }
};

/*add edges to the adjacency list*/
void addEdge(node ** adjList, int s, int d,int w, int state)
{

    if(state==BLACK)
    {
        w=INT_MAX;
    }

    node * newVertex = new node(d,w,state);
    node * temp=adjList[s];
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=newVertex;
    return;
}

/*input to this routine is a vector of edges each of which has a state i,e or o. It finds the MST resulting from this partition.
 n = number of vertexes*/

void partitionPrims(Partition & p,int n, node_base ** arr)
{

/*Temporary parent vector will have objects of the pair type <int, bool> : bool = true if the edge is
included and hence will not be changed even if a smaller edge is found; else it is false. The edges that
are excluded are given a temporary weight of INT_MAX (infinity) so that they are never included in the MST.*/

/*Find the adj list representation for the edge set of the partition.*/
    vector<edge *> updatedEdges;
    node ** adjList = new node* [n];
    for(int i=0; i<n; i++)
    {
        node * temp = new node(i,0,0);
        adjList[i] = temp;
    }

    for(int i=0; i<p.edges.size(); i++)
    {
        addEdge(adjList,p.edges[i]->s,p.edges[i]->d, p.edges[i]->w,p.edges[i]->state);
        addEdge(adjList,p.edges[i]->d,p.edges[i]->s, p.edges[i]->w,p.edges[i]->state);
        if(p.edges[i]->state!=WHITE){
            updatedEdges.push_back(p.edges[i]);
        }
    }
/*taking source for the MST as 0 (fixed)*/

    int * key = new int[n];
    int * mark = new int[n];
    int * helper = new int[n];

    for(int i=0; i<n; i++)
    {
        helper[i]=i;
    }

    parentObj ** parent = new parentObj* [n];

//initially set parent to -1 for all.
    for(int i=0; i<n; i++)
    {
        parentObj * temp  = new parentObj(-1,false);
        parent[i]=temp;
    }
//for all the vertices, set key value to infinity.
    for(int i=0; i<n; i++)
    {
        key[i]=INT_MAX;
    }
//initially all unmarked.
    for(int i=0; i<n; i++)
    {
        mark[i]=0;
    }
//source key =0.

    key[0]=0;
    int heapSize=n; //initial heap size.


    while(heapSize!=0)
    {

        MakeHeap(key,heapSize,helper);
        int u = delMinHeap(key,heapSize,helper);
        heapSize--;
        mark[u]=1;
        node * temp = adjList[u]->next;

        while(temp!=NULL)
        {

            int i;
            int value = temp->key;
            for(i=0; i<n; i++)
            {
                if(helper[i]==value)
                {
                    break;
                }
            }

            if(mark[value]==0 && temp->weight<key[i] && parent[value]->check==false)
            {
                key[i]= temp->weight;
                parentObj * p = new parentObj(u,false);
                if(temp->state==GRAY)
                {
                    p->check=true;
                }
                parent[value]=p;
            }
            temp=temp->next;
        }
    }

    p.check=true; //MST evaluated
    vector<int> MST;
    for(int i=0; i<n; i++)
    {
        int j=parent[i]->p;
        MST.push_back(j);
    }

    p.MST = MST;

        int cost=0;
        for(int i=1;i<n;i++){
        node_base * temp = arr[i];
        while(temp!=NULL && temp->key!=MST[i]){
            temp=temp->next;
        }
        cost+=temp->weight;
        }
    p.cost =cost;

    /*update the edge list of the partition with the edges that are a part of the final MST formed by this partition*/
    p=formBasePartition(arr,MST,cost,true);
    for(int i=0;i<updatedEdges.size();i++){
            bool flag=false;
        for(int j=0;j<p.edges.size();j++){
            if((p.edges[j]->s==updatedEdges[i]->s && p.edges[j]->d==updatedEdges[i]->d ) || (p.edges[j]->s==updatedEdges[i]->d && p.edges[j]->d==updatedEdges[i]->s)) {
                p.edges[j]->state = updatedEdges[i]->state;
                flag=true;
            }
        }
        if(flag==false){
             p.edges.push_back(updatedEdges[i]);
        }
    }

    /*deallocation*/
     for(int i=0;i<n;i++){
       node * temp = adjList[i];
       node * n;
       while(temp!=NULL){
            n=temp->next;
            delete temp;
            temp=n;
       }
     }
     delete adjList;
}
