
/*The prim's algorithm finds the base MST for the given undirected graph input in the form of adjacency list*/

int * PrimsMST(node_base ** arr, int s, int nov )
{
    int i;
    int * key= new int[nov*sizeof(int)];
    int * mark= new int[nov*sizeof(int)];
    int * parent= new int[nov*sizeof(int)];
    int * helper= new int[nov*sizeof(int)];

    for(i=0; i<nov; i++)
    {
        helper[i]=i;
    }

    //for all the vertices, set key value to infinity.
    for(i=0; i<nov; i++)
    {
        key[i]=INT_MAX;
    }
    //initially all unmarked.
    for(i=0; i<nov; i++)
    {
        mark[i]=0;
    }
    //parent of all initially -1= NIL
    for(i=0; i<nov; i++)
    {
        parent[i]=-1;
    }

    key[s]=0; //source key=0
    MakeHeap(key, nov, helper);

    int heapSize=nov;//initially heap size=v

    //while heap is not empty
    int cost=0;
    while(heapSize!=0)
    {
        MakeHeap(key,heapSize,helper);

        int u = delMinHeap(key,heapSize,helper);
        heapSize--;
        mark[u]=1;

        // for each vertex adjacent to u:
        node_base * temp = arr[u]->next;
        while(temp!=NULL)
        {

            int value = temp->key;
            i=0;
            while(helper[i]!=value)
            {
                i++;
            }
            if(mark[value]==0 && temp->weight<key[i])
            {
                key[i]= temp->weight;
                parent[value]=u;
            }
            temp=temp->next;
        }
    }
    return parent;
}


