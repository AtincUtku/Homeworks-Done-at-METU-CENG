#include "Graph.h"

/*Do not add new libraries or files*/


Graph::Graph() 
{

    
}

Graph::Graph(const Graph& rhs) 
{
    nameList = rhs.nameList;

}

Graph& Graph::operator=(const Graph& rhs) 
{
    this->nameList = rhs.nameList;

    

}

Graph::~Graph() 
{
    

}


void Graph::addNode(const Node &node) 
{

    list<Edge> names;
    nameList.Insert(node.getCountry(), names);


}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) 
{

    
    Edge v;
    v.setTailNode(tailNode);
    v.setImport(import);
    
    nameList.Get(headNode.getCountry()).push_back(v);
    
    

}

list<Node> Graph::getAdjacentNodes(const Node& node) 
{
    list<Node> adj;
    nameList.Get(node.getCountry());
    
    for (auto &c: nameList.Get(node.getCountry()))
    {
        adj.push_back(c.getTailNode());
    }
    

    
    return adj;
    
    

}

long Graph::getTotalImports(const Node& node) 
{
    long sum=0;
    
    for (auto &c: nameList.Get(node.getCountry()))
    {
        sum += c.getImport();
    }
    
    return sum;

}

void Graph::deleteNode(const Node& node) 
{
    nameList.Delete(node.getCountry());
    
    string *names = new string[nameList.Size()];
    
    nameList.getKeys(names);
    
    list <Edge> tmp;
    
    
    for (int i=0; i<nameList.Size(); i++)
    {
        for (auto &itr: nameList.Get(names[i]))
        {
            if (itr.getTailNode().getCountry() != node.getCountry())
            {
                tmp.push_back(itr);
            }
        }
        
        
        nameList.Get(names[i]) = tmp;
        tmp.clear();
        
    }
    
    delete [] names;
    
}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) 
{
    priority_queue<pair<long, list<string>>, vector<pair<long, list<string>>>, greater<pair<long, list<string>>> > pq;
    
    list<string> path;
    
    string *names = new string[nameList.Size()];
    nameList.getKeys(names);
    
    path.push_back(srcNode.getCountry());
    pq.push(make_pair(0, path));

    
    while (!pq.empty())
    {
        long imp = pq.top().first;
        list<string> path = pq.top().second;
        string curr = path.back();
        pq.pop();
        
        if (curr == destNode.getCountry())
        {
            delete [] names;
            return path;
        }
        
        bool visited=false;
        
        for (int i=0; i<nameList.Size(); i++)
        {
            if (names[i] == curr)
            {
                visited = true;
                names[i] = "used";

                break;
            }
        }
        
        if (visited == false) 
        {
            continue;           
        }    
            
        
        list<Edge> neighbours = nameList.Get(curr);
        
        for (auto &c: neighbours)
        {
            path.push_back(c.getTailNode().getCountry());
            long weight = c.getImport() + imp;
            pq.push(make_pair(weight, path));
            
            path.pop_back();
        }

    }

    delete [] names;
    return path;

}


bool Graph::isCyclic() 
{
    vector<bool> visit;
    vector<bool> stack;
    
    for (int i=0; i<nameList.Size(); i++)
    {
        bool add=false;
        visit.push_back(add);
        stack.push_back(add);
        
    }
    
    for (int i=0; i<nameList.Size(); i++)
    {
        if (circular(i, visit, stack) == true)
        {
            return true;
        }
    }
    
    return false;

}

bool Graph::circular(int i, vector<bool> visit, vector<bool> stack)
{
    string *names = new string[nameList.Size()];
    nameList.getKeys(names);
    int j=0;
    
    if (visit[i] == false)
    {
        visit[i] = true;
        stack[i] = true;
        
        for (auto &itr: nameList.Get(names[i]))
        {
            for (j=0; j<nameList.Size(); j++)
            {
                if (names[j] == itr.getTailNode().getCountry())
                {
                    break;
                }
            }
            
            if (!visit[j] && circular(j, visit, stack))
            {
                delete [] names;

                return true;
            }
            
            else if (stack[j])
            {
                delete [] names;
    
                return true;
            }
        }
    }
    
    stack[i] = false;
    delete [] names;
    
    return false;
} 



list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) 
{
    string *names = new string[nameList.Size()];
    nameList.getKeys(names);
    
    int x=0;
    int y=0;
    int z=0;
    for (x=0; x<nameList.Size(); x++)
    {
        if (names[x] == srcNode.getCountry())
        {
            break;
        }
    }
    
    for (y=0; y<nameList.Size(); y++)
    {
        if (names[y] == destNode.getCountry())
        {
            break;
        }
    }    
    
    vector<bool> visited(nameList.Size(), false);
    
    queue<string> q;
    
    list<string> path;
    
    visited[x] = true;
    q.push(srcNode.getCountry());
    
    string a;
    
    while (!q.empty())
    {
        a = q.front();
        
        if (a == destNode.getCountry())
        {
            path.push_back(a);
            delete [] names;
            return path;
        }
        path.push_back(q.front());
        
        q.pop();
        
        for (auto &c: nameList.Get(a))
        {
            for (z=0; z<nameList.Size(); z++)
            {
                if (names[z] == c.getTailNode().getCountry())
                {
                    break;
                }
            }
            
            if (!visited[z])
            {
                visited[z] = true;
                q.push(names[z]);
            }
        }
        
        
    }

    
    
    delete [] names;
    return path;

    

}

