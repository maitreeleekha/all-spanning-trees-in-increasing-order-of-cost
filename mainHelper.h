//included: gray (0)
//excluded: black (-1)
//open: white (1)

/*Each edge in the graph is of the type edge as described below:*/
class edge
{
public:
    int s; //source
    int d; //destination
    int w; //weight
    int state; //white, gray black

    edge(int i,int j,int weight,int s)
    {
        this->s=i;
        this->d=j;
        this->w=weight;
        this->state=s;
    }
};

/*Each partition is of the type described below:*/
class Partition
{
public:
    vector<edge *> edges; //set of edges in the partition
    vector<int> MST; //will store the MST of current partition. Initially empty.
    bool check=false; //checks of the MST for this partition has been evaluated or not. Initially false.
    int cost=INT_MAX; //Initially, the cost of the spanning tree is INT_MAX.

    Partition()
    {
        check=false;
        cost=INT_MAX;
    }
    Partition(vector<edge *> e)
    {
        for(int i=0; i<e.size(); i++)
        {
            edge * temp = e[i];
            edges.push_back(temp);
        }

        check=false;
        cost=INT_MAX;
    }
    Partition(vector<edge *> e, vector<int> m, int c, bool ch)
    {
        for(int i=0; i<e.size(); i++)
        {
            edge * temp = e[i];
            edges.push_back(temp);
        }

        for(int i=0; i<m.size(); i++)
        {
            MST.push_back(m[i]);
        }
        cost=c;
        check=ch;
    }
};


/*adjacency list holds the vertices as objects of class type node_base, described below:*/
class node_base
{
public:
    int key;
    int weight;
    node_base * next;
    node_base(int k,int w)
    {
        key=k;
        weight =w;
        next=NULL;
    }
};

/*Helper function to add edges to the adjacency list*/
void addEdgeij(node_base * arr[], int i,int j, int w)
{
    //initially isolated
    node_base * newVertex = new node_base(j,w);
    node_base * temp=arr[i];
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=newVertex;
    return;
}

/*priority queue for prims MST algorithm*/
void Heapify(int arr[], int i, int n, int * helper)
{
    while(1)
    {
        int lchild= 2*i+1;
        if(lchild>=n)
        {
            break;   //nochild heap prop satisfied
        }
        int rchild=lchild+1;
        if(rchild<n && arr[lchild]>arr[rchild])
        {
            lchild=rchild; //lchild now stores the index of the smaller of the two children
        }
        if(arr[i]>arr[lchild])
        {
            int temp= arr[i];
            arr[i]=arr[lchild];
            arr[lchild]=temp;

            temp= helper[i];
            helper[i]=helper[lchild];
            helper[lchild]=temp;

            i=lchild;
        }
        else
        {
            break;
        }

    }
}

void MakeHeap(int * key, int n, int* helper)
{
    int i=(n/2)-1;
    for(; i>=0; i--)
    {
        Heapify(key, i, n, helper);
    }
}

int delMinHeap(int *arr, int n, int *helper)
{
    int temp= arr[0];
    arr[0]=arr[n-1];
    arr[n-1]=temp;

    temp= helper[0];
    helper[0]=helper[n-1];
    helper[n-1]=temp;

    n--;
    Heapify(arr,0,n,helper);
    return temp;
}




/*final answer will store the parent array of each spanning tree along with its cost; this class type is described below:*/
class finalAns
{
public:
    vector<int> MST; //parent array
    int cost;

    finalAns(vector<int> p, int c)
    {
        for(int i=0; i<p.size(); i++)
        {
            MST.push_back(p[i]);
        }
        cost=c;
    }
};

/*forms a partition type object from the base(first/minimum) spanning tree. */
Partition formBasePartition(node_base ** arr,vector<int> MST, int cost, bool check)
{

    vector<edge *> edges;
    for(int i=1; i<MST.size(); i++)
    {
        node_base * t = arr[i];
        while(t!=NULL && t->key!=MST[i])
        {
            t=t->next;
        }
        edge * temp = new edge(i, MST[i],t->weight,WHITE);
        edges.push_back(temp);
    }

    Partition pb(edges,MST,cost,check);
    return pb;
}

/*Check connectivity of a partition using DFS. If the number of connected components is exactly 1, only then it is connected.*/
int time=0;

void DFSVisit(node_base * arr[], int i, int color[],int parent[], int d[], int f[])
{
    color[i]=GRAY;
    time++;
    d[i]=time;//set discovery time
    node_base * temp= arr[i]->next;
    while(temp!=NULL)
    {
        if(color[temp->key]==WHITE)
        {
            parent[temp->key]=i;
            DFSVisit(arr, temp->key, color, parent, d, f);
        }
        temp=temp->next;
    }
    color[i]=BLACK;
    time++;
    f[i]=time;
}
int DFS(node_base * arr[], int v)
{
    int i;
    int *parent = (int *)malloc(v*sizeof(int));
    int *color =  (int *)malloc(v*sizeof(int));
    int *discTime = (int *)malloc(v*sizeof(int));
    int *finishTime = (int *)malloc(v*sizeof(int));
    //set color initially to white= undiscovered for all
    for(i=0; i<v; i++)
    {
        color[i]=WHITE;
    }

    //set parent for all = -1 (NIL VALUE)
    for(i=0; i<v; i++)
    {
        parent[i]=-1;
    }
    int c=0;
    for(i=0; i<v; i++)
    {
        if(color[i]==WHITE)
        {
            c++;
            DFSVisit(arr,i,color,parent, discTime,finishTime);
        }
    }
    time=0;

    return c;
}


bool checkConnected(vector<edge *> edges,int n)
{

    node_base ** adjList = new node_base*[n];
    for(int i=0; i<n; i++)
    {
        node_base * temp = new node_base(i,0);
        adjList[i]=temp;
    }

    for(int i=0; i<edges.size(); i++)
    {

        if(edges[i]->state!=BLACK)
        {
            addEdgeij(adjList,edges[i]->s,edges[i]->d,edges[i]->w);
            addEdgeij(adjList,edges[i]->d,edges[i]->s,edges[i]->w);
        }
    }

    int noOfComponents = DFS(adjList,n);
    if(noOfComponents==1)
    {
        return true;
    }
    return false;

}

/*check for redundant answer type objects in the final answer vector, so that each spanning tree is added only once.*/
bool notAlreadyAdded(vector<finalAns> ans, finalAns a)
{
    for(int i=0; i<ans.size(); i++)
    {
        if(ans[i].cost==a.cost && ans[i].MST==a.MST)
        {
            return true;
        }
    }
    return false;
}

/*format and display the final answer*/
void printEdgesOfMST(finalAns a)
{
    cout<<'\t';
    for(int i=1; i<a.MST.size(); i++)
    {
        cout<<'('<<i<<','<<a.MST[i]<<')'<<'\t';
    }
}

/*priority queue compare function.*/
class compare
{
public:
    bool operator()(Partition lhs, Partition rhs)
    {
        return lhs.cost > rhs.cost;
    }
};



