#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc,char* argv[])
{
   if (argc!=2) {
      cerr << "usage: " << argv[0] << endl;
      return 1;
   }

   ifstream in(argv[1]);
   string line;
   unsigned sum=0;
   while (getline(in,line)) {
      const char* data=line.data(),*limit=data+line.length(),*last=data;
      unsigned field=0;
      for (auto iter=data;iter!=limit;++iter) {
         if ((*iter)=='|') {
            if (++field==5) {
               unsigned v=0;
               for (auto iter2=last;iter2!=iter;++iter2)
                  v=10*v+(*iter2)-'0';
               sum+=v;
               break;
            } else {
               last=iter+1;
            }
         }
      }
   }
   cout << sum << endl;
}
