#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <stack>
#include <algorithm>
using namespace std;

int numberOfCharities = 8409; //Zoe said "Fix later"
int numberOfEdges = 0;

class Charity {
private:

public:
    struct Node{
        Node* next;
        int weight;
        Charity* charnode;
        //string charname;

        Node() {
            next = nullptr;
            weight = 0;
        }
        Node(int weight) {
            this->weight = weight;
        }
    };
    string Name;
    string Category;
    string State;
    string AScore;
    string Subcategory;
    int index;
    int numberOfConnections;
    Node* head;

    Charity(){
        head = nullptr;
        Name = "empty";
        Category = "empty";
        State = "empty";
        AScore = "empty";
        Subcategory = "empty";
        numberOfConnections = 0;
    };
    void PrintCharityIndex(vector<Charity>& charities, int i);
    void PrintCharities(vector<Charity>& charities);
};

void PrintCharityIndex(vector<Charity>& charities, int i)
{
    cout << "Name (Index Number): " << charities[i].Name << " (" << charities[i].index << "):" << endl;
    cout << setw(5) << "Category: " << charities[i].Category << endl;
    cout << setw(5) << "Subcategory: " << charities[i].Subcategory << endl;
    cout << setw(5) << "State: " << charities[i].State << endl;
    cout << setw(5) << "Score: " << charities[i].AScore << endl;
}
void PrintCharities(vector<Charity>& charities)
{
    for (int i = 0; i < charities.size(); i++)
    {
        PrintCharityIndex(charities, i);
        cout << "------------------------------------------------------------------------" << endl;
    }
}

// CSV Reading from stackoverflow user sastanin (https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c)
enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
};

vector<string> readCSVRow(const string &row) {
    CSVState state = CSVState::UnquotedField;
    vector<string> fields {""};
    size_t i = 0; // index of the current field
    for (char c : row) {
        switch (state) {
            case CSVState::UnquotedField:
                switch (c) {
                    case ',': // end of field
                        fields.push_back(""); i++;
                        break;
                    case '"': state = CSVState::QuotedField;
                        break;
                    default:  fields[i].push_back(c);
                        break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                    case '"': state = CSVState::QuotedQuote;
                        break;
                    default:  fields[i].push_back(c);
                        break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                    case ',': // , after closing quote
                        fields.push_back(""); i++;
                        state = CSVState::UnquotedField;
                        break;
                    case '"': // "" -> "
                        fields[i].push_back('"');
                        state = CSVState::QuotedField;
                        break;
                    default:  // end of quote
                        state = CSVState::UnquotedField;
                        break; }
                break;
        }
    }
    return fields;
}
vector<vector<string>> readCSV(ifstream &in) {
    vector<vector<string>> table;
    string row;
    while (!in.eof()) {
        getline(in, row);
        if (in.bad() || in.fail()) {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}
void weightRandomizer(Charity& a, vector<Charity>& v){
    int edges = 5;
    for(int i = 0; i < edges; i++) {
        Charity::Node* b = new Charity::Node;
        int randomCharity = rand() % numberOfCharities;
        int randomWeight = rand() % 1000;
        //add into the first while adding into the other
        b->charnode = &v[randomCharity];
        b->weight = randomWeight;
        b->next = nullptr;
        Charity::Node* temp = a.head;
        bool found = false;
        if(a.head == nullptr){
            a.head = b;
            found = true;
        }
        else{
            while(temp->next != nullptr) {
                if (temp->charnode->Name == v[randomCharity].Name || temp->charnode->Name == a.Name){
                    found = true;
                    break;
                }
                temp = temp->next;
            }
        }
        if(!found){
            temp->next = b;
        }
        Charity::Node* c = new Charity::Node;
        //add into the first while adding into the other
        c->charnode = &a;
        c->weight = randomWeight;
        c->next = nullptr;
        found = false;
        temp = v[randomCharity].head;
        if(temp == nullptr){
            v[randomCharity].head = c;
            found = true;
        }
        else{
            while(temp->next != nullptr) {
                if (temp->charnode->Name == v[randomCharity].Name || temp->charnode->Name == a.Name){
                    found = true;
                    break;
                }
                temp = temp->next;
            }
        }
        if(!found){
            temp->next = c;
        }

    }
}
void BellmanFord(int src, vector<Charity>& graph, int dest) {
    int dist[numberOfCharities];
    int p[numberOfCharities];
    for(int i = 0; i < numberOfCharities; i++) {
        dist[i] = INT_MAX;
    }

    dist[src] = 0;

    for(int i = 0; i < numberOfCharities; i++) {
        for(int j = 0; j < numberOfCharities; j++) {
            int source = graph[j].index;
            Charity::Node* temp = graph[j].head;
            while(temp != nullptr) {
                int dest = temp->charnode->index;
                int weight = temp->weight;
                if(dist[source] != INT_MAX && dist[source] + weight < dist[dest]) {
                    dist[dest] = dist[source] + weight;
                    p[dest] = source;
                }
                temp = temp->next;
            }
        }
    }
    stack<string> st;
    int curr = dest;
    st.push(graph[dest].Name);
    st.push(graph[p[curr]].Name);
    curr = p[curr];
    while(src != curr){
        st.push(graph[p[curr]].Name);
        curr = p[curr];
    }
    cout << st.top();
    st.pop();
    while(!st.empty()){
        cout << ", " << st.top();
        st.pop();
    }
    cout << endl << endl;
}
void dijkstra(vector<Charity>& v, int src, int dest){
    int d[numberOfCharities];
    int p[numberOfCharities];
    bool s[numberOfCharities];
    for(int i = 0; i < numberOfCharities; i++){
        d[i] = INT_MAX;
        s[i] = false;
        p[i] = -1;
    }
    d[src] = 0;
    p[src] = src;
    for(int i = 0; i < numberOfCharities - 1; i++){
        int min = INT_MAX;
        int min_index;
        for(int j = 0; j < numberOfCharities; j++){
            if(s[j] == false && d[j] <= min){
                min = d[j];
                min_index = j;
            }
        }
        s[min_index] = true;
        Charity::Node* temp = v[min_index].head;
        while(temp != nullptr){
            if(!s[temp->charnode->index] && temp->weight && d[min_index] != INT_MAX && d[min_index] + temp->weight < d[temp->charnode->index]){
                d[temp->charnode->index] = temp->weight + d[min_index];
                p[temp->charnode->index] = min_index;
            }
            temp = temp->next;
        }
    }
    stack<string> st;
    int curr = dest;
    st.push(v[dest].Name);
    st.push(v[p[curr]].Name);
    curr = p[curr];
    while(src != curr){
        st.push(v[p[curr]].Name);
        curr = p[curr];
    }
    cout << st.top();
    st.pop();
    while(!st.empty()){
        cout << ", " << st.top();
        st.pop();
    }
    cout << endl << endl;
    /*printf("Vertex  Distance from Source\n");
    for(int i = 0; i < numberOfCharities; i++) {
        if(d[i] != INT_MAX)
            printf("%d\t\t%d\n", i, d[i]);
    }*/
}

    stack<string> st;
    int curr = dest;
    st.push(graph[dest].Name);
    st.push(graph[p[curr]].Name);
    curr = p[curr];
    while(src != curr){
        st.push(graph[p[curr]].Name);
        curr = p[curr];
    }
    cout << st.top();
    st.pop();
    while(!st.empty()){
        cout << ", " << st.top();
        st.pop();
    }
    cout << endl << endl;

    /*printf("Vertex  Distance from Source\n");
    for(int i = 0; i < numberOfCharities; i++) {
        if(dist[i] != INT_MAX)
            printf("%d\t\t%d\n", i, dist[i]);
    }*/

>>>>>>> Stashed changes
}

int main()
{
    vector<Charity> charities;

    string filePath = "CLEAN_charity_data.csv";
    vector<vector<string>> charityData;

    ifstream inFile(filePath);
    if (inFile.is_open()) {
        charityData = readCSV(inFile);
    }

    for (int i = 1; i < charityData.size(); i++)
    {
        Charity a;
        a.index = i-1;
        for (int j = 0; j < charityData[i].size(); j++)
        {
            if(j == 0) {
                a.AScore = charityData[i][j];
            }
            if(j == 1) {
                a.Category = charityData[i][j];
            }
            if(j == 14) {
                a.Name = charityData[i][j];
            }
            if(j == 17) {
                a.State = charityData[i][j];
            }
            if(j == 18) {
                a.Subcategory = charityData[i][j];
            }
        }
        charities.push_back(a);
    }
    for(int i = 0; i < charities.size(); i++){
        weightRandomizer(charities[i], charities);
    }

//KOINDA NEEDA FIX THIS HERE OOP
//    /****************** Add rest of random nodes to charity graph ******************/
//    bool extraNodes = false;
//    cout << "use 91591 random nodes? (type 1 for yes and 0 for no)" << endl;
//    cin >> extraNodes;
//    if (extraNodes)
//    {
//        for (int i = 0; i < 91591; i++) {
//            Charity newCharity;
//            Charity::Node *b = new Charity::Node;
//            int randomWeight = rand() % 1000;
//            b->weight = randomWeight;
//            b->next = nullptr;
//            newCharity.head = b;
//            charities.push_back(newCharity);

//    for(int i = 0 ; i < charities.size(); i++) {
//        Charity::Node* temp = charities[i].head;
//        cout << charities[i].index << " : ";
//        while(temp->next != nullptr) {
//            cout << temp->charnode->Name << ", ";
//            temp = temp->next;
//        }
//        cout << endl;
//    }

//    for (int i = 0; i < charities.size(); i++) {
//        Charity::Node* temp = charities[i].head;
//        cout << "Charity " << charities[i].Name << " connected to" << endl;
//        while(temp != nullptr){
//            cout << temp->weight << ":" << string(temp->charnode->Name) << " ";
//            temp = temp->next;
//        }
//        cout << endl << endl;
//    }

    int colWidth = 20;
    int option = -1;
    cout << setfill('=') << setw(5*colWidth) << "=" << endl;
    cout << setfill(' ') << fixed << setw(colWidth*3.2) << "Welcome to Charity NaviGator2.0" << setw(colWidth*3.2) << setfill(' ') << fixed << endl;
    cout << setfill('=') << setw(5*colWidth) << "=" << endl;
    cout << "Welcome to Charity Navigator2.0! This program seeks to store charity data and provide answers" << endl;
    cout << "to search queries and distance traversal problems regarding the charity data." << endl;//    cout << setfill(' ') << fixed;


    while(option != 0) {
        cout << setw(colWidth) << setfill('=') << " MENU " << setw(colWidth-6) << "=" << endl;
        cout << "0. Exit\n1. Dijkstra's Algorithm\n2. Bellman-Ford's Algorithm\n3. etc." << endl;
        cout << "Please Choose an option: " << endl;
        cin >> option;

        switch(option)
        {
            case 0:
                cout << "Thanks for using Charity NaviGator2.0! See you later alligator!!" << endl;
                break;
            case 1:
                int src;
                int final;
                cout << "You selected \"1. Dijkstra's Algorithm\"" << endl;
                cout << "insert a starting and final charity index";
                cin >> src;
                cin >> final;
                dijkstra(charities, src, final);
                continue;
            case 2:
                cout << "two" << endl;
                BellmanFord(0, charities, 8408);
                continue;
            case 3:{
                int minScore;
                cout << "three" << endl;
                cin >> minScore;
                for (int i = 1; i < charities.size(); i++) {
                    Charity a;
                    if(stoi(charities[i].AScore) >= minScore){
                        cout << charities[i].Name << endl;
                    }
                }
                continue;}
            case 4:{
                string cat;
                cout << "four" << endl;
                cin >> cat;
                for (int i = 1; i < charities.size(); i++) {
                    Charity a;
                    string category = a.Category;
                    transform(category.begin(), category.end(), category.begin(), ::tolower);
                    if (category.find(cat) != string::npos) {
                        cout << charities[i].Name << endl;
                    }
                }
                continue;}
                cout << "You selected \"2. Bellman-Ford's Algorithm\"" << endl;
                BellmanFord(charities[0], charities);
                continue;
            case 3:
                cout << "You selected \"3. Show List of Charities\"" << endl;
                PrintCharities(charities);
                continue;
                // do smth else
                continue;
            case 4:
                int idNum;
                cout << "You selected \"4. Search for specific charity\"" << endl;
                cout << "Enter the ID-number of a specific charity: ";
                cin >> idNum;
                PrintCharityIndex(charities, idNum);
                continue;
            default:
                cout << "Please select a valid option!" << endl;
                continue;
        }

    }

    return 0;
}

