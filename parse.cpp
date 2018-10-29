#include "course.h"
#include "stringhelper.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <map>

CourseMap getClassesFromAdminData(const char *data_path) {
    CourseMap ret;
    std::ifstream infile(data_path);
    std::string line;
    for (int i = 0; i < 3; i++)
        std::getline(infile, line);

    std::string code;
    while (std::getline(infile, line)) {
        std::vector<std::string> row_values;
        split(line, '\t', row_values);

        for (auto v: row_values)
            std::cout << v << ',';
        std::cout << std::endl;

    /*
     * CLS,5187,T2,COMP,6447,Security Assessment, ,UGRD,COMPSC,CRS,12997,CR01,Enr,8881,GRD,A,No,P,KENS,KENSINGTON, , ,2018-07-23,2018-10-28,0,39,9999, ,0,0,0,65170,1
     * CLS,5187,T2,COMP,6447,Security Assessment, ,UGRD,COMPSC,LAB,9517,H12A,Non,1,GRD,A,No,P,KENS,KENSINGTON, , ,2018-07-23,2018-10-28,11,20,20,1467/COMP6447-PGRD/19,1,0,0,65170,1
     */
        //TODO: we can use CRSE_ID
        // CRSE_OFFER_NBR is for different career under same CRSE_ID (UGRD/PGRD)
        if (row_values[0] == "CLS") {
            code = row_values[3] + row_values[4];
            std::string career = row_values[7]; //TODO: UGRD or PGRD

            CourseMap::iterator it = ret.find(code);
            if (it == ret.end()) {
                Course c;
                c.name = row_values[5];
                c.code = code;
                ret[code] = c;
            }
            ret[code].classes.push_back(Class(row_values));
        }
        else if (row_values[0] == "MTG") {
            ret[code].classes.back().mtgs.push_back(Meeting(row_values));
        }
    }
    return ret;
}
