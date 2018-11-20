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

static constexpr uint64_t broadcast8(char symbol)
{
   uint64_t result=0;
   for (unsigned index=0;index!=8;index++)
      result=(result<<8)|(symbol&0xFF);
   return result;
}

template <char symbol>
static inline const char* findSymbol(const char* iter,const char* fileEnd)
{
#if 1
   auto safeFileEnd = fileEnd - 16;
   auto pattern = _mm_set1_epi8(symbol);
   for (;iter<safeFileEnd;iter+=16) {
      auto block=_mm_loadu_si128(reinterpret_cast<const __m128i*>(iter));
      auto matchVec=_mm_cmpeq_epi8(block,pattern);
      auto matches=_mm_movemask_epi8(matchVec);

      if (matches)
         return iter + (__builtin_ctzll(matches));
   }
#else
   auto safeFileEnd = fileEnd - 8;
   for (;iter<safeFileEnd;iter+=8) {
      auto block=*reinterpret_cast<const uint64_t*>(iter);
      constexpr uint64_t high = 0x8080808080808080, low=~high;
      constexpr uint64_t pattern = broadcast8(symbol);
      auto lowChars = (~block)&high;
      auto potentialMatches = ~((((block^pattern)&low)+low)&high);
      auto matches = potentialMatches&lowChars;

      if (matches)
         return iter + (__builtin_ctzll(matches)>>3);
   }
#endif
   while ((iter<fileEnd)&&((*iter)!=symbol)) ++iter;
   return iter;
}

template <char symbol>
static inline const char* findNSymbols(const char* iter,const char* fileEnd,unsigned count)
{
   auto safeFileEnd = fileEnd - 16;
   auto pattern = _mm_set1_epi8(symbol);
   for (;iter<safeFileEnd;iter+=16) {
      auto block=_mm_loadu_si128(reinterpret_cast<const __m128i*>(iter));
      auto matchVec=_mm_cmpeq_epi8(block,pattern);
      auto matches=_mm_movemask_epi8(matchVec);

      if (matches) {
         unsigned hits=__builtin_popcountll(matches);
         if (hits<count) {
            count-=hits;
            continue;
         }
         for (;count>1;--count)
            matches=matches&(matches-1);
         return iter + (__builtin_ctzll(matches));
      }
   }
   while (iter<fileEnd)
      if ((*iter)==symbol) {
         if (--count<1)
            return iter;
      }
   return iter;
}

static unsigned computeSum(const char* fileBegin, const char* fileEnd)
{
   unsigned sum=0;
   for (auto line=fileBegin;line<fileEnd;) {
      auto field5=findNSymbols<'|'>(line,fileEnd,4)+1;
      if (field5>=fileEnd) break;
      auto iter=findSymbol<'|'>(field5,fileEnd);
      if (iter>=fileEnd) break;
      unsigned v=0;
      for (auto iter2=field5;iter2!=iter;++iter2)
         v=10*v+(*iter2)-'0';
      sum+=v;
      line=findSymbol<'\n'>(iter,fileEnd)+1;
   }
   return sum;
}

int main(int argc,char* argv[])
{
   if (argc!=2) {
      cerr << "usage: " << argv[0] << endl;
      return 1;
   }
   int handle=open(argv[1],O_RDONLY);
   if (handle<0) {
      cerr << "unable to open " << argv[1] << endl;
      return 1;
   }
   long fileSize = lseek(handle,0,SEEK_END);
   auto fileBegin = static_cast<const char*>(mmap(nullptr,fileSize,PROT_READ,MAP_SHARED,handle,0));

   atomic<unsigned> sum(0);
   unsigned threadCount = thread:: hardware_concurrency();
   auto getThreadBounds = [fileBegin,fileSize,threadCount](unsigned index) {
      auto cut=fileBegin+(index*fileSize/threadCount);
      auto fileEnd=fileBegin+fileSize;
      if ((cut!=fileBegin)&&(cut!=fileEnd)) {
         cut=findSymbol<'\n'>(cut,fileEnd)+1;
      }
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
