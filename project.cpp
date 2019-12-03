#include<bits/stdc++.h>
#include<set>
using namespace std;
int connectionGraph[1000][1000];
int numberOfEdge=38;
bool visited[11][25];
int route[11][25];
double prob[25][25];
double dis[26][26];
double cost[26][26];
double pheromen[26][26];
vector<set<int> >v;
set<int>st;
vector<pair<int,int> >blocked_edge;
int base_node=15;
set<int>base_component;
set<int>boundary_node;
void DFSUtil_(int s)
{
    visited[s] = true;
    st.insert(s);
    for (int i = 1; i <= 24; i++)
    {
        if (connectionGraph[s][i] == 1 && visited[i] == false)
        {
            DFSUtil_(i);
        }
    }
}
int main()
{

    int n,x,y,i,j;
    string line;
    map<int,pair<int,int> >mp;
    map<int,pair<int,int> >::iterator it;

    bool visited[1000]= {0};
    freopen("system.txt","r",stdin);
    i=0;
    while(i<24)
    {
        //intput city;
        cin>>n>>x>>y;
        mp[n]=make_pair(x,y);
        i++;

    }

    i=0;
    while(i<76)
    {
        //input graph;
        cin>>x>>y;
          //cout<<x<<" "<<y<<endl;
        connectionGraph[x][y]=1;
        i++;
    }
    i=0;
    while(i<13)
    {
        //input block edge;
        cin>>x>>y;
        //cout<<x<<" "<<y<<endl;
        connectionGraph[x][y]=2;
        connectionGraph[y][x]=2;
        i++;
        blocked_edge.push_back(make_pair(x,y));
    }
//for component;
    for(int i=1; i<=24; i++)
    {
        st.clear();
        if(visited[i]==0)
        {
            DFSUtil_(i);
        }
        for(set<int>::iterator it=st.begin(); it!=st.end(); it++)
        {
            visited[*it]=1;
        }
        if(!st.empty())
            v.push_back(st);
    }
    for(i=0; i<v.size(); i++)
    {
        for(set<int>::iterator it=v[i].begin(); it!=v[i].end(); it++)
        {
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    int flag=0;
    for(i=0; i<v.size(); i++)
    {
        for(set<int>::iterator it=v[i].begin(); it!=v[i].end(); it++)
        {
            if((*it)==base_node)
            {
                flag=1;
                for(set<int>::iterator it=v[i].begin(); it!=v[i].end(); it++)
                {
                    base_component.insert(*it);
                }
                break;
            }
        }
        if(flag==1)
        {
            break;
        }

    }
    cout<<"base component contains:";
    for(set<int>::iterator it=base_component.begin(); it!=base_component.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    //cout<<"do"<<endl;
    for(int i=0; i<blocked_edge.size(); i++)
    {
        // cout<<blocked_edge[i].first<<" "<<blocked_edge[i].second<<endl;
        int bb=blocked_edge[i].first;
        //cout<<bb<<endl;
        set<int>::iterator it=base_component.find(bb);


        if(it==base_component.end())
        {
            //cout<<"into if";
            boundary_node.insert(bb);
            //cout<<*it<<endl;
        }
        bb=blocked_edge[i].second;
        //cout<<bb<<endl;
        it=base_component.find(blocked_edge[i].second);
        if(it==base_component.end())
        {
            //cout<<"into 2nd if";
            boundary_node.insert(bb);
            // cout<<*it<<endl;
        }
    }
    cout<<"boundary:";
    for(set<int>::iterator it=boundary_node.begin(); it!=boundary_node.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
//cost between cities
double maxx=-1;
    for(it=mp.begin(); it!=mp.end(); it++)
    {
        int node=it->first;
        for(int i=1;i<=24;i++)
        {
            if(connectionGraph[node][i]==1 || connectionGraph[node][i]==2 )
            {
               long long x=powl(it->second.first-mp[i].first,2);
               long long y=powl(it->second.second-mp[i].second,2);
               dis[node][i]=sqrt(x+y);

            }
        }

    }
    //intinial pheromen
    for(i=1;i<=24;i++)
    {
        for(j=1;j<=24;j++)
        {
            if(connectionGraph[i][j]!=0)
           pheromen[i][j]= 1/dis[i][j];

        }

    }

    for(i=1;i<=50;i++)
    {
        //10 ant
        for(int antk=1;antk<=10;antk++)
        {
            if()
        }

    }

}

