
#include "Assn2.h"
const char* dataFile = "Data.txt";
const int minStockAlertTrigger = 100;
list<list<Data>> arr;
string password;
const char* passFile = "password.txt";
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool quit = false;

void upper(char *a)
{
	for(int i = 0; a[i] != '\0'; i++)
		a[i] = toupper(a[i]);
}

int monthParse(const char* pch)
{
	if(!strcmp(pch,"JAN"))
		return 0;
	else if(!strcmp(pch,"FEB"))
		return 1;
	else if(!strcmp(pch,"MAR"))
		return 2;
	else if(!strcmp(pch,"APR"))
		return 3;
	else if(!strcmp(pch,"MAY"))
		return 4;
	else if(!strcmp(pch,"JUN"))
		return 5;
	else if(!strcmp(pch,"JUL"))
		return 6;
	else if(!strcmp(pch,"AUG"))
		return 7;
	else if(!strcmp(pch,"SEP"))
		return 8;
	else if(!strcmp(pch,"OCT"))
		return 9;
	else if(!strcmp(pch,"NOV"))
		return 10;
	else if(!strcmp(pch,"DEC"))
		return 11;
	else
		return -1;
}

bool validDate(time_t& t, struct tm& time)
{
	int cmp1 = time.tm_mday;
	int cmp2 = time.tm_mon;
	t = mktime(&time);
	if(cmp1 == time.tm_mday && cmp2 == time.tm_mon && t != -1)
		return true;
	return false;
}

string commaIt(double money)
{
	stringstream ss;
	ss << setprecision(2) << fixed << money;
	string commarise(ss.str());
	int Position = commarise.length() - 6;
	if(commarise[0] == '-')
	{
		while (Position > 1) 
		{
			commarise.insert(Position, ",");
			Position-=3;
		}
		commarise.insert(1, "$");
	}
	else
	{
		while (Position > 0) 
		{
			commarise.insert(Position, ",");
			Position-=3;
		}
		commarise.insert(0, "$");
	}
	return commarise;
}

string summary(char op, string& date1, string& date2)
{
	int in, out, counted;
	double amtPerUnit;
	stringstream ss, split;
	struct tm from = {}, till ={};
	time_t FROM, TILL;
	split << date1;
	string pch;
	getline(split, pch, '-');
	from.tm_mday = atoi(pch.c_str());
	getline(split, pch, '-');
	/*int mon = monthParse(pch);
	if(mon >= 0)
		from.tm_mon = mon;
	else
		ss << "invalid month" << endl;*/
	from.tm_mon = atoi(pch.c_str())-1;
	getline(split, pch, '-');
	from.tm_year = atoi(pch.c_str())+100;
	split.clear();
	split << date2;
	getline(split, pch, '-');
	till.tm_mday = atoi(pch.c_str());
	getline(split, pch, '-');
	/*mon = monthParse(pch);
	if(mon >= 0)
		till.tm_mon = mon;
	else
		ss << "invalid month" << endl;*/
	till.tm_mon = atoi(pch.c_str())-1;
	getline(split, pch, '-');
	till.tm_year = atoi(pch.c_str())+100;
	char timestr1[15], timestr2[15];
	int count = 0;
	if(validDate(FROM, from) && validDate(TILL, till))
	{
		if(op == 'f')
		{
			ss << "\nDay" << setw(10) << "From" << setw(10) << "To" << setw(15) 
			<< "item ID" << setw(30) << "Item Description" << setw(20) 
			<< "Amount Per Unit" << setw(10) << "in" << setw(15) << "out" 
			<< setw(15) << "Total($)" << endl;
			for(;FROM <= TILL; FROM+=24*60*60)
			{
				count++;
				struct tm* now = localtime(&FROM);
				strftime(timestr1,15, "%d-%m-%Y", now);
				int trigger = ss.str().size();
				for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
				{
					in = 0; out = 0; amtPerUnit = 0; counted = 0;
					for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
					{
						if((*it).dateTransacted == FROM)
						{
							if((*it).qty < 0)
								out -= (*it).qty;
							else if((*it).qty > 0)
								in += (*it).qty;
							amtPerUnit += (*it).amtPerUnit*abs((*it).qty);
							counted+=abs((*it).qty);
						}
					}
					if(counted > 0)
					{
						amtPerUnit /= counted;
						ss << setw(2) << count << setw(13)<< timestr1 << setw(13) << timestr1 << setw(8) << (*i).front().ID << setw(30) << (*i).front().Description 
						<< setw(20) << commaIt(amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*amtPerUnit) << endl;
					}
					
				}
				if(trigger == ss.str().size())
					ss << setw(2) << count << setw(13)<< timestr1 << setw(13) << timestr1 << endl;
			}
		}
		else if(op == 'g')
		{
			ss << "\nWeek" << setw(10) << "From" << setw(10) << "To" << setw(15) 
			<< "item ID" << setw(30) << "Item Description" << setw(20) 
			<< "Amount Per Unit" << setw(10) << "in" << setw(15) << "out" 
			<< setw(15) << "Total($)" << endl;
			while(FROM <= TILL)
			{
				count++;
				time_t TO = FROM;
				struct tm* to = localtime(&TO);
				strftime(timestr1,15, "%d-%m-%Y", to);
				while(to->tm_wday != 0 && TO < TILL )
				{
					TO+=24*60*60;
					to = localtime(&TO);
				}
				
				strftime(timestr2,15, "%d-%m-%Y", to);
				
				int trigger = ss.str().size();
				for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
				{
					in = 0; out = 0; amtPerUnit = 0; counted = 0;
					for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
					{
						if((*it).dateTransacted >= FROM && (*it).dateTransacted <= TO )
						{
							if((*it).qty < 0)
								out -= (*it).qty;
							else if((*it).qty > 0)
								in += (*it).qty;
							amtPerUnit += (*it).amtPerUnit*abs((*it).qty);
							counted+=abs((*it).qty);
						}
					}
					if(counted > 0)
					{
						amtPerUnit /= counted;
						ss << setw(2) << count << setw(13)<< timestr1 << setw(13) << timestr2 << setw(8) << (*i).front().ID << setw(30) << (*i).front().Description 
						<< setw(20) << commaIt(amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*amtPerUnit) << endl;
					}
					
				}
				if(trigger == ss.str().size())
					ss << setw(2) << count << setw(13) << timestr1 << setw(13) << timestr2 << endl;
				FROM = TO + 24*60*60;
			}
		}
		else if(op == 'h')
		{
			ss << "\nMonth" << setw(10) << "From" << setw(10) << "To" << setw(15) 
			<< "item ID" << setw(30) << "Item Description" << setw(20) 
			<< "Amount Per Unit" << setw(10) << "in" << setw(15) << "out" 
			<< setw(15) << "Total($)" << endl;
			while(FROM <= TILL)
			{
				count++;
				time_t TO = FROM;
				struct tm* to = localtime(&TO);
				strftime(timestr1,15, "%d-%m-%Y", to);
				int check = to->tm_mon;
				while(check == to->tm_mon && TO <= TILL )
				{
					TO+=24*60*60;
					to = localtime(&TO);
				}
				TO-=24*60*60;
				to = localtime(&TO);
				strftime(timestr2,15, "%d-%m-%Y", to);
				
				int trigger = ss.str().size();
				for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
				{
					in = 0; out = 0; amtPerUnit = 0; counted = 0;
					for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
					{
						if((*it).dateTransacted >= FROM && (*it).dateTransacted <= TO )
						{
							if((*it).qty < 0)
								out -= (*it).qty;
							else if((*it).qty > 0)
								in += (*it).qty;
							amtPerUnit += (*it).amtPerUnit*abs((*it).qty);
							counted+=abs((*it).qty);
						}
					}
					if(counted > 0)
					{
						amtPerUnit /= counted;
						ss << setw(2) << count << setw(13)<< timestr1 << setw(13) << timestr2 << setw(8) << (*i).front().ID << setw(30) << (*i).front().Description 
						<< setw(20) << commaIt(amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*amtPerUnit) << endl;
					}
					
				}
				if(trigger == ss.str().size())
					ss << setw(2) << count << setw(13) << timestr1 << setw(13) << timestr2 << endl;
				FROM = TO + 24*60*60;
			}
		}
		else if(op == 'i')
		{
			ss << "\nYear" << setw(10) << "From" << setw(10) << "To" << setw(15) 
			<< "item ID" << setw(30) << "Item Description" << setw(20) 
			<< "Amount Per Unit" << setw(10) << "in" << setw(15) << "out" 
			<< setw(15) << "Total($)" << endl;
			while(FROM <= TILL)
			{
				count++;
				time_t TO = FROM;
				struct tm* to = localtime(&TO);
				strftime(timestr1,15, "%d-%m-%Y", to);
				int check = to->tm_year;
				while(check == to->tm_year && TO <= TILL )
				{
					TO+=24*60*60;
					to = localtime(&TO);
				}
				TO-=24*60*60;
				to = localtime(&TO);
				strftime(timestr2,15, "%d-%m-%Y", to);
				
				int trigger = ss.str().size();
				for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
				{
					in = 0; out = 0; amtPerUnit = 0; counted = 0;
					for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
					{
						if((*it).dateTransacted >= FROM && (*it).dateTransacted <= TO )
						{
							if((*it).qty < 0)
								out -= (*it).qty;
							else if((*it).qty > 0)
								in += (*it).qty;
							amtPerUnit += (*it).amtPerUnit*abs((*it).qty);
							counted+=abs((*it).qty);
						}
					}
					if(counted > 0)
					{
						amtPerUnit /= counted;
						ss << setw(2) << count << setw(13)<< timestr1 << setw(13) << timestr2 << setw(8) << (*i).front().ID << setw(30) << (*i).front().Description 
						<< setw(20) << commaIt(amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*amtPerUnit) << endl;
					}
					
				}
				if(trigger == ss.str().size())
					ss << setw(2) << count << setw(13) << timestr1 << setw(13) << timestr2 << endl;
				FROM = TO + 24*60*60;
			}
		}
	}
	else
		ss << "ERR:date validation failed!" << endl;
	
	if(ss.str().size() > bufsize - 1)
	{
		ss.str("ERR:buffer overflow");
	}
	return ss.str();
}

int itemExist(Data& a)
{
	for(list<list<Data>>::iterator it = arr.begin(); it != arr.end(); ++it)
	{
		if((*it).front().ID == a.ID && !(*it).front().Description.compare(a.Description) && !(*it).front().Category.compare(a.Category) && !(*it).front().Sub_category.compare(a.Sub_category))
			return distance(arr.begin(), it);
		else if((*it).front().ID == a.ID)
			return -2;
	}
	return -1;
}

int find(int ID)
{
	for(list<list<Data>>::iterator it = arr.begin(); it != arr.end(); ++it)
	{
		if((*it).front().ID == ID)
			return distance(arr.begin(), it);
	}
	return -1;
}

void readData ()
{
	string buf;
	fstream file(dataFile, ios::in|ios::binary);
	if(!file)
		cerr << "data file io" << endl;
	else
	{
		stringstream ss;
		while(getline(file,buf, '\0'))
		{
			Data d;
			ss << buf;
			string pch;
			struct tm time = {};
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.Description = pch;
			getline(ss, pch, ':');
			d.Category = pch;
			getline(ss, pch, ':');
			d.Sub_category = pch;
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			getline(ss, pch, '-');
			time.tm_mday = atoi(pch.c_str());
			getline(ss, pch, '-');
			int mon = monthParse(pch.c_str());
			if(mon >= 0)
				time.tm_mon = mon;
			else
				cerr << "invalid month" << endl;
			getline(ss, pch, '-');
			time.tm_year = atoi(pch.c_str())+100;
			d.dateTransacted = mktime(&time);
			if(d.dateTransacted == -1)
			{
				cerr << "critical error at load: time create failed!" << endl;
				exit(EXIT_FAILURE);
			}
			int exist = itemExist(d);
			if(exist >= 0)
			{
				list<list<Data>>::iterator it = arr.begin();
				advance(it, exist);
				(*it).push_back(d);
			}
			else if(exist == - 1)
			{
				arr.push_back(list<Data>());
				arr.back().push_back(d);
			}
			else
				cerr << "ID conflict detected!" << endl;
			ss.clear();
		}
	}
	
	file.close();
}

void appWrite(Data& d)
{
	fstream out(dataFile, ios::out|ios::binary|ios::app);
	if(!out)
		cerr << "write io" << endl;
	else
	{
		stringstream ss;
		
		struct tm *time = localtime(&d.dateTransacted);
		string month;
		if(time->tm_mon == 0)
			month = "JAN";
		else if(time->tm_mon == 1)
			month = "FEB";
		else if(time->tm_mon == 2)
			month = "MAR";
		else if(time->tm_mon == 3)
			month = "APR";
		else if(time->tm_mon == 4)
			month = "MAY";
		else if(time->tm_mon == 5)
			month = "JUN";
		else if(time->tm_mon == 6)
			month = "JUL";
		else if(time->tm_mon == 7)
			month = "AUG";
		else if(time->tm_mon == 8)
			month = "SEP";
		else if(time->tm_mon == 9)
			month = "OCT";
		else if(time->tm_mon == 10)
			month = "NOV";
		else if(time->tm_mon == 11)
			month = "DEC";
		ss << d.ID << ':' << d.Description << ':' << d.Category << ':' << d.Sub_category << ':' << d.amtPerUnit << ':' << d.qty << ':' << time->tm_mday << '-' << month << '-' << time->tm_year - 100;
		out.write(ss.str().c_str(), strlen(ss.str().c_str())+1);
	}
	out.close();
}

void writeData()
{
	fstream out(dataFile, ios::out|ios::binary);
	if(!out)
		cerr << "write io" << endl;
	else
	{
		stringstream ss;
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				struct tm *time = localtime(&(*it).dateTransacted);
				string month;
				if(time->tm_mon == 0)
					month = "JAN";
				else if(time->tm_mon == 1)
					month = "FEB";
				else if(time->tm_mon == 2)
					month = "MAR";
				else if(time->tm_mon == 3)
					month = "APR";
				else if(time->tm_mon == 4)
					month = "MAY";
				else if(time->tm_mon == 5)
					month = "JUN";
				else if(time->tm_mon == 6)
					month = "JUL";
				else if(time->tm_mon == 7)
					month = "AUG";
				else if(time->tm_mon == 8)
					month = "SEP";
				else if(time->tm_mon == 9)
					month = "OCT";
				else if(time->tm_mon == 10)
					month = "NOV";
				else if(time->tm_mon == 11)
					month = "DEC";
				ss << (*it).ID << ':' << (*it).Description << ':' << (*it).Category << ':' << (*it).Sub_category << ':' << (*it).amtPerUnit << ':' << (*it).qty << ':' << time->tm_mday << '-' << month << '-' << time->tm_year - 100;
				out.write(ss.str().c_str(), strlen(ss.str().c_str())+1);
				ss.str("");
			}
		}
		out.close();
		
	}
}

bool readLine (int fd, char* str)
 {
   int n;
   do 
     {
       n = read (fd, str, 1); 
     }
   while (n > 0 && *str++ != 0);
   return (n > 0); 
 }

const char* addNew(Data& d)
{
	int exist = itemExist(d);
	time_t NOW = time(NULL);
	struct tm *now = localtime(&NOW);
	now->tm_sec=0;
	now->tm_min=0;
	now->tm_hour=0;
	d.dateTransacted = mktime(now);
	if(exist >= 0)
	{
		list<list<Data>>::iterator it = arr.begin();
		advance(it, exist);
		(*it).push_back(d);
		appWrite(d);
		return "existing stock detected: stock updated under existing stock of same ID, description, category and sub_category.\n";
	}
	else if(exist == - 1)
	{
		arr.push_back(list<Data>());
		arr.back().push_back(d);
		appWrite(d);
		return "stock added!\n";
	}
	else
		return "ID conflict detected!no changes made.\n";
}

const char* add(Data& d)
{
	int found = find(d.ID);
	if(found >= 0)
	{
		list<list<Data>>::iterator it = arr.begin();
		advance(it, found);
		d.Description = (*it).front().Description;
		d.Category = (*it).front().Category;
		d.Sub_category = (*it).front().Sub_category;
		time_t NOW = time(NULL);
		struct tm *now = localtime(&NOW);
		now->tm_sec=0;
		now->tm_min=0;
		now->tm_hour=0;
		d.dateTransacted = mktime(now);
		(*it).push_back(d);
		appWrite(d);
		return "stock updated!\n";
	}
	else
	{
		return "ID not found!\n";
	}
}

string remove(Data& d)
{
	int found = find(d.ID);
	stringstream ss;
	if(found >= 0)
	{
		list<list<Data>>::iterator it = arr.begin();
		advance(it, found);
		int qty = 0;
		for(list<Data>::iterator i=(*it).begin(); i != (*it).end(); ++i)
		{
			qty+=(*i).qty;
		}
		if((qty += d.qty) < 0) //d.qty is already negative. if - will result in addition instead
		{
			ss << "ERR: there is a shortage of: " << abs(qty) << ". no changes made" << endl;
			return ss.str();
		}
		else if(qty < minStockAlertTrigger)
			ss << "Warning: low stock alert. stock available is less than 100\n";
		d.Description = (*it).front().Description;
		d.Category = (*it).front().Category;
		d.Sub_category = (*it).front().Sub_category;
		time_t NOW = time(NULL);
		struct tm *now = localtime(&NOW);
		now->tm_sec=0;
		now->tm_min=0;
		now->tm_hour=0;
		d.dateTransacted = mktime(now);
		(*it).push_back(d);
		appWrite(d);
	}
	else
	{
		return "ID not found!\n";
	}
	ss << "stock updated!\n";
	return ss.str();
}

void edit(int found,Data& d)
{
	list<list<Data>>::iterator it = arr.begin();
	advance(it, found);
	if(d.ID != (*it).front().ID)
	{
		for(list<Data>::iterator i = (*it).begin(); i != (*it).end(); ++i)
		{
			(*i).ID = d.ID;
		}
	}
	//if(!d.Description.empty())
	//{
		if(d.Description.compare((*it).front().Description) != 0)
		{
			for(list<Data>::iterator i = (*it).begin(); i != (*it).end(); ++i)
			{
				(*i).Description = d.Description;
			}
		}
	//}
	//if(!d.Category.empty()) 
		if(d.Category.compare((*it).front().Category) != 0)
		{
			for(list<Data>::iterator i = (*it).begin(); i != (*it).end(); ++i)
			{
				(*i).Category = d.Category;
			}
		}
	//if(!d.Sub_category.empty())
		if(d.Sub_category.compare((*it).front().Sub_category) != 0)
		{
			for(list<Data>::iterator i = (*it).begin(); i != (*it).end(); ++i)
			{
				(*i).Sub_category = d.Sub_category;
			}
		}
	writeData();
}

string search(char a, string& b)
{
	stringstream ss;
	ss << "\nDate" << setw(15) << "item ID" << setw(25) << "Item Description" << setw(15) << "Item Category" << setw(20) << "Item Sub-category" << setw(18) 
			<< "Amount Per Unit" << setw(10) << "in" << setw(10) << "out" 
			<< setw(15) << "Total($)" << endl;
	char timestr1[15];
	if( a == 'a')
	{
		int size = strlen(b.c_str()+1);
		char up[size];
		strcpy(up, b.c_str());
		upper(up);
		
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if(!(*it).Category.compare(up))
				{
					int in = 0, out = 0;
					if((*it).qty < 0)
						out -= (*it).qty;
					else if((*it).qty > 0)
						in += (*it).qty;
					struct tm* now = localtime(&(*it).dateTransacted);
					strftime(timestr1,15, "%d-%m-%Y", now);
					ss << setw(2) << timestr1 << setw(7) << (*it).ID << setw(25) << (*it).Description << setw(15) << (*it).Category << setw(15) << (*it).Sub_category 
					<< setw(20) << commaIt((*it).amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it).amtPerUnit) << endl;
				}
			}		
					
		}
	}
	else if( a == 'b')
	{
		int size = strlen(b.c_str()+1);
		char up[size];
		strcpy(up, b.c_str());
		upper(up);
		
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if(!(*it).Sub_category.compare(up))
				{
					int in = 0, out = 0;
					if((*it).qty < 0)
						out -= (*it).qty;
					else if((*it).qty > 0)
						in += (*it).qty;
					struct tm* now = localtime(&(*it).dateTransacted);
					strftime(timestr1,15, "%d-%m-%Y", now);
					ss << setw(2) << timestr1 << setw(7) << (*it).ID << setw(25) << (*it).Description << setw(15) << (*it).Category << setw(15) << (*it).Sub_category 
					<< setw(20) << commaIt((*it).amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it).amtPerUnit) << endl;
				}
			}		
					
		}
	}
	else if( a == 'c')
	{
		struct tm at={};
		char *pch = strtok((char*)b.c_str(), "-");
		at.tm_mday = atoi(pch);
		pch = strtok(NULL, "-");
		at.tm_mon = atoi(pch)-1;
		pch = strtok(NULL, "-");
		at.tm_year = atoi(pch)+100;
		time_t AT = mktime(&at);
		
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if((*it).dateTransacted == AT)
				{
					int in = 0, out = 0;
					if((*it).qty < 0)
						out -= (*it).qty;
					else if((*it).qty > 0)
						in += (*it).qty;
					struct tm* now = localtime(&(*it).dateTransacted);
					strftime(timestr1,15, "%d-%m-%Y", now);
					ss << setw(2) << timestr1 << setw(7) << (*it).ID << setw(25) << (*it).Description << setw(15) << (*it).Category << setw(15) << (*it).Sub_category 
					<< setw(20) << commaIt((*it).amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it).amtPerUnit) << endl;
				}
			}		
					
		}
	}
	else if( a == 'd')
	{
		double from, to;
		char *pch = strtok((char*)b.c_str(), ",");
		from = atof(pch);
		pch = strtok(NULL, ",");
		to = atof(pch);
		vector<Data*> buffer;
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if((*it).amtPerUnit >= from && (*it).amtPerUnit <= to)
				{
					buffer.push_back(&*it);
				}
			}				
		}
		
		stable_sort(buffer.begin(), buffer.end(), priceAsc);
		
		for(vector<Data*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			int in = 0, out = 0;
			if((*it)->qty < 0)
				out -= (*it)->qty;
			else if((*it)->qty > 0)
				in += (*it)->qty;
			struct tm* now = localtime(&(*it)->dateTransacted);
			strftime(timestr1,15, "%d-%m-%Y", now);
			ss << setw(2) << timestr1 << setw(7) << (*it)->ID << setw(25) << (*it)->Description << setw(15) << (*it)->Category << setw(15) << (*it)->Sub_category 
			<< setw(20) << commaIt((*it)->amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it)->amtPerUnit) << endl;
		}
	}
	else if( a == 'e')
	{
		double from, to;
		char *pch = strtok((char*)b.c_str(), ",");
		from = atof(pch);
		pch = strtok(NULL, ",");
		to = atof(pch);
		vector<Data*> buffer;
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if((*it).amtPerUnit >= from && (*it).amtPerUnit <= to)
				{
					buffer.push_back(&*it);
				}
			}				
		}
		
		stable_sort(buffer.begin(), buffer.end(), priceDsc);
		
		for(vector<Data*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			int in = 0, out = 0;
			if((*it)->qty < 0)
				out -= (*it)->qty;
			else if((*it)->qty > 0)
				in += (*it)->qty;
			struct tm* now = localtime(&(*it)->dateTransacted);
			strftime(timestr1,15, "%d-%m-%Y", now);
			ss << setw(2) << timestr1 << setw(7) << (*it)->ID << setw(25) << (*it)->Description << setw(15) << (*it)->Category << setw(15) << (*it)->Sub_category 
			<< setw(20) << commaIt((*it)->amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it)->amtPerUnit) << endl;
		}
	}
	else if( a == 'f')
	{
		int from, to;
		char *pch = strtok((char*)b.c_str(), ",");
		from = atoi(pch);
		pch = strtok(NULL, ",");
		to = atoi(pch);
		vector<Data*> buffer;
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if((*it).qty >= from && (*it).qty <= to)
				{
					buffer.push_back(&*it);
				}
			}				
		}
		
		stable_sort(buffer.begin(), buffer.end(), qtyAsc);
		
		for(vector<Data*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			int in = 0, out = 0;
			if((*it)->qty < 0)
				out -= (*it)->qty;
			else if((*it)->qty > 0)
				in += (*it)->qty;
			struct tm* now = localtime(&(*it)->dateTransacted);
			strftime(timestr1,15, "%d-%m-%Y", now);
			ss << setw(2) << timestr1 << setw(7) << (*it)->ID << setw(25) << (*it)->Description << setw(15) << (*it)->Category << setw(15) << (*it)->Sub_category 
			<< setw(20) << commaIt((*it)->amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it)->amtPerUnit) << endl;
		}
	}
	else if( a == 'g')
	{
		int from, to;
		char *pch = strtok((char*)b.c_str(), ",");
		from = atoi(pch);
		pch = strtok(NULL, ",");
		to = atoi(pch);
		vector<Data*> buffer;
		for(list<list<Data>>::iterator i = arr.begin(); i != arr.end(); ++i)
		{
			for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
			{
				if((*it).qty >= from && (*it).qty <= to)
				{
					buffer.push_back(&*it);
				}
			}				
		}
		
		stable_sort(buffer.begin(), buffer.end(), qtyDsc);
		
		for(vector<Data*>::iterator it = buffer.begin(); it != buffer.end(); ++it)
		{
			int in = 0, out = 0;
			if((*it)->qty < 0)
				out -= (*it)->qty;
			else if((*it)->qty > 0)
				in += (*it)->qty;
			struct tm* now = localtime(&(*it)->dateTransacted);
			strftime(timestr1,15, "%d-%m-%Y", now);
			ss << setw(2) << timestr1 << setw(7) << (*it)->ID << setw(25) << (*it)->Description << setw(15) << (*it)->Category << setw(15) << (*it)->Sub_category 
			<< setw(20) << commaIt((*it)->amtPerUnit) << setw(13) << in << setw(13) << out << setw(20) << commaIt((out - in)*(*it)->amtPerUnit) << endl;
		}
	}
	
	if(ss.str().size() > bufsize - 1)
	{
		ss.str("ERR:buffer overflow");
	}
	return ss.str();
}

const char* ADD(Data& d)
{
	int exist = itemExist(d);
	if(exist >= 0)
	{
		list<list<Data>>::iterator it = arr.begin();
		advance(it, exist);
		(*it).push_back(d);
		appWrite(d);
		return "existing transaction detected: updated under existing ID, description, category and sub_category.\n";
	}
	else if(exist == - 1)
	{
		arr.push_back(list<Data>());
		arr.back().push_back(d);
		appWrite(d);
		return "transaction added!\n";
	}
	else
		return "ID conflict detected!no changes made.\n";
}

const char* del(Data& d)
{
	int exist = itemExist(d);
	if(exist >= 0)
	{
		list<list<Data>>::iterator i = arr.begin();
		advance(i, exist);
		for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
		{
			if(d == (*it))
			{
				(*i).erase(it);
				if((*i).empty())
					arr.erase(i);
				writeData();
				return "transaction removed!\n";
			}
		}
	}
	
	return "transaction not found!\n";
}

int match(int position, Data& d)
{
	list<list<Data>>::iterator i = arr.begin();
	advance(i, position);
	for(list<Data>::iterator it = (*i).begin(); it != (*i).end(); ++it)
	{
		if(d == (*it))
		{
			return distance((*i).begin(), it);
		}
	}
	
	return -1;
}

const char* update(Data& d, Data& u)
{
	int exist = itemExist(d);
	int check = itemExist(u);
	if(exist >= 0 && check >= -1)
	{
		list<list<Data>>::iterator i = arr.begin();
		advance(i, exist);
			(*i).remove(d);
		if(check >= 0)
		{
			list<list<Data>>::iterator it = arr.begin();
			advance(it, check);
			(*it).push_back(u);
			writeData();
		}
		else if(check == - 1)
		{
			arr.push_back(list<Data>());
			arr.back().push_back(u);
			writeData();
		}
		return "transaction updated!\n";
		
	}
	
	return "update failed!\n";
}

string crypt(string crypt) 
{
    char key[3] = {'A', 'B', 'C'}; 
    
    for (int i = 0; i < crypt.size(); i++)
        crypt[i] = crypt[i] ^ key[i % (sizeof(key) / sizeof(char))];
    
    return crypt;
}

void *comm(void* arg)
{
	int fd = *(int*)arg;
	char str[bufsize];
	stringstream ss;
	while(1)
	{
		readLine (fd, str);
		if(str[0] == 'm')
			break;
		else if(str[0] == 'a')
		{
			upper(str);
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.Description = pch;
			getline(ss, pch, ':');
			d.Category = pch;
			getline(ss, pch, ':');
			d.Sub_category = pch;
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, addNew(d));
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'b')
		{
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, add(d));
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'c')
		{
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = -atoi(pch.c_str());
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, remove(d).c_str());
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'd')
		{
			ss << str;
			string pch;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			int ID = atoi(pch.c_str());
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			int found = find(ID);
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
			const char* msg = "found";
			if(found >= 0)
			{
				write(fd, msg, strlen(msg) + 1);
				readLine (fd, str);
				if(!strcmp(str, ""))
					continue;
				else
				{
					upper(str);
					ss.clear();
					ss << str;
					Data d;
					getline(ss, pch, ':');
					d.ID = atoi(pch.c_str());
					getline(ss, pch, ':');
					d.Description = pch;
					getline(ss, pch, ':');
					d.Category = pch;
					getline(ss, pch, ':');
					d.Sub_category = pch;
					int status = pthread_mutex_lock (&mutex);
					if (status != 0)
						cerr << "Lock mutex";
					edit(found, d);
					status = pthread_mutex_unlock (&mutex);
					if (status != 0)
						cerr << "Unlock mutex";
					continue;
				}
			}
			else
				strcpy(str, "ID not found!\n");
		}
		else if(str[0] == 'e')
		{
			ss << str;
			string pch;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			char op = pch[0];
			getline(ss, pch, ':');
			//string temp(pch);
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, search(op, pch).c_str());
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'f' || str[0] == 'g' || str[0] == 'h' || str[0] == 'i')
		{
			ss << str;
			string pch;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			string para2, para3;
			para2 = pch;
			getline(ss, pch, ':');
			para3 = pch;
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, summary(str[0], para2, para3).c_str());
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'j')
		{
			upper(str);
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.Description = pch;
			getline(ss, pch, ':');
			d.Category = pch;
			getline(ss, pch, ':');
			d.Sub_category = pch;
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			struct tm at={};
			getline(ss, pch, '-');
			at.tm_mday = atoi(pch.c_str());
			getline(ss, pch, '-');
			at.tm_mon = atoi(pch.c_str())-1;
			getline(ss, pch, '-');
			at.tm_year = atoi(pch.c_str())+100;
			time_t AT = mktime(&at);
			d.dateTransacted = AT;
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, ADD(d));
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'k')
		{
			upper(str);
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.Description = pch;
			getline(ss, pch, ':');
			d.Category = pch;
			getline(ss, pch, ':');
			d.Sub_category = pch;
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			struct tm at={};
			getline(ss, pch, '-');
			at.tm_mday = atoi(pch.c_str());
			getline(ss, pch, '-');
			at.tm_mon = atoi(pch.c_str())-1;
			getline(ss, pch, '-');
			at.tm_year = atoi(pch.c_str())+100;
			time_t AT = mktime(&at);
			d.dateTransacted = AT;
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			strcpy(str, del(d));
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
		}
		else if(str[0] == 'l')
		{
			upper(str);
			ss << str;
			string pch;
			Data d;
			getline(ss, pch, ':');
			getline(ss, pch, ':');
			d.ID = atoi(pch.c_str());
			getline(ss, pch, ':');
			d.Description = pch;
			getline(ss, pch, ':');
			d.Category = pch;
			getline(ss, pch, ':');
			d.Sub_category = pch;
			getline(ss, pch, ':');
			d.amtPerUnit = atof(pch.c_str());
			getline(ss, pch, ':');
			d.qty = atoi(pch.c_str());
			struct tm at={};
			getline(ss, pch, '-');
			at.tm_mday = atoi(pch.c_str());
			getline(ss, pch, '-');
			at.tm_mon = atoi(pch.c_str())-1;
			getline(ss, pch, '-');
			at.tm_year = atoi(pch.c_str())+100;
			time_t AT = mktime(&at);
			d.dateTransacted = AT;
			int status = pthread_mutex_lock (&mutex);
			if (status != 0)
				cerr << "Lock mutex";
			int outer = itemExist(d);
			int inner = -1;
			if(outer >= 0)
				inner = match(outer, d);
			status = pthread_mutex_unlock (&mutex);
			if (status != 0)
				cerr << "Unlock mutex";
			const char* msg = "found";
			if(inner >= 0)
			{
				write(fd, msg, strlen(msg) + 1);
				readLine (fd, str);
				if(!strcmp(str, ""))
					continue;
				else
				{
					upper(str);
					ss.clear();
					ss << str;
					Data u;
					getline(ss, pch, ':');
					u.ID = atoi(pch.c_str());
					getline(ss, pch, ':');
					u.Description = pch;
					getline(ss, pch, ':');
					u.Category = pch;
					getline(ss, pch, ':');
					u.Sub_category = pch;
					getline(ss, pch, ':');
					u.amtPerUnit = atof(pch.c_str());
					getline(ss, pch, ':');
					u.qty = atoi(pch.c_str());
					struct tm at={};
					getline(ss, pch, '-');
					at.tm_mday = atoi(pch.c_str());
					getline(ss, pch, '-');
					at.tm_mon = atoi(pch.c_str())-1;
					getline(ss, pch, '-');
					at.tm_year = atoi(pch.c_str())+100;
					time_t AT = mktime(&at);
					u.dateTransacted = AT;
					int status = pthread_mutex_lock (&mutex);
					if (status != 0)
						cerr << "Lock mutex";
					strcpy(str, update(d, u)); //use outer in case inner position change due to other threads.
					status = pthread_mutex_unlock (&mutex);
					if (status != 0)
						cerr << "Unlock mutex";
				}
			}
			else
				strcpy(str, "transaction not found!\n");
		}
		else if(str[0] == 'p')
		{
			string pch(str);
			pch.erase(0,2);
			if(password.compare(crypt(pch)) == 0)
				strcpy(str,"ok");
		}
		write(fd, str, strlen(str) + 1);
		ss.clear();
	}
	close (fd); 
}

void loadPass()
{
	fstream file(passFile, ios::in);
	if(!file)
	{
		fstream out(passFile, ios::out);
		if(out)
		{
			out << crypt("0000");
			out.close();
			loadPass();
		}
		else
		{
			cerr << "unable to get password information" << endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		getline(file, password);
		password = crypt(password);
		file.close();
	}
}

int main()
{
	readData ();
	loadPass();
	int serverFd, clientFd, status;
	socklen_t serverLen, clientLen;
	struct sockaddr_un serverAddress;
	struct sockaddr_un clientAddress;
	struct sockaddr* serverSockAddrPtr; 
	struct sockaddr* clientSockAddrPtr; 

	signal(SIGPIPE, SIG_IGN);
	
	serverSockAddrPtr = (struct sockaddr*)&serverAddress;
	serverLen = sizeof (serverAddress);

	clientSockAddrPtr = (struct sockaddr*)&clientAddress;
	clientLen = sizeof (clientAddress);

	serverFd = socket (AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverAddress.sun_family = AF_LOCAL; 
	strcpy (serverAddress.sun_path, "recipe");
	unlink ("recipe"); 
	bind (serverFd, serverSockAddrPtr, serverLen); 
	listen (serverFd, 5); 
	pthread_t thread;
	
	while (!quit) 
	{
		
		clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
		status = pthread_create(&thread, NULL, comm, (void*)&clientFd);
		if (status != 0)
			cerr << "Create thread";
	}
	//pthread_mutex_destroy ( &mutex );
}

// ====================================================================



