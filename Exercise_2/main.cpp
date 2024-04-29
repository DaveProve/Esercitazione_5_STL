#include <iostream>
#include <Utils.hpp>

using namespace std;
using namespace PolygonalLibrary;

int main()
{
    Polygonal_Mesh mesh;
    double scaling = 10; //Definiamo un fattore di scaling

    string filepath = "C:/Users/davep/PCS2024_Exercises/Esercitazione_5_STL/Exercise_2/PolygonalMesh";
    if(!ImportMesh(filepath, mesh, scaling))
    {
        return 1;
    }
    cout << "Mesh valida!" << endl;

    printPolygonalMesh(mesh); //Stampiamo le info della mesh poligonale

    return 0;
}

