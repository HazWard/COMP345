#pragma once

#include "../include/map.h"

class Opener{
    private:
        string fileName;
    public:
        Opener(string file);
        vector<string> readLines();
};

class Parser{
    private:
        vector<Node>* nodes;
        vector<Continent>* continents;
        Graph* graph;
    public:
        Parser(string fileName);
        Graph* getGraph();
        vector<Continent>* getContinents();
};
