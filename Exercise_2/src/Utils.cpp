#include <string>
#include <Utils.hpp>


using namespace std;

namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                Polygonal_Mesh& mesh, double scaling)
{
    // Importa i dati dalla directory fornita e il file CSV "Cell0Ds.csv" in un oggetto mesh.
    // Se l'importazione non riesce, restituisce false.
    // Se l'importazione ha successo, stampa le informazioni sui marcatori delle cellule 0D.
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else  //Se ha successo stampa le varie informazioni sulle Celle 0D
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    //In modo analogo anche per le celle 1D
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    // E per le celle poligonali
    cout << "Test sull'importazione dei poligoni: ";
    if (!ImportPolygons(filepath + "/Cell2Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        // Verifica la correttezza dei lati
        for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];

            for (unsigned int e = 0; e < edges.size(); e++) //Per ogni lato ottiene i vertici
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];
                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin); //Verifica il vertice d'origine
                if (findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end); //E di fine
                if (findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }
            }
        }
        cout << "Poligoni importati correttamente" << endl << endl;
    }
    //Test
    cout << "Test sulla lunghezza: ";
    if(!CheckLength(mesh, scaling))
    {
        return false;
    }
    cout << "Lunghezza dei lati valida." << endl << endl;

    cout << "Test Area: ";
    if(!CheckArea(mesh, scaling))
    {
        return false;
    }
    cout << "L'area dei poligoni risulta valida." << endl << endl;

    return true;

}

bool ImportCell0Ds(const string &filename,
                   Polygonal_Mesh& mesh)
{

    ifstream file;  //Apre il file
    file.open(filename);

    if(file.fail())   // Dà errore se non si riesce a visualizzarlo
        return false;

    list<string> listLines;  //Legge il contenuto riga per riga e lo memorizza in una lista
    string line;
    while (getline(file, line))
        listLines.push_back(line);


    listLines.pop_front(); //RImuove l'intestazione

    mesh.NumberCell0D = listLines.size(); //Determina il numero di celle 0D

    if (mesh.NumberCell0D == 0)  //Controlla se non ci sono celle 0D
    {
        cerr << "Non ci sono celle 0D" << endl;
        return false;
    }
    //Ridimensiona le strutture in base al numero delle celle
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string cell;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        getline(converter,cell , ';');  //Estrae gli ID i marker e le coordinate e li converte in interi, double e double rispettivamente
        id = stoi(cell);
        getline(converter, cell, ';');
        marker = stoi(cell);
        getline(converter, cell, ';');
        coord(0) = stod(cell);
        getline(converter, cell, ';');
        coord(1) = stod(cell);

        mesh.Cell0DId.push_back(id);   // Aggiunge l'ID e le coordinate della cella 0D alle rispettive strutture dati nella mesh
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0) // Se il marcatore non è zero, aggiunge l'ID alla mappa dei marcatori di cella 0D.
        {
            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
                mesh.Cell0DMarkers.insert({marker, {id}});
            else
                mesh.Cell0DMarkers[marker].push_back(id);
        }

    }
    file.close();
    return true;
}
// Stesso procedimento per le 1D
bool ImportCell1Ds(const string &filename,
                   Polygonal_Mesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "Non ci sono celle 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string cell;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        getline(converter,cell , ';');
        id = stoi(cell);
        getline(converter, cell, ';');
        marker = stoi(cell);
        getline(converter, cell, ';');
        vertices(0) = stod(cell);
        getline(converter, cell, ';');
        vertices(1) = stod(cell);
        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
               mesh.Cell1DMarkers.insert({marker, {id}});
            else
               mesh.Cell1DMarkers[marker].push_back(id);
        }
    }

    file.close();

    return true;
}
// E per le poligonali
bool ImportPolygons(const string &filename, Polygonal_Mesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "NOn ci sono celle 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string cell;

        unsigned int id;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
        getline(converter, cell, ';');
        id = stoi(cell);
        getline(converter, cell, ';');
        getline(converter, cell, ';');
        unsigned int numvert= stoi(cell);
        for(unsigned int i=0; i<numvert; i++)
        {
            getline(converter, cell, ';');
            unsigned int v=stod(cell);
            vertices.push_back(v);
        }
        getline(converter, cell, ';');
        unsigned int numedges = stoi(cell);
        for(unsigned int i=0;i<numedges;i++)
        {
            getline(converter, cell, ';');
            unsigned int e=stod(cell);
            edges.push_back(e);
        }

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();

    return true;
}

bool CheckLength(Polygonal_Mesh& mesh, double &scaling)
{
    // Definisco la precisione
    double eps = numeric_limits<double>::epsilon()*scaling;
    for(const auto& points : mesh.Cell1DVertices) // Scorro il vettore che contiene il numero dei vertici associati al lato
    {
        Vector2d c1 = mesh.Cell0DCoordinates[points(0)]; // Ricavo le coordinate dei singoli vertici e controllo la distanza reciproca
        Vector2d c2 = mesh.Cell0DCoordinates[points(1)];
        double dist = (c1 - c2).norm();
        if(dist < eps)
        {
            cerr << "Ci sono dei lati di lunghezza nulla." << endl;
            return false;
        }
    }
    return true;
}

bool CheckArea(Polygonal_Mesh& mesh, double &scaling)
{
    double eps = numeric_limits<double>::epsilon() * scaling;  //Definiamo una tolleranza eps

    for(const auto& edges : mesh.Cell2DEdges)
    {
        unsigned int counter_zeros = 0;

        for(unsigned int i = 0; i < edges.size(); i++)
        {
            Vector2i p_lato_a = mesh.Cell1DVertices[edges[i]];             //Prendiamo il lato a di vertici A e B
            Vector2d p_A = mesh.Cell0DCoordinates[p_lato_a[0]];
            Vector2d p_B = mesh.Cell0DCoordinates[p_lato_a[1]];
            Vector2d length_a = p_B - p_A;

            Vector2i p_lato_b;
            if(i != edges.size()-1)
                p_lato_b = mesh.Cell1DVertices[edges[i+1]]; // Se non siamo sull'ultimo lato, p_lato_b contiene gli indici dei vertici che definiscono il lato b.
            else
                p_lato_b = mesh.Cell1DVertices[edges[0]]; // Se siamo sull'ultimo lato, p_lato_b contiene gli indici dei vertici che definiscono il primo lato.

            Vector2d p_C = mesh.Cell0DCoordinates[p_lato_b[0]];   // E ora il lato b di vertici C e D
            Vector2d p_D = mesh.Cell0DCoordinates[p_lato_b[1]];
            Vector2d length_b = p_D - p_C;

            double prod = length_a.x() * length_b.y() - length_a.y() * length_b.x(); //Facciamo il prodotto scalare

            if(abs(prod) < eps)
            {
                counter_zeros++;
            }
        }

        if(counter_zeros == edges.size())  //Asseriamo se ci sono poligoni con area nulla
        {
            cerr << "Ci sono poligoni con area nulla." << endl;
            return false;
        }
    }
    return true;
}

double calcArea(const Polygonal_Mesh& mesh, unsigned int index)
{   //Calcoliamo l'area attraverso la formula dell'area di un poligono tramite prodotto vettoriale
    const vector<unsigned int>& vertices = mesh.Cell2DVertices[index];
    double area = 0.0;

    for (unsigned int i = 0; i < vertices.size(); ++i) {
        unsigned int j = (i + 1) % vertices.size();
        unsigned int vertexA = vertices[i];
        unsigned int vertexB = vertices[j];
        Vector2d coordA = mesh.Cell0DCoordinates[vertexA];
        Vector2d coordB = mesh.Cell0DCoordinates[vertexB];
        area += coordA.x() * coordB.y() - coordB.x() * coordA.y();
    }
    area = abs(area) / 2.0;

    return area;
}

void printPolygonalMesh(const Polygonal_Mesh& mesh)
{
    //Stampiamo i dati relativi ai poligoni della mesh
    cout << "Numero totale di poligoni 2D: " << mesh.NumberCell2D << endl;

    for (unsigned int i = 0; i < mesh.NumberCell2D; ++i) {
        cout << "Poligono " << i << ":" << endl;
        cout << "   ID: " << mesh.Cell2DId[i] << endl;
        cout << "   Vertici: ";
        for (unsigned int vertice : mesh.Cell2DVertices[i]) {
            cout << vertice << " ";
        }
        cout << endl;
        cout << "   Bordi: ";
        for (unsigned int bordo : mesh.Cell2DEdges[i]) {
            cout << bordo << " ";
        }
        cout << endl;

        cout << "   Area: " << calcArea(mesh, i) << endl;
    }
}
}
