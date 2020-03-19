#include "Assn2.h"
#define DEFAULT_PROTOCOL    0

/* POSIX renames "Unix domain" as "local IPC."
    Not all systems define AF_LOCAL and PF_LOCAL (yet). */
#ifndef AF_LOCAL
#define AF_LOCAL    AF_UNIX
#endif
#ifndef PF_LOCAL
#define PF_LOCAL    PF_UNIX
#endif
/****************************************************************/
 /**************************************************************/
 /* Read a single NULL-terminated line */
bool readLine (int fd, char* str)
 {
   int n;
   do /* Read characters until NULL or end-of-input */
     {
	 // ssize_t read (int fd, void *buf, size_t count);
	 // if successful, read will:
	 // a) stores data read into 'buf', and
	 // b) returns the no. of bytes read
	 // read returns zero if it reaches end-of-input
       n = read (fd, str, 1); /* Read one character */
     }
   while (n > 0 && *str++ != 0);
   return (n > 0); /* Return false if end-of-input */
 }

string crypt(string crypt) 
{
    char key[3] = {'A', 'B', 'C'}; 
    
    for (int i = 0; i < crypt.size(); i++)
        crypt[i] = crypt[i] ^ key[i % (sizeof(key) / sizeof(char))];
    
    return crypt;
}


 
void com(int fd)
{
	string str;
	char buf[bufsize];
	stringstream ss;
	cout << "Welcome to stock managment system" << endl;
	int loginCount = 0;
	bool login = false;
	while( !login && loginCount < 3)
	{
		cout << "User name: ";
		getline(cin, str);
		str = crypt(getpass("enter password: "));
		str.insert(0, "p:");
		write(fd, str.c_str(), strlen(str.c_str()) + 1);
		readLine (fd, buf);
		if(!strcmp(buf,"ok"))
			login = true;
		else
		{
			cout << "incorrect password" << endl;
		}
		loginCount++;
		while(str[0] != 'm' && login)
		{
			cout << "a) Create stock record\nb) Add to existing stock\nc) Remove stock\nd) Edit stock item\ne) Search stock item\nf) Daily stock summary report\ng) Weekly stock summary report\nh) Monthly stock summary report\ni) Yearly stock summary report\nj) Add transaction\nk) Remove transaction\nl) update transaction\nm) Quit\n\nenter your choice: ";
			getline(cin, str);
			if(str[0] == 'a')
			{
				ss << str[0];
				int ID, qty;
				string Description, Category, Sub_category;
				double amtPerUnit;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				cout << "Description: ";
				getline(cin, Description);
				ss << ':' << Description;
				cout << "Category: ";
				getline(cin, Category);
				ss << ':' << Category;
				cout << "Sub_category: ";
				getline(cin, Sub_category);
				ss << ':' << Sub_category;
				cout << "amount per unit: ";
				cin >> amtPerUnit;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << amtPerUnit;
				cout << "quantity to add: ";
				cin >> qty;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << qty;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				cout << buf;
			}
			else if(str[0] == 'b')
			{
				ss << str[0];
				int ID, qty;
				double amtPerUnit;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				cout << "amount per unit: ";
				cin >> amtPerUnit;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << amtPerUnit;
				cout << "quantity to add: ";
				cin >> qty;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << qty;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				cout << buf;
			}
			else if(str[0] == 'c')
			{
				ss << str[0];
				int ID, qty;
				double amtPerUnit;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				cout << "amount per unit: ";
				cin >> amtPerUnit;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << amtPerUnit;
				cout << "quantity to remove: ";
				cin >> qty;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << qty;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				cout << buf;
			}
			else if(str[0] == 'd')
			{
				ss << str[0];
				int ID;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				if(!strcmp(buf, "found"))
				{
					int op = 0;
					ss.str("");
					string Description, Category, Sub_category;
					while(op != 6 && op != 5)
					{
						cout << "\t1) update ID\n\t2) update Description\n\t3) update Category\n\t4) update Sub_category\n\t5) save and return\n\t6) return without saving\n\tenter option: ";
						cin >> op;
						cin.clear();
						cin.ignore(9999, '\n');
						if(op == 1)
						{
							cout << "new ID: ";
							cin >> ID;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						else if(op == 2)
						{
							cout << "new description: ";
							getline(cin, Description);
						}
						else if(op == 3)
						{
							cout << "new category: ";
							getline(cin, Category);
						}
						else if(op == 4)
						{
							cout << "new sub_category: ";
							getline(cin, Sub_category);
						}
						else if(op == 5)
						{
							ss << ID << ':' << Description << ':' << Category << ':' << Sub_category;
							write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
						}
						else if(op == 6)
						{
							write(fd, "", strlen("") + 1);
						}
					}
				}
				else
					cout << buf;
			}
			else if(str[0] == 'e')
			{
				ss << str[0];
				char op = '0';
				while(op > 'h' || op < 'a')
				{
					cout << "\n\ta) by category\n\tb) by sub_category\n\tc) by date\n\td) by Price range(asc)\n\te) by Price range(dsc)\n\tf) by quantity range(asc)\n\tg) by quantity range(dsc)\n\th) back to main menu\n\tenter option: ";
					cin >> op;
					cin.clear();
					cin.ignore(9999, '\n');
					if(op == 'a' || op == 'b' || op == 'c')
					{
						if(op == 'a')
							cout << "category name: ";
						else if(op == 'b')
							cout << "sub_category name: ";
						else if(op == 'c')
							cout << "enter date(dd-mm-yy): ";
						getline(cin, str);
						ss << ':' << op << ':' << str;
						write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
						readLine (fd, buf);
						cout << buf;
					}
					else if(op == 'd' || op == 'e' || op == 'f' || op == 'g')
					{
						double from, to;
						if(op == 'd' || op == 'e')
						{
							cout << "from $: ";
							cin >> from;
							cin.clear();
							cin.ignore(9999, '\n');
							cout << "to $: ";
							cin >> to;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						else
						{
							cout << "from : ";
							cin >> from;
							cin.clear();
							cin.ignore(9999, '\n');
							cout << "to : ";
							cin >> to;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						ss << ':' << op << ':' << from << ',' << to;
						write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
						readLine (fd, buf);
						cout << buf;
					}
					else if(op != 'h')
						cout << "invalid selection!" << endl;
				}
			}
			else if(str[0] == 'f' || str[0] == 'g' || str[0] == 'h' || str[0] == 'i')
			{
				ss << str[0]; 
				string from, till;
				cout << "From date(dd-mm-yy): ";
				getline(cin, from);
				ss << ':' << from;
				cout << "till date(dd-mm-yy): ";
				getline(cin, till);
				ss << ':' << till;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				cout << buf;
			}
			else if(str[0] == 'j' || str[0] == 'k')
			{
				ss << str[0];
				int ID, qty;
				string Description, Category, Sub_category, date;
				double amtPerUnit;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				cout << "Description: ";
				getline(cin, Description);
				ss << ':' << Description;
				cout << "Category: ";
				getline(cin, Category);
				ss << ':' << Category;
				cout << "Sub_category: ";
				getline(cin, Sub_category);
				ss << ':' << Sub_category;
				cout << "amount per unit: ";
				cin >> amtPerUnit;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << amtPerUnit;
				cout << "quantity: ";
				cin >> qty;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << qty;
				cout << "Date(dd-mm-yy): ";
				getline(cin, date);
				ss << ':' << date;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				cout << buf;
			}
			else if(str[0] == 'l')
			{
				ss << str[0];
				int ID, qty;
				string Description, Category, Sub_category, date;
				double amtPerUnit;
				cout << "ID: ";
				cin >> ID;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << ID;
				cout << "Description: ";
				getline(cin, Description);
				ss << ':' << Description;
				cout << "Category: ";
				getline(cin, Category);
				ss << ':' << Category;
				cout << "Sub_category: ";
				getline(cin, Sub_category);
				ss << ':' << Sub_category;
				cout << "amount per unit: ";
				cin >> amtPerUnit;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << amtPerUnit;
				cout << "quantity: ";
				cin >> qty;
				cin.clear();
				cin.ignore(9999, '\n');
				ss << ':' << qty;
				cout << "Date(dd-mm-yy): ";
				getline(cin, date);
				ss << ':' << date;
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
				readLine (fd, buf);
				if(!strcmp(buf, "found"))
				{
					int op = 0;
					ss.str("");
					while(op != 8 && op != 9)
					{
						cout << "\t1) update ID\n\t2) update Description\n\t3) update Category\n\t4) update Sub_category\n\t5) update amtPerUnit\n\t6) update quantity\n\t7) update date\n\t8) save and return\n\t9) return without saving\n\tenter option: ";
						cin >> op;
						cin.clear();
						cin.ignore(9999, '\n');
						if(op == 1)
						{
							cout << "new ID: ";
							cin >> ID;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						else if(op == 2)
						{
							cout << "new description: ";
							getline(cin, Description);
						}
						else if(op == 3)
						{
							cout << "new category: ";
							getline(cin, Category);
						}
						else if(op == 4)
						{
							cout << "new sub_category: ";
							getline(cin, Sub_category);
						}
						else if(op == 5)
						{
							cout << "new amount per unit: ";
							cin >> amtPerUnit;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						else if(op == 6)
						{
							cout << "new quantity: ";
							cin >> qty;
							cin.clear();
							cin.ignore(9999, '\n');
						}
						else if(op == 7)
						{
							cout << "new Date(dd-mm-yy): ";
							getline(cin, date);
						}
						else if(op == 8)
						{
							ss << ID << ':' << Description << ':' << Category << ':' << Sub_category << ':' << amtPerUnit << ':' << qty << ':' << date;
							write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
							readLine (fd, buf);
							cout << buf;
						}
						else if(op == 9)
						{
							write(fd, "", strlen("") + 1);
						}
					}
				}
				else
					cout << buf;
			}
			else if(str[0] == 'm')
			{
				ss << str[0];
				write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
			}
			else
				cout << "invalid choice!" << endl;
			ss.str("");
		}
	}
	if(loginCount >= 3)
	{
		cout << "maximum tries exceeded" << endl;
		ss << 'm';
		write(fd, ss.str().c_str(), strlen(ss.str().c_str()) + 1);
	}
	printf("\n*** Thank you for using, have a nice day! ***\n");
}

 /**************************************************************/

int main  ()
 {
   int clientFd, serverLen, result;
   struct sockaddr_un serverAddress;
   struct sockaddr* serverSockAddrPtr;
   serverSockAddrPtr = (struct sockaddr*)&serverAddress;
   serverLen = sizeof (serverAddress);

   /* Create a socket, bidirectional, default protocol */
   clientFd = socket (AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);
   serverAddress.sun_family = AF_LOCAL; /* Server domain */
   strcpy (serverAddress.sun_path, "recipe"); /* Server name */
   do /* Loop until a connection is made with the server */
     {
       result = connect (clientFd, serverSockAddrPtr, serverLen);
	   printf("attempting to connect\n");
       if (result == -1) sleep (1); /* Wait and then try again */
     }
   while (result == -1);
   com(clientFd); /* Read the recipe */
   close (clientFd); /* Close the socket */
   exit (/* EXIT_SUCCESS */ 0); /* Done */
 }
