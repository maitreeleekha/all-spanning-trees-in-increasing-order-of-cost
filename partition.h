
vector<Partition> partitionMST(Partition p,vector<edge *> allEdges,int n)
{

    /*receives a partition type object as input (current partition). The result of this routine is a set of
    connected partitions formed from this partition. All the black and gray edges are added to all the sub partitions;
    the white edges that are open are added sequentially to various sub partitions.*/

    /*STRATEGY:
    ->separate the original partition into two sets: white edges (open) and non white edges (taken).
    ->the taken edges will be the same for all the sub partitions.
    ->iterate all the white edges and sequentially form sub partitions.
    ->then check that each sub partition is connected.*/

    vector<edge *> original;
    for(int i=0; i<p.edges.size(); i++)
    {
        original.push_back(p.edges[i]);
    }

    vector<edge *>whites;
    vector<edge *> nonwhites;

    for(int i=0; i<original.size(); i++)
    {
        if(original[i]->state==WHITE)
        {
            whites.push_back(original[i]);
        }
        else
        {
            nonwhites.push_back(original[i]);
        }
    }

    vector<Partition> ans;
    for(int k=0; k<whites.size(); k++)
    {

        /*map keeps a track of the edges that have been added*/
        map<edge *,bool> marker;
        for(int i=0;i<allEdges.size();i++){
            marker[allEdges[i]]=false;
        }

        /*add all the taken edges as it is to all the sub partitions.*/
        vector<edge *> curr;
        for(int i=0; i<nonwhites.size(); i++)
        {
            curr.push_back(nonwhites[i]);
            map<edge*,bool> :: iterator it = marker.begin();
            while(it!=marker.end()){
                if((it->first->s==nonwhites[i]->s && it->first->d==nonwhites[i]->d) || (it->first->s==nonwhites[i]->d && it->first->d==nonwhites[i]->s)){
                     it->second=true;
                }
                it++;
            }
        }


        int j=0;
        /*add all but the last edge as gray and the last one will be added as black.*/
        for(j=0; j<k; j++)
        {
            edge * temp = whites[j];
            temp->state=GRAY;
            curr.push_back(temp);
           map<edge*,bool> :: iterator it = marker.begin();
            while(it!=marker.end()){
                if((it->first->s==whites[j]->s && it->first->d==whites[j]->d) || (it->first->s==whites[j]->d && it->first->d==whites[j]->s)){
                     it->second=true;
                }
                it++;
            }
        }

        /*last edge as black.*/

        edge * ne = new edge(whites[j]->s,whites[j]->d,whites[j]->w,BLACK);
        curr.push_back(ne);
        map<edge*,bool> :: iterator it = marker.begin();
            while(it!=marker.end()){
                if((it->first->s==whites[j]->s && it->first->d==whites[j]->d) || (it->first->s==whites[j]->d && it->first->d==whites[j]->s)){
                     it->second=true;
                }
                it++;
            }

/*now add all the white edges that have not been added so far out of the total edges.
This is necessary so that the MST is evaluated correctly. Once we have evaluated the MST,
we update the set of edges with the edges that are actually a part of the MST.*/
        map<edge*,bool> :: iterator it1 = marker.begin();
        while(it1!=marker.end()){
              if(it1->second==false){
                curr.push_back(it1->first);
              }
              it1++;

              }
 /*check for connectedness of this set of edges curr.*/

       bool connected = checkConnected(curr,n);
      if(connected){
        ans.push_back(curr);
      }
    }
    return ans;
}
