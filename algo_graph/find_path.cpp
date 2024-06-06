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

void print_plan(std::vector<Linie> lines)
{
        for (int i = 0; i < lines.size(); ++i)
        {
            std::cout << lines.at(i).line << std::endl;
            for (int j = 0; j < lines.at(i).stations.size(); ++j)
            {
				std::cout << lines.at(i).stations.at(j).station_name << std::endl;
            }
            std::cout << std::endl << "___________________________________________" << std::endl;
        }
}

int main(int argument_count, char* argument_vector[])
{
    using namespace std;

    if (argument_count != 4)
    {
        cout << "Sie muessen folgende informationen uebergeben in folgendem format: Dateiname.txt Start Ziel" << endl;
    }
    else
    {
		string filename = argument_vector[1];
		string start_point = argument_vector[2];
		string end_point = argument_vector[3];


        cout << filename << endl;
        cout << start_point << endl;
        cout << end_point << endl;

		//bereits markierte Knoten müssen vermerkt werden, mögl. bool
		bool prev_visited = false;

		//????
		std::vector<Linie> lines;

		read_file(filename, lines);
		print_plan(lines);
    }


    return 0;
}

