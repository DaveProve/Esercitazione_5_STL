#pragma once

#include <iostream>
#include <PolygonalMesh.hpp>

//Richiama le funzioni della PolygonalLibrary definite in Utils.cpp
using namespace std;

namespace PolygonalLibrary {
bool ImportMesh(const string &filepath,
                Polygonal_Mesh& mesh, double scaling);

bool ImportCell0Ds(const string &filename, Polygonal_Mesh& mesh);

bool ImportCell1Ds(const string &filename, Polygonal_Mesh& mesh);

bool ImportPolygons(const string &filename, Polygonal_Mesh& mesh);

bool CheckLength(Polygonal_Mesh& mesh, double& scaling);

bool CheckArea(Polygonal_Mesh& mesh, double& scaling);

double calcArea(const Polygonal_Mesh& mesh, unsigned int index);

void printPolygonalMesh(const Polygonal_Mesh& mesh);
}
