#include <iostream>
#include <set>
#include <thread>
#include <deque>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "string.h"
#include "version.h"
#include "parser.h"

using namespace std;
using boost::asio::ip::tcp;

class join_participant
{
public:
    virtual ~join_participant() {}
    virtual void deliver(const std::string& msg) = 0;
};

using chat_message_queue = std::deque<std::string>;
using join_participant_ptr = std::shared_ptr<join_participant>;

class join_room
{
public:

    void join(join_participant_ptr participant)
    {
        participants_.emplace(participant);
    }

    void leave(join_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const join_participant_ptr participant, std::string& msg)
    {
        //std::cout << std::this_thread::get_id() << " : ";
        std::string result = parser_.parse_input(msg);
        participant->deliver(result);
    }

private:
    std::set<join_participant_ptr> participants_;
    parser parser_;
};

class join_session
        : public join_participant,
          public std::enable_shared_from_this<join_session>
{
public:
    join_session(tcp::socket socket, join_room& room)
        : socket_(std::move(socket)),
          room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_message();
    }

    void deliver(const std::string& msg)
    {
      bool write_in_progress = !write_msgs_.empty();
      write_msgs_.push_back(msg);
      if (!write_in_progress)
      {
        do_write();
      }
    }

private:
    void do_read_message()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(str),
                                [this](const boost::system::error_code & err, size_t bytes)->size_t
        {
            if ( err) return 0;
            bool found = std::find(str, str + bytes, '\n') < str + bytes;
            str[bytes] = '\0';
            return found ? 0 : 1;
        },
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::string s = string{str};
                s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
                room_.deliver(shared_from_this(), s);

                do_read_message();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    tcp::socket socket_;
    join_room &room_;
    chat_message_queue write_msgs_;
    char str[512];
};

class join_server
{
public:
    join_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint),
          socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::make_shared<join_session>(std::move(socket_), room_)->start();
            }
            else
            {
                cout << "accept error " << ec.message() << endl;
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    join_room room_;
};

int main(int argc, char* argv[])
{
    try
    {
        int port = 0;

        if ((argc > 1) &&
                (!strncmp(argv[1], "-v", 2) || !strncmp(argv[1], "--version", 9)))
        {
            cout << "version " << version() << endl;
            return 0;
        }
        else if (argc == 2)
        {
            port = atoi(argv[1]);
            //cout << "join_server starting on port: " << port << endl;
        }
        else
        {
            std::cerr << "Usage: join_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), port);

        join_server server(io_service, endpoint);

        vector<thread> thread_pool;
        for(int i = 0; i < 3; ++i)
        {
            thread_pool.emplace_back(thread([&] {io_service.run();}));
        }

        for (auto &thr : thread_pool)
        {
            thr.join();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
