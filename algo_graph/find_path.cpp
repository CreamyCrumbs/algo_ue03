
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//Werte je Station
struct Station
{
    std::string station_name;
    int travel_duration;
};

class Linie
{
public:
    std::string line;
    //Liste der Stationen:
    std::vector<Station> stations;

    //Stationen zur Linie hinzufügen:
    void add_station(const std::string& station_name, int travel_duration) 
    {
        stations.push_back({ station_name, travel_duration });
    }
};

void read_file(const std::string& filename, std::vector<Linie>& lines)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::cerr << "Fehler beim oeffnen der Datei!" << std::endl;
        return;
    }
    
   

}

int main()
{
    //bereits markierte Knoten müssen vermerkt werden, mögl. bool
    bool prev_visited = false;

    //????
    std::vector<Linie> lines;

}

