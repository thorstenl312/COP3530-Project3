#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <stack>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
typedef std::chrono::high_resolution_clock Clock;

int numberOfCharities = 8409;
int numberOfEdges = 0;

class Charity {
private:

public:
    struct Node{
        Node* next;
        int weight;
        Charity* charnode;

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
    int ReturnIndexFromName(vector<Charity>& charities, string charityName);
};

void PrintCharityIndex(vector<Charity>& charities, int i)
{
    cout << "Name: " << charities[i].Name << endl;
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

int ReturnIndexFromName(vector<Charity>& charities, string charityName)
{
    int index = -1;
    transform(charityName.begin(), charityName.end(), charityName.begin(), ::tolower);
    for (int i = 0; i < charities.size(); i++)
    {
        string charName = charities[i].Name;
        transform(charName.begin(), charName.end(), charName.begin(), ::tolower);
        if (charityName == charName)
        {
            index = i;
        }
    }
    return index;
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

    for(int i = 0; i < numberOfCharities -1 ; i++) {
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
    // Generate random weights and edges
//    for(int i = 0; i < charities.size(); i++){
//        weightRandomizer(charities[i], charities);
//    }

    int colWidth = 20;
    int option = -1;
    cout << setfill('=') << setw(5*colWidth) << "=" << endl;
    cout << setfill(' ') << fixed << setw(colWidth*3.2) << "Welcome to Charity NaviGator2.0" << setw(colWidth*3.2) << setfill(' ') << fixed << endl;
    cout << setfill('=') << setw(5*colWidth) << "=" << endl;
    cout << "Welcome to Charity Navigator2.0! This program seeks to store charity data and provide answers" << endl;
    cout << "to search queries and distance traversal problems regarding the charity data." << endl;//    cout << setfill(' ') << fixed;

    /****************** Add rest of random nodes to charity graph ******************/
    int extraNodes = false;
    cout << endl;
    cout << "Insert randomly generated charities to add to 100,000 charities? (Type \"1\" for \"yes\" and \"0\" for \"no\"): ";
    cin >> extraNodes;
    string tempName = "A";
    string cat[9] = {"Animals", "Education", "Human Services", "International", "Arts, Culture, Humanities", "Health", "Religion", "Research and Public Policy", "Community Development"};
    string subCat[37] = {"Adult Education Programs and Services","Advocacy and Education","Animal Rights, Welfare, and Services","Botanical Gardens, Parks, and Nature Centers","Children's and Family Services",
                         "Community Foundations","Development and Relief Services","Diseases, Disorders, and Disciplines","Early Childhood Programs and Services","Education Policy and Reform","Environmental Protection and Conservation",
                         "Food Banks, Food Pantries, and Food Distribution","Homeless Services","Housing and Neighborhood Development","Humanitarian Relief Supplies","International Peace, Security, and Affairs","Jewish Federations","Libraries, Historical Societies and Landmark Preservation","Medical Research",
                         "Multipurpose Human Service Organizations","Museums","Non-Medical Science & Technology Research","Patient and Family Support","Performing Arts","Public Broadcasting and Media","Religious Activities","Religious Media and Broadcasting","Scholarship and Financial Support","Social and Public Policy Research",
                         "Social Services","Special Education","Treatment and Prevention Services","United Ways","Wildlife Conservation","Youth Development, Shelter, and Crisis Services","Youth Education Programs and Services","Zoos and Aquariums"};
    string state[50] = {"AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA","HI","ID","IL","IN","IA","KS","KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY","NC","ND","OH","OK","OR","PA","RI","SC","SD","TN","TX","UT","VT","VA","WA","WV","WI","WY"};

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> catRNG(0,8);
    uniform_int_distribution<std::mt19937::result_type> scoreRNG(60,100);
    uniform_int_distribution<std::mt19937::result_type> subCatRNG(0,36);
    uniform_int_distribution<std::mt19937::result_type> stateRNG(0,49);



    if (extraNodes == 1) {
        vector<Charity> randoCharities;
        cout << "You've added 91591 randomly generated charities!" << endl;
        for (int i = 0; i < 91591; i++) {
            Charity newCharity;
            newCharity.Name = "A" + to_string(i);
            newCharity.Category = cat[catRNG(rng)];
            newCharity.AScore = to_string(scoreRNG(rng));
            newCharity.Subcategory = subCat[subCatRNG(rng)];
            newCharity.State = state[stateRNG(rng)];
            newCharity.index = i+8409;
            charities.push_back(newCharity);
//            randoCharities.push_back(newCharity);
//            weightRandomizer(randoCharities[i], randoCharities);
        }
//        for (int j = 0; j < randoCharities.size(); j++)
//        {
//            charities.push_back(randoCharities[j]);
//        }
    }
    else if (extraNodes == 0)
    {
        cout << "You've skipped out on the extra charities ;-;" << endl;
    }
    else
    {
        cout << "You didn't enter \"1\" or \"0\" ;-;" << endl;
    }

//    // Generate random weights and edges
    for(int i = 0; i < charities.size(); i++){
        weightRandomizer(charities[i], charities);
    }


    while(option != 0) {
        cout << setw(colWidth) << setfill('=') << " MENU " << setw(colWidth-6) << "=" << endl;
        cout << "0. Exit\n1. Show list of charities\n2. Search for specific charity\n3. Return charities with greater than minimum score\n4. Return charities in user-chosen category\n5. Return charities in user-chosen state\n6. Dijkstra's Algorithm\n7. Bellman-Ford's Algorithm" << endl;
        cout << "Please Choose an option: " << endl;
        cin >> option;

        switch(option)
        {
            case 0:
                cout << "Thanks for using Charity NaviGator2.0! See you later alligator!!" << endl;
                break;
            case 1: {
                cout << "You selected \"1. Show list of charities\"" << endl;
                PrintCharities(charities);
                continue;
            }
            case 2: {
                string charityName;
                int numId;
                cout << "You selected \"2. Search for specific charity\"" << endl;
                cout << "Enter the name of a specific charity: ";
                cin.ignore();
                getline(cin, charityName);
                cout << charityName << endl;
                numId = ReturnIndexFromName(charities, charityName);
                if (numId != -1) {
                    PrintCharityIndex(charities, numId);
                }
                else
                    cout << "Please enter a valid charity name. Observe our list of charities for options (menu option \"1\")." << endl;
                continue;
            }
            case 3:{
                int minScore;
                cout << "You selected \"3. Return charities with greater than minimum score\"" << endl;
                cout << "Enter desired minimum score (maximum of 100): ";
                cin >> minScore;
                if (minScore >= 0 && minScore <= 100) {
                    for (int i = 1; i < charities.size(); i++) {
                        Charity a;
                        if (stoi(charities[i].AScore) >= minScore) {
                            cout << charities[i].Name << endl;
                        }
                    }
                }
                else
                    cout << "Score is not in range." << endl;
                continue;
            }
            case 4:{
                string cat;
                cout << "You selected \"4. Return charities from user-chosen category\"" << endl;
                cout << "Enter charity category: ";
                cin.ignore();
                getline(cin, cat);
                transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
                for (int i = 1; i < charities.size(); i++) {
                    string category = charities[i].Category;
                    transform(category.begin(), category.end(), category.begin(), ::tolower);
                    if (category.find(cat) != string::npos)
                    {
                        cout << charities[i].Name << endl;
                    }
                }
                continue;
            }
            case 5:{
                string state;
                cout << "You selected \"5. Return charities from user-chosen state\"" << endl;
                cout << "Enter charity state: ";
                cin >> state;
                transform(state.begin(), state.end(), state.begin(), ::tolower);
                for (int i = 0; i < charities.size(); i++) {
                    string st = charities[i].State;
                    transform(st.begin(), st.end(), st.begin(), ::tolower);
                    if(state.find(st) != string::npos){
                        cout << charities[i].Name << endl;
                    }
                }
                continue;
            }
            case 6: {
                string srcName, finalName;
                int srcIndex, finalIndex;
                cout << "You selected \"6. Dijkstra's Algorithm\"" << endl;
                cout << "This will find a list of the closest charities between two different charities in terms of distance." << endl;
                cout << "Insert name of one charity: ";
                cin.ignore();
                getline(cin, srcName);
                cout << "Insert name of one charity: ";
                getline(cin, finalName);
                if (srcIndex != -1 && finalIndex != -1) {
                    srcIndex = ReturnIndexFromName(charities, srcName);
                    finalIndex = ReturnIndexFromName(charities, finalName);
                    auto t1 = Clock::now();
                    dijkstra(charities, srcIndex, finalIndex);
                    auto t2 = Clock::now();
                    std::cout << "Delta t2-t1 (1000): "
                              << duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
                              << " nanoseconds" << '\n';
                }
                else {
                    cout
                            << "Please enter valid charity names. Observe our list of charities for options (menu option \"5\")."
                            << endl;
                    cout << srcIndex << " : " << finalIndex << endl;
                }
                continue;
            }
            case 7: {
                string srcName, finalName;
                int srcIndex, finalIndex;
                cout << "You selected \"7. Bellman-Ford's Algorithm\"" << endl;
                cout
                        << "This will find a list of the closest charities between two different charities in terms of distance."
                        << endl;
                cout << "Insert name of one charity: ";
                cin.ignore();
                getline(cin, srcName);
                cout << "Insert name of one charity: ";
                getline(cin, finalName);
                if (srcIndex != -1 && finalIndex != -1) {
                    srcIndex = ReturnIndexFromName(charities, srcName);
                    finalIndex = ReturnIndexFromName(charities, finalName);
                    auto t1 = Clock::now();
                    BellmanFord(srcIndex, charities, finalIndex);
                    auto t2 = Clock::now();
                    std::cout << "Delta t2-t1 (1000): "
                              << duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
                              << " nanoseconds" << '\n';
                } else
                    cout << "Please enter valid charity names. Observe our list of charities for options (menu option \"5\")." << endl;
                continue;
            }
            default:
                cout << "Please select a valid option!" << endl;
                continue;
        }

    }

    return 0;
}