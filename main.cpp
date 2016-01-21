#include <iostream>
using namespace std;

struct llist
{
    int value; //node id
    llist *next=NULL;  //list of other nodes connected to it in the graph
    int degree=0; //number of nodes connected to it in the graph
};

struct node //node struct to implement priority queue
{
    int id;
    int weight;
    struct node *link;
};

class PriorityQ //class implementing priority queue
{
    node *frnt;
    int qmax;

    public:

    PriorityQ()
    {
        frnt = NULL;
        qmax = 1000;
    }

    PriorityQ(int n)
    {
        frnt = NULL;
        qmax = n;
    }

    bool qempty()
    {
        if(frnt == NULL)
        {
            return true;
        }
        else
            return false;
    }

    void insert(int i, int wt)
    {
        node *temp, *f;
        temp = new node;
        temp->id = i;
        temp->weight = wt;

        if(qempty() || wt < frnt->weight)
        {
            temp->link = frnt;
            frnt = temp;
        }
        else
        {
            f = frnt;

            while (f->link != NULL && f->link->weight <= wt)
            {
                f = f->link;
            }
            temp->link = f->link;
            f->link = temp;
        }
    }

    int extractMin()
    {
        node *temp;

        if(qempty())
        {
            cout<<"Queue empty"<<endl;
            return qmax;
        }
        else
        {
            temp = frnt;
            frnt = frnt->link;
            return temp->id;
        }
    }

    bool qfind(int i)
    {
        if(qempty())
        {
            return false;
        }
        else
        {
            node *temp;
            temp = frnt;
            while (temp->link != NULL)
            {
                if (temp->id == i)
                {
                    return true;
                }
                temp = temp->link;
            }
            if (temp->id == i)
            {
                return true;
            }
            else
                return false;
        }
    }

    void update(int i, int wt)
    {
        node *temp, *f;
        f = frnt;
        if(f->id == i)
        {
            frnt = frnt->link;
            insert(i,wt);
        }
        else
        {
            while (f->link != NULL)
            {
                if (f->id == i)
                {
                    break;
                }
                temp = f;
                f = f->link;
            }
            temp->link = f->link;
            insert(i,wt);
        }
    }
};

struct vertex //used to implement dijkstras algorithm
{
    int dist; //distance from parent
    int id; //id of the node
    struct vertex *parent; //predecessor of the node in the adjacency list
};

struct comp //used to implement component function
{
    int value; //id of the node
    int rank; // number of nodes connected to it
    comp *parent=NULL; //predecessor of the node in the adjacency list
};

void makeSet (comp *cset[], int n) //create independent set for each node
{
    for(int i=0; i<n; i++)
    {
        cset[i]=new comp;
        cset[i]->value=i;
        cset[i]->rank=0;
        cset[i]->parent=cset[i];
    }
}

comp* findComp (comp *s) //find the top most parent node for a given node
{
    if (s->value != s->parent->value)
    {
        s->parent = findComp(s->parent);
    }
    return s->parent;
}

void linkComp (comp *a, comp *b) //merge two components using union by rank approach
{
    if(a->value != b->value)
    {
        if(a->rank > b->rank)
        {
            b->parent=a;
        }
        else
        {
            a->parent=b;
            if (a->rank == b->rank)
            {
                b->rank = b->rank +1;
            }
        }
    }
}

void unionComp (comp *a, comp *b) //merge two distinct components together
{
    linkComp(findComp(a), findComp(b));
}

void components (comp *cset[], llist *link[], int n) //print number of connected components using BFS
{
    makeSet(cset, n); //create independent set for each node
    int temp=0;
    int connect[n];
    for (int i=0; i<n ; i++)
    {
        connect[i]=0;
    }
    for (int i=0; i<n ; i++)
    {
        llist *ref1=link[i];
        while(ref1->next != NULL)
        {
            temp= ref1->next->value;
            unionComp(cset[i], cset[temp]); //merge two distinct components together
            ref1=ref1->next;
        }
    }
    for (int i=0; i<n ; i++)
    {
        comp *ref2=findComp(cset[i]);
        temp = ref2->value;
        connect[temp]+=1;
    }

    int key;
    for(int i=1; i<n; i++) //Insertion Sort
    {
        key=connect[i];
        int j=i-1;
        while(j>=0 && connect[j]>key)
        {
            connect[j+1]=connect[j];
            j=j-1;
        }
            connect[j+1]=key;
    }
    temp =0;
    for (int i=0; i<n ; i++)
    {
        if(connect[i]!=0)
        {
            //cout<<connect[i]<<endl;
            temp++;
        }
    }
    cout<<temp<<endl;
    for (int i=0; i<n ; i++)
    {
        if(connect[i]!=0)
        {
            cout<<connect[i]<<endl;
        }
    }
}

void histogram (llist *link[], int n) //count no of nodes of degree 0-n
{
    int deg[n];
    for (int i=0; i<n ; i++)
    {
        deg[i]=0;
    }
    for (int i=0; i<n ; i++)
    {
        int temp = link[i]->degree;
        int temp2 = deg[temp];
        temp2++;
        deg[temp] = temp2;
    }
    for (int i=0; i<n ; i++)
    {
        if(deg[i]!=0)
        {
            cout<<i<<"  "<<deg[i]<<endl;
        }
    }

}

void initsinglesource (vertex *V[], int s, int n) //initialize each vertex
{
    for (int i=0; i< n; i++)
    {
        V[i] = new vertex;
        V[i]->dist = n+10;
        V[i]->id = i;
        V[i]->parent = NULL;
    }

    V[s]->dist=0;
    V[s]->parent = V[s];
}

void relax (vertex *u, vertex *v, PriorityQ *p, int w ) //update the distance of vertex v from u
{
    if(v->dist > u->dist+w)
    {
        v->dist =  u->dist+w;
        v->parent = u;
        p->update(v->dist,u->dist+w);
    }
}

void path (int s, vertex *t) //print path from s to t
{
    if(t->parent == NULL)
    {
        cout<<"No path exist from " << s <<" - "<< t->id<<endl;
    }
    else if( t->parent == t)
    {
        cout<<t->id;
    }
    else
    {
        path(s,t->parent);
        cout <<" -> "<<t->id;
    }
}

void dijkstra (llist *link[],int s, int t, int n ) //find shortest path from a to b using dijkstras algorithm
{
    vertex *dij[n];
    PriorityQ pq;
    initsinglesource(dij, s, n); //initialize each vertex

    for (int i=0; i<n; i++)
    {
        pq.insert(dij[i]->id, dij[i]->dist);
    }

    while(!pq.qempty())
    {
        int u = pq.extractMin();
        llist *ref1=link[u];
        while(ref1->next != NULL)
        {
            int temp = ref1->next->value;
            if(pq.qfind(temp))
            {
                relax(dij[u], dij[temp], &pq, 1 ); //update the distance of vertex v from u
            }
            if(temp == t)
            {
                break;
            }
            ref1=ref1->next;
        }
    }

    cout<<"The length of the path from " <<s<<" - " <<t<< " is "<<dij[t]->dist<<endl;
    path(s, dij[t]); //print path from s to t
    cout<<endl;
}

int main()
{
    int n=0;
    int m=0;
    int a,b;
    int flag = 0;
    cin>>n;
    cin>>m;

    llist *A[n]; //linked list representation of graph
    comp *cset[n]; //sets to represent connected components

    for (int i=0; i<n; i++)
    {
        llist *item=new llist;
        item->value=i;
        item->next=NULL;
        item->degree=0;
        A[i]=item;//null
    }

    for (int i=0; i<m; i++)
    {
        cin>>a;
        cin>>b;

        if ( a != b )
        {
            llist *a1=new llist;
            a1->value=a;
            a1->next= NULL;
            llist *b1=new llist;
            b1->value=b;
            b1->next= NULL;
            llist *ref1=new llist;
            ref1=A[a];
            int temp=0;
            temp=A[a]->degree;
            temp++;
            while(ref1->next != NULL)
            {
                if( ref1->next->value == b)
                {
                    flag = 1;
                    break;
                }
                ref1=ref1->next;
            }
            if ( flag != 1)
            {
                ref1->next=b1;
                A[a]->degree=temp;
            }

            llist *ref2=new llist;
            ref2=A[b];
            temp=0;
            temp=A[b]->degree;
            temp++;
            flag = 0;

            while(ref2->next != NULL )
            {
                if( ref1->next->value == a)
                {
                    flag = 1;
                    break;
                }
                ref2=ref2->next;
            }
            if ( flag != 1)
            {
                ref2->next=a1;
                A[b]->degree=temp;
            }
        }
    }

    int f;
    string input;
    cin>>f;

    for(int i=0; i<f; i++)
    {
        cin>>input;
        if(input.compare("degree-distribution")==0)
        {

            cout<<"------------ Graph Analyses # "<<i+1<<" ------------"<<endl;
            histogram(A,n); //count no of nodes of degree 0-n
        }
        else if(input.compare("shortest-path")==0)
        {
            cin>>a;
            cin>>b;
            cout<<"------------ Graph Analyses # "<<i+1<<" ------------"<<endl;
            dijkstra(A,a,b,n); //find shortest path from a to b using dijkstras algorithm
        }
        else if(input.compare("components")==0)
        {
            cout<<"------------ Graph Analyses # "<<i+1<<" ------------"<<endl;
            components(cset, A, n); //print number of connected components using BFS
        }
        else
        {
            cout<<"Wrong input"<<endl;
        }
    }
    return 0;
}
