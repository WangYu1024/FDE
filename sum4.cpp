#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <emmintrin.h>

using namespace std;

static unsigned computeSum(const unsigned* fileBegin, const unsigned* fileEnd)
{
   unsigned sum=0;
   for (auto iter=fileBegin;iter!=fileEnd;++iter)
      sum+=*iter;
   return sum;
}

static void convertToBinary(const char* inFileName, const char* outFileName)
{
   ifstream in(inFileName);
   ofstream out(outFileName);
   string line;
   while (getline(in,line)) {
      const char* data=line.data(),*limit=data+line.length(),*last=data;
      unsigned field=0;
      for (auto iter=data;iter!=limit;++iter) {
         if ((*iter)=='|') {
            if (++field==5) {
               union { unsigned v; char buffer[sizeof(unsigned)]; };
               v=0;
               for (auto iter2=last;iter2!=iter;++iter2)
                  v=10*v+(*iter2)-'0';
               out.write(buffer,sizeof(unsigned));
               break;
            } else {
               last=iter+1;
            }
         }
      }
   }
}

int main(int argc,char* argv[])
{
   if (argc<2) {
      cerr << "usage: " << argv[0] << endl;
      return 1;
   }
   if (argc==3) {
      convertToBinary(argv[1], argv[2]);
      return 0;
   }



   int handle=open(argv[1],O_RDONLY);
   if (handle<0) {
      cerr << "unable to open " << argv[1] << endl;
      return 1;
   }
   long fileSize = lseek(handle,0,SEEK_END)/sizeof(unsigned);
   auto fileBegin = static_cast<const unsigned*>(mmap(nullptr,fileSize*sizeof(unsigned),PROT_READ,MAP_SHARED,handle,0));

   atomic<unsigned> sum(0);
   unsigned threadCount = thread:: hardware_concurrency();
   auto getThreadBounds = [fileBegin,fileSize,threadCount](unsigned index) {
      auto cut=fileBegin+(index*fileSize/threadCount);
      return cut;
   };
   vector<thread> threads;
   for (unsigned index=0;index!=threadCount;++index) {
      threads.push_back(thread([&sum, &getThreadBounds, index]() {
         sum += computeSum(getThreadBounds(index), getThreadBounds(index+1));
      }));
   }
   for (auto& t:threads) t.join();
   cout << sum.load() << endl;
}
