
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip> //std::quoted

struct Station
{
    std::string station_name;
    int travel_duration;
};

class Linie
{
public:
    std::string line;
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
    
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string line_name;

        if (std::getline(iss, line_name, ':'))
        {
            Linie new_line;
            new_line.line = line_name;

            std::string station_name;
            int travel_duration;

            while(iss >> std::quoted(station_name) >> travel_duration) //quoted für ""
            {
                new_line.add_station(station_name, travel_duration);
            }
            lines.push_back(new_line);
        }
    }
    infile.close();

}

int main(int argc, char* argv[])
{
    //bereits markierte Knoten müssen vermerkt werden, mögl. bool
    bool prev_visited = false;

    //????
    std::vector<Linie> lines;
    std::string filename;
    std::string start;
    std::ifstream start(argv[1]);
    std::string ziel;
    std::ifstream ziel(argv[2]);


    read_file(filename, lines);

}

