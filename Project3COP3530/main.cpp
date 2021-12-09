#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <set>
#include <queue>
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

};

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
void BellmanFord(Charity src, vector<Charity>& graph) {
    int dist[numberOfCharities];

    for(int i = 0; i < numberOfCharities; i++) {
        dist[i] = INT_MAX;
    }

    dist[src.index] = 0;

    for(int i = 0; i < numberOfCharities; i++) {
        for(int j = 0; j < numberOfCharities; j++) {
            int source = graph[i].index;
            Charity::Node* temp = graph[i].head;
            while(temp->next != nullptr) {
                int dest = temp->charnode->index;
                int weight = temp->weight;
                if(dist[source] != INT_MAX && dist[source] + weight < dist[dest]) {
                    dist[dest] = dist[source] + weight;
                }
                temp = temp->next;
            }
        }
    }



    printf("Vertex  Distance from Source\n");
    for(int i = 0; i < numberOfCharities; i++) {
        if(dist[i] != INT_MAX)
            printf("%d\t\t%d\n", i, dist[i]);
    }

}
vector<int> dijkstra(vector<Charity>& v, int src){
    /*int d[numberOfCharities];
    int p[numberOfCharities];
    for(int i = 0; i < numberOfCharities; i++){
        d[i] = INT_MAX;
        p[i] = -1
    }
    d[src] = 0;
    set<int> s;
    queue<int> q;
    for(int i = 0; i < numberOfCharities; i++){
        q.push(i);
    }
    while(!q.empty()){

    }*/
    set<int> s;
    set<int> vs;
    int d[numberOfCharities];
    int p[numberOfCharities];
    s.insert(src);
    int min = INT_MAX;
    int minIndex;
    d[0] = 0;
    for(int i = 1; i < numberOfCharities; i++){
        vs.insert(i);
    }
    for(int i = 1; i < numberOfCharities; i++){
        d[i] = -1;
    }
    Charity::Node* temp = v[src].head;
    for(int i = 0; i < numberOfCharities; i++){
        p[i] = src;
        if(temp == nullptr) continue;
        if(i < v[src].numberOfConnections && temp->weight != 0 && temp->weight < INT_MAX && d[temp->charnode->index] == -1){
            d[temp->charnode->index] = temp->weight;
            i--;
        }
        else if(d[i] == -1){
            d[i] = INT_MAX;
        }
        temp = temp->next;
    }
    while(!vs.empty()){
        for(auto iter = vs.begin(); iter != vs.end(); ++iter){
            if(d[*iter] < min){
                min = d[*iter];
                minIndex = *iter;
            }
        }
        s.insert(minIndex);
        vs.erase(minIndex);
        temp = v[minIndex].head;
        for(int j = 0; j < v[minIndex].numberOfConnections; j++){
            if(d[minIndex] + temp->weight < d[temp->charnode->index]){
                d[temp->charnode->index] = d[minIndex] + temp->weight;
                p[temp->charnode->index] = minIndex;
            }
            temp = temp->next;
        }
        min = INT_MAX;
    }
    vector<int> v1;
    cout << endl;
    for(int i = 0; i < numberOfCharities; i++){
        v1.push_back(d[i]);
    }
    return v1;
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

    BellmanFord(charities[0], charities);

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
                ;            case 1:
                cout << "one" << endl;
                // do dijkstra's algo
                continue;
            case 2:
                cout << "two" << endl;
                // do bellman-ford algo
                continue;
            case 3:
                cout << "three" << endl;
                // do smth else
                continue;
            default:
                cout << "Please select a valid option!" << endl;
                continue;
        }

    }

    return 0;
}