//
// Created by Nolan Gregory on 3/29/24.
//

#ifndef CRINGE_DATABASE_H
#define CRINGE_DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class CringeDB {
private:
	sqlite3 *db;

public:
	CringeDB(const std::string& dbFile);
	~CringeDB();
	void execute(const std::string& sql);
	std::vector<std::vector<std::string>> query(const std::string& sql);
};


#endif //CRINGE_DATABASE_H
