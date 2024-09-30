//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

class printer
{
public:
  printer(boost::asio::io_context& io)
    : strand_(boost::asio::make_strand(io)),
      timer1_(io, boost::asio::chrono::seconds(1)),
      timer2_(io, boost::asio::chrono::seconds(1)),
      count_(0)
  {
    timer1_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&printer::print1, this)));

    timer2_.async_wait(boost::asio::bind_executor(strand_,
          std::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print(
    boost::asio::steady_timer printer::*timer,
    const std::string &name,
    std::chrono::duration<long, std::ratio<1, 1000>> dur
  )
  {
    if (count_ < 10)
    {
      std::cout << name << count_ << std::endl;
      ++count_;

      (this->*timer).expires_at((this->*timer).expiry() + dur);

      (this->*timer).async_wait(
        boost::asio::bind_executor(strand_,
        std::bind(&printer::print, this, timer, name, dur)
      ));
    }
  }

  void print1()
  {
    print(&printer::timer1_, "Timer 1:", boost::asio::chrono::milliseconds(700));
  }

  void print2()
  {
    print(&printer::timer2_, "Timer 2:", boost::asio::chrono::seconds(1));
  }

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};

int main()
{
  boost::asio::io_context io;
  printer p(io);
  std::thread t([&]{ io.run(); });
  io.run();
  t.join();

  return 0;
}