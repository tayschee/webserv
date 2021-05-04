#ifndef EXCHANGE_MANAGEMENT_HPP
# define EXCHANGE_MANAGEMENT_HPP

# include "message.hpp"

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
		exchange_management(const internal_exchange *x);
		exchange_management &operator=(const exchange_management &x);
		~exchange_management();
};


/*child class of exchange_magemenent, which is again a parent class.
It be usefull to parse header but not really to parse body */
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
				virtual int		receive(const int socket, message *req) = 0;
				virtual int		check(message *req) = 0;
				virtual internal_receive *clone() const = 0;

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
				int		receive(const int socket, message *req);
				int		check(message *req);
				receive_header *clone() const;

			public :
				receive_header();
				receive_header(size_t buf_size);
				receive_header(const receive_header &x);
				receive_header &operator=(const receive_header &x);

				~receive_header();
		};

		/*child class of internal receive, it is use when header contain Content-Length
		or anything and he is set with default value */
		struct receive_cl : public internal_receive
		{
			public :
				//size_t	buf_size; //store the size of body or rest of size
				//std::string msg; //to store body during parsing
	
			public :
				int				receive(const int socket, message *req);
				int				check(message *req);
				receive_cl		*clone() const;

			public :
				receive_cl();
				receive_cl(const receive_cl &x);
				receive_cl &operator=(const receive_cl &x);
				receive_cl(internal_receive &x, size_t buf_size);

				~receive_cl();
		};

		struct receive_tf : public internal_receive
		{
			public :
				size_t		pos; //position of next buf_size
				//size_t	buf_size; //store the size of body or rest of size
				//std::string msg; //to store body during parsing
	
			public :
				int				receive(const int socket, message *req);
				int				check(message *req);
				receive_tf		*clone() const;

			public :
				receive_tf();
				receive_tf(const std::string &msg);
				receive_tf(const receive_tf &x);
				receive_tf &operator=(const receive_tf &x);
				receive_tf(internal_receive *x);

				~receive_tf();
		};


	private :
		internal_receive *data;

	private :
		int		specialization(message *req);
		internal_receive *clone() const;

	public :
		int			receive(const int socket, message *req);
		void		clear();

	public :
		receive_management();
		receive_management(size_t read_size);
		receive_management(const receive_management &x);
		receive_management(const receive_management *x);
		receive_management &operator=(const receive_management &x);

		virtual ~receive_management();

};


/*child class of receive_management, to parse body with Transfert-Encoding*/
/*class message::receive_tf : public message::receive_management
{
	private :
		size_t		pos; //position of next buf_size
		//size_t	buf_size; / store the size of body or rest of size /
		//std::string msg; / to store body during parsing  /
	
	public :
		receive_tf();
		~receive_tf();
};*/
		
/*struct send_management : public exchange_management
{
	bool step;

	send_management();
	~send_management();
};*/

#endif