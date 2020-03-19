
// ====================================================================
#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <vector>
#include <unistd.h>
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>       /* for sockaddr_un struct */
#define DEFAULT_PROTOCOL   0

/* POSIX renames "Unix domain" as "local IPC."
    Not all systems define AF_LOCAL and PF_LOCAL (yet). */
#ifndef AF_LOCAL
#define AF_LOCAL    AF_UNIX
#endif
#ifndef PF_LOCAL
#define PF_LOCAL    PF_UNIX
#endif

// ====================================================================

#define bufsize 100000


using namespace std;

struct Data
{
	int ID;
	string Description;
	string Category;
	string Sub_category;
	double amtPerUnit;
	int qty;
	time_t dateTransacted;
	
	bool operator==(const Data& b) const
	{
		return ID == b.ID && !Description.compare(b.Description) && !Category.compare(b.Category) && !Sub_category.compare(b.Sub_category) && amtPerUnit == b.amtPerUnit && qty == b.qty && dateTransacted == b.dateTransacted;
	}
	
	friend ostream& operator<< (ostream& o, const Data& d)
	{
		struct tm* time = localtime(&d.dateTransacted);
		char timestr[15];
		strftime(timestr,15, "%d-%m-%Y", time);
		return o << "ID:" << d.ID << "\nDescription: " << d.Description << "\nCategory: " << d.Category << "\nSub_category: " <<  d.Sub_category << "\namount per unit: " << d.amtPerUnit << "\nquantity: " << d.qty << "\nDate: " << timestr << endl;
	}
};

struct PriceA 
{
  bool operator() (Data* i,Data* j) 
  {
	  return (i->amtPerUnit < j->amtPerUnit);
  }
} priceAsc;

struct PriceD
{
  bool operator() (Data* i,Data* j) 
  {
	  return (i->amtPerUnit > j->amtPerUnit);
  }
} priceDsc;

struct QtyA 
{
  bool operator() (Data* i,Data* j) 
  {
	  return (i->qty < j->qty);
  }
} qtyAsc;

struct QtyD 
{
  bool operator() (Data* i,Data* j) 
  {
	  return (i->qty > j->qty);
  }
} qtyDsc;