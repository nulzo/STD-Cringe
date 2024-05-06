#ifndef CRINGE_CRINGE_DATABASE_H
#define CRINGE_CRINGE_DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class CringeDB {
private:
	sqlite3 *db;

public:
	explicit CringeDB(const std::string& dbFile);
	~CringeDB();
	void execute(const std::string& sql, const std::vector<std::string>& params = {}) const;
	std::vector<std::vector<std::string>> query(const std::string& sql);
};


#endif
