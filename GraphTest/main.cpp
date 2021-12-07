#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <list>
using namespace std;

class Graph {
public:
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

    string Name;
    string Category;
    string State;
    string AScore;
    string Subcategory;
    Node* head;

};
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