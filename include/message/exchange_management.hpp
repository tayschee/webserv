#ifndef EXCHANGE_MANAGEMENT_HPP
# define EXCHANGE_MANAGEMENT_HPP

# include "message/message.hpp"

class message;
/*parent class of receive_management and send_management, it does nothing just here to allow polymorphism*/
class	message::exchange_management
{
	protected :
		struct internal_exchange
		{
			public :
				std::string msg; //to store header to form of string during parsing / other utility for child class

			public :
				internal_exchange();
				internal_exchange(const std::string &msg);
				internal_exchange(const internal_exchange &x);
				internal_exchange &operator=(const internal_exchange &x);

				virtual ~internal_exchange();
		};

	public :
		exchange_management();
		exchange_management(const exchange_management &x);
		exchange_management &operator=(const exchange_management &x);
		~exchange_management();
};


/*child class of exchange_magemenent, it will be use to receive*/
class message::receive_management : public exchange_management
{
	protected :
		//class internal_exchange;
		struct internal_receive : public internal_exchange
		{
			public :
				size_t	buf_size; //store the recommend size of read / other utility for child class
				//std::string msg; //to store header to form of string during parsing / other utility for child class

			public : 
				virtual int		receive(const int socket) = 0;
				virtual int		check() = 0;
				virtual internal_receive *clone() const = 0;
				virtual internal_receive *next_step() const = 0;

			public :
				internal_receive();
				internal_receive(size_t buf_size);
				internal_receive(const std::string &msg, const size_t buf_size);
				internal_receive(const internal_receive &x);
				internal_receive &operator=(const internal_receive &x);

				virtual ~internal_receive();
		};

		struct receive_header : public internal_receive
		{
			public :
				//size_t	buf_size; //store the recommend size of read / other utility for child class
				//std::string msg; //to store header to form of string during parsing / other utility for child class

			public :
				int		receive(const int socket);
				int		check();
				receive_header *clone() const;
				internal_receive *next_step() const;

			public :
				receive_header();
				receive_header(size_t buf_size);
				receive_header(const receive_header &x);
				receive_header &operator=(const receive_header &x);

				~receive_header();
		};

		/*child class of internal receive, it is use when header contain Content-Length
		or anything and he is set with default value */
		/*struct receive_cl : public internal_receive
		{
			public :
				//size_t	buf_size; //store the size of body or rest of size
				//std::string msg; //to store body during parsing
	
			public :
				int				receive(const int socket);
				int				check();
				receive_cl		*clone() const;
				internal_receive *next_step() const;

			public :
				receive_cl();
				receive_cl(const receive_cl &x);
				receive_cl(const std::string &msg, const size_t buf);
				receive_cl &operator=(const receive_cl &x);
				receive_cl(internal_receive &x, size_t buf_size);

				~receive_cl();
		};*/

		struct receive_tf : public internal_receive
		{
			public :
				//std::string body;
				size_t		pos; //position of next buf_size
				//size_t	buf_size; //store the size of body or rest of size
				//std::string msg; //to store body during parsing
	
			public :
				int				receive(const int socket);
				int				check();
				receive_tf		*clone() const;
				internal_receive *next_step() const;

			public :
				receive_tf();
				receive_tf(const std::string &msg);
				receive_tf(const receive_tf &x);
				receive_tf(const std::string &msg, const size_t pos);
				receive_tf &operator=(const receive_tf &x);
				receive_tf(internal_receive *x);

				~receive_tf();
		};


	private :
		internal_receive *data;

	private :
		int		next_step();
		internal_receive *clone() const;

	public :
		std::string	get_msg() const;
		int			receive(const int socket);
		void		clear();
		void		reset(size_t read_size);

	public :
		receive_management();
		receive_management(size_t read_size);
		receive_management(const receive_management &x);
		receive_management &operator=(const receive_management &x);

		virtual ~receive_management();

};

class message::send_management : public message::exchange_management
{
	private :
		std::string msg;
		size_t buf_size;

	public :
		send_management();
		send_management(message *msg);
		send_management(message *msg, size_t buf_size);
		send_management(std::string msg, size_t buf_size);
		send_management(const send_management &x);
		send_management &operator=(const send_management &x);

		~send_management();
};

#endif