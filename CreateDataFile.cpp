#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;
void upper(string &a)
{
	for(int i = 0; i < a.size(); i++)
		a[i] = toupper(a[i]);
}
int main(int argc, char** argv)
{
	string buf;
	fstream file(argv[1], ios::in);
	if(!file)
		cerr << "file not found" << endl;
	else
	{
		fstream out("Data.txt", ios::out|ios::binary);
		if(!out)
			cerr << "output file io" << endl;
		else
		{
			while(getline(file,buf))
			{
				if(buf.size() > 1)
				{
					upper(buf);
					out.write(buf.c_str(), strlen(buf.c_str())+1);
			
				}
			}
		}
	
		out.close();
	}
	
	file.close();
}