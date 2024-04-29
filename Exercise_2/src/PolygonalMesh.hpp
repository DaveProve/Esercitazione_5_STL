#pragma once
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace Eigen;

struct Polygonal_Mesh
{

    unsigned int NumberCell0D = 0; // Inizializzazione del numero totale di celle 0D nella mesh
    vector<unsigned int> Cell0DId = {}; // Degli ID delle celle 0D
    vector<Vector2d> Cell0DCoordinates = {}; // Delle coordinate delle celle 0D
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; // Della mappa che associa a un marcatore un elenco di ID delle celle 0D

    // Similmente per le celle 1D
    unsigned int NumberCell1D = 0;
    vector<unsigned int> Cell1DId = {};
    vector<Vector2i> Cell1DVertices = {};
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    // E per le 2D (poligonali)
    unsigned int NumberCell2D = 0;
    vector<unsigned int> Cell2DId = {};
    vector<vector<unsigned int>> Cell2DVertices = {};
    vector<vector<unsigned int>> Cell2DEdges = {}; // Vettore dei lati delle celle 2D
};
