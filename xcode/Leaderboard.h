//
//  Leaderboard.h
//  SamichIsland
//
//  Created by Marione Semilla on 3/11/13.
//
//

#ifndef LEADERBOARD_HPP__
#define LEADERBOARD_HPP__

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const size_t MAXIMUM_NUM_LENGTH = 8;

struct Score {
    string name;
    int num_score;
};

vector<Score> getLeaderBoard ( char *file_path )
{
    vector<Score> leaderBoard(10);
    //simple parser
    string line;
    ifstream leadFile (file_path, ios::in);
    //leadFile.open(file_path);
    if (leadFile.good() == false )
    {
        cerr << "File not found" << endl;
        return leaderBoard;
    }
    
    
    while ( getline(leadFile, line))
    {
       
        string temp_score;
        string token; //one line
        istringstream iss(line);
        while (iss >> token)
        {
            size_t pos = token.find(' ');
            if (pos != string::npos)
            {
                Score s;
                s.name = token.substr(0, pos);
                string number = token.substr( pos, 4);
                s.num_score = std::atoi( number.c_str() );
                leaderBoard.push_back(s);
                cout << token << endl;
            }
         
        } 
        
    }
    
    for (size_t i = 0; i < leaderBoard.size(); ++i )
    {
        Score &s = leaderBoard[i];
        cout << "NAME: " << s.name << " SCORE: " << s.num_score << endl;
    }
    
    return leaderBoard;
}


#endif
