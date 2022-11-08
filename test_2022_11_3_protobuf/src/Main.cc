#include"test.pb.h"
#include<iostream>
#include<string>
#include<iostream>
using namespace std;
int main(){
  test::LoginRequest req;
  req.set_name("hello");
  req.set_pwd("123456");
  std::string send_msg;
  if(req.SerializeToString(&send_msg))
  {
    std::cout<<send_msg<<std::endl;
  }

  // test::User user1;
  // user1.set_name("张三");
  // user1.set_age(20);
  // user1.set_sex(test::User::Sex::User_Sex_MAN);
  // string msg;
  // if(user1.SerializeToString(&msg))
  // {
  //   cout<<msg<<endl;
  //   cout<<user1.age()<<endl;
  // }
  // test::GetFriendList lists;
  // test::Result*res=lists.mutable_result();
  // res->set_errmsg("1");
  // res->set_errcode("hello world");

  // auto t1=lists.add_friend_lists();
  // t1->set_name("张三");
  // t1->set_age(20);
  // t1->set_age(test::User::Sex::User_Sex_MAN);
  // string buf;
  // if(lists.SerializeToString(&buf))
  // {
  //   cout<<buf<<endl;
  // }
  // auto t2=lists.add_friend_lists();
  // t2->set_name(("李四"));
  // t2->set_age(30);
  // t2->set_sex(test::User::Sex::User_Sex_WOMAN);
  // if(lists.SerializeToString(&buf))
  // {
  //   cout<<buf<<endl;
  // }
  // cout<<lists.friend_lists_size()<<endl;

}
