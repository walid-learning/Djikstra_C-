#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

namespace travel{
  struct Station{
    std::string Name;
    std::string Line_id;
    std::string Address;
    std::string Line_Name;
    friend std::ostream& operator<<(std::ostream& _os, const Station& _stop){
      _os << "Station: " << _stop.Name << " (Line " << _stop.Line_id << ")";
      return _os;
    }
  };

  class Generic_station_parser{
  public:
    
    const std::unordered_map<uint64_t, Station>& get_stations_hashmap() const{
      return this->stations_hashmap;
    }

  protected:
    virtual void read_stations(const std::string& _fileName) = 0;

  protected:
    std::unordered_map<uint64_t, Station> stations_hashmap;
    
  };
}
