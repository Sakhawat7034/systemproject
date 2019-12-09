#include <bits/stdc++.h>
#include "Random.cpp"

using namespace std;

#define TAUMAX 2
#define ALPHA (double)1
#define BETA (double)2
#define RO (double)0.3
#define Q (double)1000
double BESTLENGTH = (double)INT_MAX;
int connectionGraph[1000][1000];
int numberOfEdge = 0;
int numberOfBlockEdge = 0;
int numberOfNode = -100;
bool visited[100] = {0};
double prob[100][2] = {0};
double dis[100][100] = {0};
double cost[100][100] = {0};
double pheromen[100][100] = {0};
double DELTAPHEROMONES[100][100] = {0};
vector<int> ROUTES[11];
vector<set<int>> component;
vector<set<int>> component_boundary;
set<int> st;
vector<pair<int, int>> blocked_edge;
int base_node = 1;
set<int> base_component;
set<int> boundary_node;
Randoms *randoms;
vector<int> BESTROUTE[100];

void DFSUtil_(int s)
{
    visited[s] = true;
    st.insert(s);
    for (int i = 1; i <= numberOfNode; i++)
    {
        if (connectionGraph[s][i] == 1 && visited[i] == false)
        {
            DFSUtil_(i);
        }
    }
}

void visited_component(int init)
{
    for (int i = 0; i < component.size(); i++)
    {
        set<int>::iterator it = component[i].find(init);
        if (it != component[i].end())
        {
            for (set<int>::iterator it2 = component[i].begin(); it2 != component[i].end(); it2++)
            {
                visited[*it2] = 1;
            }
            break;
        }
    }
}

int valid(int antk)
{
    if (ROUTES[antk].size() == 0)
        return -1;
    for (int i = 0; i < ROUTES[antk].size() - 1; i++)
    {
        int cityi = ROUTES[antk][i];
        int cityj = ROUTES[antk][i + 1];
        if (cityi < 0 || cityj < 0)
        {
            return -2;
        }
        if (connectionGraph[cityi][cityj] == 0)
        {
            return -3;
        }
        for (int j = 0; j < i - 1; j++)
        {
            if (ROUTES[antk][i] == ROUTES[antk][j])
            {
                return -4;
            }
        }
    }

    // Check if element 22 exists in vector
    vector<int>::iterator it = find(ROUTES[antk].begin(), ROUTES[antk].end(), base_node);

    if (it == ROUTES[antk].end())
    {
        return -4;
    }

    return 0;
}

int city()
{
    double xi = randoms->Uniforme();
    //cout<<"random "<<xi<<endl;
    int i = 0;
    double sum = prob[i][0];
    while (sum < xi)
    {
        i++;
        sum += prob[i][0];
    }
    visited[(int)prob[i][1]] = 1;
    return (int)prob[i][1];
}

double PHI(int cityi, int cityj, int antk)
{
    double ETAij = (double)pow(1 / dis[cityi][cityj], BETA);
    double TAUij = (double)pow(pheromen[cityi][cityj], ALPHA);

    double sum = 0.0;
    for (int c = 1; c <= numberOfNode; c++)
    {
        if ((connectionGraph[cityi][c] != 0) && (visited[c] != 1))
        {
            double ETA = (double)pow(1 / dis[cityi][c], BETA);
            double TAU = (double)pow(pheromen[cityi][c], ALPHA);
            sum += ETA * TAU;
        }
    }
    return (ETAij * TAUij) / sum;
}

void route(int antk, int init)
{
    ROUTES[antk].push_back(init);
    memset(visited, 0, sizeof(visited));
    //visited_component(16);
    visited[init] = 1;
    for (int i = 0; i < ROUTES[antk].size(); i++)
    {
        int cityi = ROUTES[antk][i];
        int cnt = 0;
        for (int c = 1; c <= numberOfNode; c++)
        {
            if (cityi == c)
                continue;
            if ((connectionGraph[cityi][c] != 0) && (visited[c] != 1))
            {
                prob[cnt][0] = PHI(cityi, c, antk);
                prob[cnt][1] = (double)c;
                cnt++;
            }
        }
        // for(int k=0; k<cnt; k++)
        // {
        //     for(int m=0; m<2; m++)
        //     {
        //         cout<<prob[k][m]<<" ";
        //     }
        //     cout<<endl;
        // }
        // deadlock
        if (0 == cnt)
        {
            return;
        }
        int new_node = city();
        if (new_node != base_node)
        {

            ROUTES[antk].push_back(new_node);
        }

        else
        {
            ROUTES[antk].push_back(new_node);
            break;
        }
    }
}

double length(int antk)
{
    double sum = 0.0;
    for (int j = 0; j < ROUTES[antk].size() - 1; j++)
    {
        sum += dis[ROUTES[antk][j]][ROUTES[antk][j + 1]];
    }
    return sum;
}

void updatePHEROMONES()
{
    for (int k = 0; k < 4; k++)
    {
        double rlength = length(k);
        for (int r = 0; r < ROUTES[k].size() - 1; r++)
        {
            int cityi = ROUTES[k][r];
            int cityj = ROUTES[k][r + 1];
            DELTAPHEROMONES[cityi][cityj] += Q / rlength;
            DELTAPHEROMONES[cityj][cityi] += Q / rlength;
        }
    }
    for (int i = 1; i <= numberOfNode; i++)
    {
        for (int j = 1; j <= numberOfNode; j++)
        {
            pheromen[i][j] = (1 - RO) * pheromen[i][j] + DELTAPHEROMONES[i][j];
            DELTAPHEROMONES[i][j] = 0.0;
        }
    }
}

void optimize()
{

    for (int iterations = 1; iterations <= 5; iterations++)
    {
        cout << flush;
        //cout << "ITERATION " << iterations << " HAS STARTED!" << endl << endl;
        for (int i = 0; i < component_boundary.size(); i++)
        {
            vector<int> pathFromComponent[component_boundary[i].size()];
            int ptr = 0;
            cout << "component " << i << endl;
            for (set<int>::iterator it = component_boundary[i].begin(); it != component_boundary[i].end(); it++)
            {
                ptr++;
                for (int k = 0; k < 4; k++)
                {

                    while (0 != valid(k))
                    {

                        ROUTES[k].clear();
                        cout<<"com "<<i<<" "<<"ant "<<k<<" node "<<*it<<endl;
                        route(k, *it);
                    }

                    // for (int i = 0; i < ROUTES[k].size(); i++)
                    // {
                    //     cout << ROUTES[k][i] << " ";
                    // }
                    // cout << endl;

                    double rlength = length(k);
                    // cout << rlength << endl;
                    //cout<<"Ant "<<k<<endl;
                    for (int j=0; j<ROUTES[k].size(); j++) {
                    	cout<<ROUTES[k][j]<<" ";
                    }

                    if (rlength < BESTLENGTH)
                    {
                        BESTLENGTH = rlength;
                        ROUTES[k].clear();
                        pathFromComponent[ptr].clear();
                        for (int i = 0; i < ROUTES[k].size(); i++)
                        {
                            pathFromComponent[ptr].push_back(ROUTES[k][i]);
                            //cout << ROUTES[k][i] << " ";
                        }
                        //cout << endl;
                    }
                    //cout << " : ant " << k << " has ended!" << endl;
                }
                BESTLENGTH = (double)INT_MAX;
                // cout << "From component" << i << ";" << endl;
                // for (int j = 0; j < ptr; j++)
                // {
                //     for (int k = 0; k < pathFromComponent[j].size(); k++)
                //     {
                //         cout << pathFromComponent[j][k] << " ";
                //     }
                //     cout << endl;
                // }
                for (int i = 0; i < 4; i++)
                {
                    ROUTES[i].clear();
                }
                memset(visited, 0, sizeof(visited));
            }
           
        }

        //Update pheromones
        updatePHEROMONES();

        cout << endl
             << "ITERATION " << iterations << " HAS ENDED!" << endl
             << endl;
    }
    cout << BESTLENGTH << endl;
    for (int i = 0; i < BESTROUTE[0].size(); i++)
        cout << BESTROUTE[0][i] << " ";
}

int main()
{

    int n, x, y, i, j;
    FILE *ptr;
    string line;
    map<int, pair<int, int>> mp;
    map<int, pair<int, int>>::iterator it;
    bool visited[1000] = {0};
    ptr = fopen("projectData.txt", "r");
    freopen("out.txt", "w", stdout);
    i = 0;
    ///initialize random number
    randoms = new Randoms(21);
    while (fscanf(ptr, "%d %d %d", &x, &y, &n) == 3)
    {
        numberOfEdge++;
        //cout << x << " " << y << " " << n << endl;
        connectionGraph[x][y] = 1;
        connectionGraph[y][x] = 1;
        dis[x][y] = n;
        dis[y][x] = n;
        numberOfNode = max(numberOfNode, x);
        numberOfNode = max(numberOfNode, y);
    }

    i = 0;
    fclose(ptr);
    ptr = fopen("input2.txt", "r");
    while (fscanf(ptr, "%d %d", &x, &y) == 2)
    {
        //input block edge;
        connectionGraph[x][y] = 2;
        connectionGraph[y][x] = 2;
        i++;
        blocked_edge.push_back(make_pair(x, y));
    }
    fclose(ptr);
    //for component;
    for (int i = 1; i <= numberOfNode; i++)
    {
        st.clear();
        if (visited[i] == 0)
        {
            DFSUtil_(i);
        }
        for (set<int>::iterator it = st.begin(); it != st.end(); it++)
        {
            visited[*it] = 1;
        }
        if (!st.empty())
            component.push_back(st);
    }
    //   cout<<"component:"<<endl;
    //   for(i=0; i<component.size(); i++)
    //   {
    //       for(set<int>::iterator it=component[i].begin(); it!=component[i].end(); it++)
    //       {
    //           cout<<*it<<" ";
    //       }
    //       cout<<endl;
    //   }
    //Now  Collecting base component
    int flag = 0;
    for (i = 0; i < component.size(); i++)
    {
        for (set<int>::iterator it = component[i].begin(); it != component[i].end(); it++)
        {
            if ((*it) == base_node)
            {
                flag = 1;
                for (set<int>::iterator it = component[i].begin(); it != component[i].end(); it++)
                {
                    base_component.insert(*it);
                }
                break;
            }
        }
        if (flag == 1)
        {
            break;
        }
    }
    /*  cout<<"base component contains:";
      for(set<int>::iterator it=base_component.begin(); it!=base_component.end(); it++)
      {
          cout<<*it<<" ";
      }
      cout<<endl;*/

    //now collecting the boundary nodes
    //cout<<"do"<<endl;
    for (int i = 0; i < blocked_edge.size(); i++)
    {
        // cout<<blocked_edge[i].first<<" "<<blocked_edge[i].second<<endl;
        int bb = blocked_edge[i].first;
        //cout<<bb<<endl;
        set<int>::iterator it = base_component.find(bb);

        if (it == base_component.end())
        {
            //cout<<"into if";
            boundary_node.insert(bb);
            //cout<<*it<<endl;
        }
        bb = blocked_edge[i].second;
        //cout<<bb<<endl;
        it = base_component.find(blocked_edge[i].second);
        if (it == base_component.end())
        {
            //cout<<"into 2nd if";
            boundary_node.insert(bb);
            // cout<<*it<<endl;
        
        }
    }
    cout << "boundary:";
    for (set<int>::iterator it = boundary_node.begin(); it != boundary_node.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
    for (i = 0; i < component.size(); i++)
    {
        set<int> temp;
        temp.clear();
        for (set<int>::iterator it = component[i].begin(), it2 = boundary_node.begin(); it != component[i].end(); it++)
        {
            it2 = boundary_node.find(*it);
            if (it2 != boundary_node.end())
            {
                temp.insert(*it);
                cout << *it << " ";
            }
        }
        cout << endl;
        if (temp.size() != 0)
        {
            component_boundary.push_back(temp);
        }
    }

    //intinial pheromen
    for (i = 1; i <= numberOfNode; i++)
    {
        for (j = 1; j <= numberOfNode; j++)
        {
            if (connectionGraph[i][j] == 1 || connectionGraph[i][j] == 2)
            {
                pheromen[i][j] = randoms->Uniforme() * 1.5;
                pheromen[j][i] = pheromen[i][j];
            }
            DELTAPHEROMONES[i][j] = 0.0;
        }
    }
     for (int i = 0; i < component_boundary.size(); i++)
        {
            cout << "component " << i << endl;
            for (set<int>::iterator it = component_boundary[i].begin(); it != component_boundary[i].end(); it++)
            {
                cout<<*it<<" ";
            }
            cout<<endl;
        }

    optimize();
    cout<<"finish"<<endl;
}
