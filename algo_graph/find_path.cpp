#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip> //std::quoted
#include <tuple>

struct Station
{
    std::string station_name;
    int travel_duration;
};

struct station_node
{
    Station station;
    std::shared_ptr<station_node> next_station = nullptr;
    std::shared_ptr<station_node> prev_station = nullptr;

    int time_to_next_station = 0;
    int time_to_prev_station = 0;

    bool visited = false;
    bool is_kreuzung = false;
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

            while (iss >> std::quoted(station_name) >> travel_duration) //quoted für ""
            {
                new_line.add_station(station_name, travel_duration);
            }
            if (iss.eof())
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
            std::cout << lines.at(i).stations.at(j).station_name << " Travel Time: ";
            std::cout << lines.at(i).stations.at(j).travel_duration << std::endl;

        }
        std::cout << std::endl << "___________________________________________" << std::endl;
    }
}

struct end_station_paar
{
    std::string linie;
    std::shared_ptr<station_node> head = nullptr;
    std::shared_ptr<station_node> tail = nullptr;
    std::vector<std::shared_ptr<station_node>> kreuzungen;
};

class netz
{
public:
    std::vector<end_station_paar> anfang_end_stationen;

    std::vector<std::shared_ptr<station_node>> get_kreuzungen(std::string station_name, std::string linie_name)
    {
        std::vector<std::shared_ptr<station_node>> kreuzungen;
        for (int i = 0; i < this->anfang_end_stationen.size(); ++i)
        {
            if (linie_name == this->anfang_end_stationen.at(i).linie)
            {
                continue;
            }
            std::shared_ptr<station_node> current = this->anfang_end_stationen.at(i).head;
            while (current != nullptr)
            {
                if (current->station.station_name == station_name)
                {
                    current->is_kreuzung = true;
                    kreuzungen.push_back(current);
                }
                current = current->next_station;
            }
        }
        return kreuzungen;
    }

    void print_route(std::vector<std::tuple<int, std::string, std::shared_ptr<station_node>>> pfad)
    {
        std::cout << "Route:" << std::endl;
        int distance = std::get<0>(pfad.back());
        std::string route = std::get<1>(pfad.back());
        std::cout << route << std::endl;
        std::cout << distance << std::endl;
        return;
    }

    void create_netz(std::vector <Linie>& lines)
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            end_station_paar current_line;
            current_line.linie = lines[i].line;
            std::shared_ptr<station_node> prev_station = nullptr;

            for (int j = 0; j < lines[i].stations.size(); ++j)
            {
                std::shared_ptr<station_node> current_station = std::make_shared<station_node>();
                current_station->prev_station = prev_station;
                current_station->station = lines[i].stations.at(j);

                if (prev_station != nullptr)
                {
                    current_station->time_to_prev_station = current_station->prev_station->time_to_next_station;
                    current_station->prev_station->next_station = current_station;
                }

                current_station->time_to_next_station = current_station->station.travel_duration;

                if (prev_station == nullptr)
                {
                    current_line.head = current_station;
                }
                prev_station = current_station;


            }
            anfang_end_stationen.push_back(current_line);
        }

        for (int i = 0; i < anfang_end_stationen.size(); ++i)
        {
            std::shared_ptr<station_node> current = anfang_end_stationen.at(i).head;

            while (current != nullptr)
            {
                std::vector<std::shared_ptr<station_node>> kreuzungen = this->get_kreuzungen(current->station.station_name, anfang_end_stationen.at(i).linie);

                if (kreuzungen.size() > 0)
                {
                    for (int j = 0; j < kreuzungen.size(); ++j)
                    {
                        anfang_end_stationen.at(i).kreuzungen.push_back(kreuzungen.at(j));
                    }
                }

                current = current->next_station;
            }
        }
    }

    void print_lines()
    {
        for (int i = 0; i < anfang_end_stationen.size(); ++i)
        {
            std::cout << "_________________Current Line: " << anfang_end_stationen.at(i).linie << "_________________" << std::endl;

            std::shared_ptr<station_node> current = anfang_end_stationen.at(i).head;

            while (current != nullptr)
            {
                std::cout << current->station.station_name << " -> ";
                current = current->next_station;
            }

            if (anfang_end_stationen.at(i).kreuzungen.size() > 0)
            {
                std::cout << std::endl << std::endl << "Kreuzungen: " << std::endl;
                for (int j = 0; j < anfang_end_stationen.at(i).kreuzungen.size(); ++j)
                {
                    std::cout << anfang_end_stationen.at(i).kreuzungen.at(j)->station.station_name << std::endl;
                }
            }

            std::cout << std::endl << std::endl;
        }
    }

    std::tuple<std::string, std::shared_ptr<station_node>> get_station(std::string station_name)
    {
        for (int i = 0; i < this->anfang_end_stationen.size(); ++i)
        {
            std::shared_ptr<station_node> current = this->anfang_end_stationen.at(i).head;
            while (current != nullptr)
            {
                if (current->station.station_name == station_name)
                {
                    return { this->anfang_end_stationen.at(i).linie, current };
                }
                current = current->next_station;
            }
        }
        return { "", nullptr };
    }

    void dijkstra(std::string start_point, std::string end_point)
    {
        auto info = this->get_station(start_point);
        std::string linie = std::get<0>(info);
        std::shared_ptr<station_node> current_station = std::get<1>(info);
        current_station->visited = true;
        std::vector<std::tuple<int, std::string, std::shared_ptr<station_node>>> pfad;
        //pfad.push_back({ 0, " ", current_station });
        std::string temp_pfad;// = current_station->station.station_name + " -> ";
        int temp_distance = 0;

        int temp_index = 0;
        while (temp_index < 100 && current_station && current_station->station.station_name != end_point)
        {
            int min = INT_MAX;
            ++temp_index;
            int index = 0;
            if (pfad.size() > 0)
            {
                for (int i = 0; i < pfad.size(); ++i)
                {
                    if (min > std::get<0>(pfad.at(i)))
                    {
                        if (std::get<2>(pfad.at(i))->next_station->visited == true && std::get<2>(pfad.at(i))->prev_station->visited == true)
                        {
                            pfad.erase(pfad.begin() + i);
                            continue;
                        }
                        min = std::get<0>(pfad.at(i));
                        index = i;
                    }
                }
                temp_distance = std::get<0>(pfad.at(index));
                temp_pfad = std::get<1>(pfad.at(index));
                current_station = std::get<2>(pfad.at(index));
                //std::cout << std::endl << "Distance: " << temp_distance << std::endl;
                //std::cout << "Pfad: " << temp_pfad << std::endl;
                //std::cout << "Current_station: " << current_station->station.station_name << std::endl << std::endl;
            }

            //std::cout << current_station->station.station_name << std::endl;
            std::string temp_temp_pfad = temp_pfad;
            int temp_temp_distance = temp_distance;
            if (current_station->next_station->visited == false)
            {
                temp_pfad = temp_pfad + current_station->station.station_name + " -> ";
                if (current_station->station.station_name != end_point)
                {
                    temp_distance += current_station->time_to_next_station;
                }
                pfad.push_back({ temp_distance, temp_pfad, current_station->next_station });
                current_station->next_station->visited = true;
            }
            if (current_station->prev_station->visited == false)
            {
                temp_temp_pfad = temp_temp_pfad + current_station->station.station_name + " -> ";
                if (current_station->station.station_name != end_point)
                {
                    temp_temp_distance += current_station->time_to_prev_station;
                }
                pfad.push_back({ temp_temp_distance, temp_temp_pfad, current_station->prev_station });
                current_station->prev_station->visited = true;
            }
        }
        print_route(pfad);
    }
};

int main(int argument_count, char* argument_vector[])
{
    using namespace std;

    if (argument_count != 4)
    {
        cout << "Sie muessen folgende informationen in folgendem format uebergeben: Dateiname.txt Start Ziel" << endl;
    }
    else
    {
        string filename = argument_vector[1];
        string start_point = argument_vector[2];
        string end_point = argument_vector[3];


        std::vector<Linie> lines;

        read_file(filename, lines);

        netz wiener_netz;
        wiener_netz.create_netz(lines);

        //wiener_netz.print_lines();
        wiener_netz.dijkstra(start_point, end_point);
    }

    return 0;
}
