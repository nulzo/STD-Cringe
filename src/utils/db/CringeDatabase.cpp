#include <sqlite3.h>

class CringeDatabase {
private:
  sqlite3 dbase;

public:
  CringeDatabase(const std::string &database) : dbase(nullptr) {
    int connected = sqlite3_open(database.c_str(), &dbase);
    if (connected != SQLITE_OK) {
      std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(dbase);
    }
  }
  
  ~CringeDatabase() {
    if(dbase) {
      sqlite3_close(dbase);
    }
  }

  bool insert()
}
