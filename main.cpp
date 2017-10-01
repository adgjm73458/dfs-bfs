#ifndef GRAPH_H
#define GRAPH_H
#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<stdexcept>
#include<queue>
#include<algorithm>
class Graph{
private:
    typedef std::vector<std::vector<bool>> AdjacentMatrix;
    typedef std::vector<std::string> NamesList;
    NamesList m_namesOfNodes;
    AdjacentMatrix m_matrix;
    int m_size;
private:
    bool checkName(std::string) const;
    NamesList getPath(const std::vector<int> &, int from, int to) const;
    void DFSrecursive(int current, int destination,
    bool & found, std::vector<int> & moved,
    std::vector<int> & used);
public:
    Graph() : m_size(0) {}
    AdjacentMatrix getMatrix() const;
    NamesList getNamesOfNodes() const;
    int getIndex(std::string);
    void addNode(std::string name);
    void addPath(std::string from, std::string to);
    NamesList BFS(std::string name);
    NamesList DFS(std::string name);
};

#endif // GRAPH_H

void Graph::addNode(std::string name)
{
    if(m_size == 0) {
        m_namesOfNodes.push_back(name);
        m_matrix.push_back(std::vector<bool>());
        m_matrix[0].push_back(0);
        m_size++;
    }
    else {
        if(checkName(name))
            throw std::runtime_error("Node already exists.");
        m_namesOfNodes.push_back(name);
        m_matrix.push_back(std::vector<bool>());
        for(int i = 0; i < m_size; i++)
            m_matrix[m_size].push_back(0);
        for(int i = 0; i < m_size + 1; i++)
            m_matrix[i].push_back(0);
        m_size++;


    }
}

void Graph::addPath(std::string from, std::string to)
{
    if((checkName(from) && checkName(to)) == 0 || from == to)
        throw std::runtime_error(std::string("There is no possible route between ")
                                 + from + std::string(" and ")
                                 + to + std::string("."));
    int from_pos, to_pos;
    for(int i = 0; i < m_size; i++) {
        if(m_namesOfNodes[i] == from)
            from_pos = i;
        if(m_namesOfNodes[i] == to)
            to_pos = i;
    }
    m_matrix[from_pos][to_pos] = 1;
    m_matrix[to_pos][from_pos] = 1;
}


bool Graph::checkName(std::string name) const
{
    for(int i = 0; i < m_size; i++)
        if(m_namesOfNodes[i] == name)
            return true;
    return false;
}

Graph::AdjacentMatrix Graph::getMatrix() const
{
    return m_matrix;
}

Graph::NamesList Graph::getNamesOfNodes() const
{
    return m_namesOfNodes;
}

int Graph::getIndex(std::string name)
{
    for(int i = 0; i < m_size; i++)
        if(m_namesOfNodes[i] == name)
            return i;
    return -1;
}

Graph::NamesList Graph::getPath(const std::vector<int> & array, int from, int to) const
{
    NamesList result;
    for(int i = to; i != from; i = array[i])
        result.push_back(m_namesOfNodes[i]);
    result.push_back(m_namesOfNodes[from]);
    std::reverse(result.begin(),result.end());
    return result;
}

Graph::NamesList Graph::DFS(std::string name)
{
        std::vector<int> used(m_size);      //Массив, хранящий использованные вершины
        std::vector<int> moved(m_size);      //Массив, хранящий пути
        bool found = false;
        int finish = getIndex(name);
        DFSrecursive(0, finish,found, moved, used);
        if(found)
            return getPath(moved,0,finish);
        else
            return NamesList();

}

void Graph::DFSrecursive(int current, int destination,
                         bool & found, std::vector<int> & moved,
                         std::vector<int> & used)
{
    if(found)
        return;
    if (current == destination) {  // Проверяем, конец ли
        found = true;
        return;
    }
    used[current] = 1;
    for ( int i=0; i<m_size; i++ )  //Находим смежные.
        if ( m_matrix[current][i]==1) {
            if(used[i] == 1)
                continue;
            moved[i] = current;
            DFSrecursive(i, destination, found, moved, used);
        }
}

Graph::NamesList Graph::BFS(std::string name)
{
    if(!checkName(name))
        throw std::runtime_error("There is no node with such name.");

    std::queue<int> turn;               //Это очередь, хранящая номера вершин
    std::vector<int> used(m_size);      //Массив, хранящий использованные вершины
    std::vector<int> from(m_size);      //Массив, хранящий пути

    turn.push(0);
    while(!turn.empty()) {              //Проверяем, пуста ли очередь
        int index = turn.front();       //Берем из очереди крайнюю вершину.
        if(m_namesOfNodes[index] == name)
            return getPath(from, 0, index);
        used[index] = 1;                //Помечаем как пройденную.
        turn.pop();                     //Удаляем ее.
        for ( int i=0; i<m_size; i++ )  //Находим смежные.
            if ( m_matrix[index][i]==1) {
                if(used[i] == 1)
                    continue;
                turn.push(i);           //добавляем в очередь вершину i.
                from[i] = index;
            }
    }
    return NamesList();
}
void printMenu();
void errMsg(std::string what);

int main()
{
    using std::cout;
    using std::cin;

    Graph graph;
    std::cout << "Hello, this is a lil' programm which helps you to test the Graph class. Enjoy!\n\n\n";
    printMenu();
    char ch;
    cin.get(ch).get();
    while (toupper(ch) != 'Q') {
        switch (ch) {
            case '1': {
                std::string temp;
                cout << "Name of the node: _\b";
                std::getline(cin,temp);
                try {
                    graph.addNode(temp);
                }
                catch(std::runtime_error & e) {
                        errMsg(e.what());
                        break;
                }
                cout << "Successfull.\n";
                break;
            }
            case '2': {
                std::string temp1;
                cout << "Name of the first node: _\b";
                std::getline(cin,temp1);
                std::string temp2;
                cout << "Name of the second node: _\b";
                std::getline(cin,temp2);
                try {
                    graph.addPath(temp1,temp2);
                }
                catch(std::runtime_error & e) {
                        errMsg(e.what());
                        break;
                }
                cout << "Successfull.\n";
                break;
            }
            case '3': {
                std::string temp;
                cout << "Name of the searching node: _\b";
                std::getline(cin,temp);
                try {
                    auto result = graph.BFS(temp);
                    if(result.size()) {
                        cout << "Path to " << temp << " is " ;
                        for(auto node : result)
                            cout << node << "  ";
                        cout << std::endl;
                    }
                    else
                        cout << "There is no route between beginning of the Graph and " << temp << ".\n";
                }
                catch(std::runtime_error & e) {
                        errMsg(e.what());
                        break;
                }
                cout << "Successfull.\n";
                break;
            }
        case '4': {
            std::string temp;
            cout << "Name of the searching node: _\b";
            std::getline(cin,temp);
            try {
                auto result = graph.DFS(temp);
                if(result.size()) {
                    cout << "Path to " << temp << " is " ;
                    for(auto node : result)
                        cout << node << "  ";
                    cout << std::endl;
                }
                else
                    cout << "There is no route between beginning of the Graph and " << temp << ".\n";
            }
            catch(std::runtime_error & e) {
                    errMsg(e.what());
                    break;
            }
            cout << "Successfull.\n";
            break;
        }
            case '5': {
                auto matrix = graph.getMatrix();
                if(!matrix.size())
                    cout << "Empty matrix.\n";
                else {
                    auto names = graph.getNamesOfNodes();
                    for(auto name : names)
                        cout << "\t" <<name;
                    cout << std::endl;
                    auto iter = names.begin();
                    for(auto row : matrix) {
                        cout << *iter++ << "\t";
                        for(auto element : row)
                            cout << element << "\t";
                        cout << std::endl;
                    }
                }
                break;
            }
            default:
                errMsg("\nWrong input pls try again.");
                break;
        }
        printMenu();
        cin.get(ch).get();
    }
}

void printMenu()
{

    std::cout<< "\n\nChoose one of the functions below: (q to quit)\n\n"
         << "==============================================\n"
         << "1)Add node.\t\t\t 2)Add path.\n"
         << "3)Find path(BFS).\t\t 4)Find path(DFS).\n"
         << "5)Show adjacent matrix.\n"
         << "Input:_\b";
}

void errMsg(std::string what)
{
    std::cerr << what << std::endl;
}
