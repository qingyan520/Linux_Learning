#include"stack.h"
int main()
{
  hello::stack<int,deque<int> >st;
      st.push(1);
      st.push(2);
      st.push(3);
      st.push(4);
      while(!st.empty())
      {
        cout<<st.top()<<endl;
        st.pop();
      }
      return 0;
}
