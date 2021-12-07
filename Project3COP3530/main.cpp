#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <list>
using namespace std;

int numberOfCharities = 8000; //Zoe said "Fix later"
struct Node{
    Node* next;
    int weight;
    // Charity* charnode;
    string charname;

    Node() {
        next = nullptr;
        weight = 0;
    }
    Node(int weight) {
        this->weight = weight;
    }
};
class Charity {
private:

public:

    string Name;
    string Category;
    string State;
    string AScore;
    string Subcategory;
    Node* head;

    Charity(){
        head = nullptr;
        Name = "empty";
        Category = "empty";
        State = "empty";
        AScore = "empty";
        Subcategory = "empty";
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
void weightRandomizer(Charity a, vector<Charity>& v){
    int edges = 5;
//    for(int i = 0; i < edges; i++){
//        int randomCharity = rand() % numberOfCharities;
//        pair<Charity, float> p;
//        p.first = v[randomCharity];
//        p.second = rand() % 1000;
//        a.adj.push_back(p);
//        p.first = a;
//        v[randomCharity].adj.push_back(p);
//    }
//
    int count = 0;
    for(int i = 0; i < edges; i++) {
        Node* b;
        int randomCharity = rand() % numberOfCharities;
        int randomWeight = rand() % 1000;
        //add into the first while adding into the other
        cout << v[randomCharity].Name << endl;
        b->charname = v[count].Name;
        count++;
        b->weight = randomWeight;
        cout << v[randomCharity].Name << endl;
        if(a.head == nullptr) {
            a.head = b;
        }
        else {
            Node* temp = a.head;
            b->next = temp;
            a.head = b;

//            b->next = a.head;
//            a.head = b;
        }

        Node* c;
        //add into the first while adding into the other
        c->charname = a.Name;
        c->weight = randomWeight;
        if(v[count].head == nullptr) {
            v[count].head = c;
        }
        else {
            Node* temp = v[count].head;
            c->next = temp;
            v[count].head = c;

//           c->next = v[randomCharity].head;
//            v[randomCharity].head = c;
        }


    }
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
    for (int i = 0; i < charities.size(); i++) {
        weightRandomizer(charities[i], charities);
    }
    return 0;

}