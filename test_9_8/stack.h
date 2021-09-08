#include<iostream>
#include<deque>
using namespace std;
namespace hello
{
   template< typename T,typename Contain=deque<T> >
  class stack{
    public:
      stack()
      {

      }
      void push(int val)
      {
        _c.push_back(val);
      }
      T&top()
      {
        return _c.back();
      }
      void pop()
      {
        _c.pop_back();
      }
      size_t size()
      {
        return _c.size();
      }
      bool empty()
      {
        return _c.empty();
      }
    private:
      Contain _c;
  };
}
