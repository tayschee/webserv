#ifndef EXCHANGE_HPP
# define EXCHANGE_HPP

# include "message/message.hpp"
# include <queue>

class message;
/*parent class of receive and send, it does nothing just here to allow polymorphism*/
class	message::exchange
{
	protected :
		int fd;

	public :
		virtual int	operator()() = 0;

	public :
		exchange(int fd = -1);
		exchange(const exchange &x);
		exchange &operator=(const exchange &x);
		virtual ~exchange();
};


/*child class of exchange_magemenent, it will be use to receive*/
class message::receive : public exchange
{
	protected :
		class body;

		class header
		{
			protected :
				size_t		buf_size;
				std::string	msg; //to store header to form of string during parsing

			public :
				
				virtual int			receive(const int socket);
				virtual int			check();
				virtual header		*clone() const;
				virtual std::string	get_buffer();
				body		*next_step();
				void		set_buf_size(size_t buf_size);

			public :
				header(const size_t buf_size = 0, const std::string &msg = "");
				header(const header &x);
				header &operator=(const header &x);

				virtual ~header();
		};

		class body : public header
		{
			protected :
				//size_t		buf_size;
				//std::string	msg; //to store message to form of string during parsing
				size_t			pos;

			public :
				virtual int			receive(const int socket) = 0;
				virtual int			check() = 0;
				virtual body		*clone() const = 0;

			public :
				std::string	get_header_buffer();
				std::string	get_buffer();
				header		*previous_step(const size_t buf_size) const;

			public :
				body(const std::string &msg = "", const size_t buf_size = 0, const size_t pos = 0);
				body(const body &x);
				body &operator=(const body &x);

				virtual ~body();
		};

		class cl_body : public body
		{
			private :
				//size_t		buf_size;
				//std::string	msg; //to store message to form of string during parsing
				//size_t			pos;

			public :
				int			receive(const int socket);
				int			check();
				cl_body		*clone() const;

			public :
				cl_body(const std::string &msg = "", const size_t buf_size = 0, size_t body_begin = 0);
				cl_body(const cl_body &x);
				cl_body &operator=(const cl_body &x);

				virtual ~cl_body();
		};

		class tf_body : public body
		{
			public :
				bool			msg_begin;
				//size_t		buf_size; //store the size of body or rest of size
				//std::string	msg; //to store body during parsing
				//size_t		pos; //position of next buf_size
	
			private :
				int					check_end(const size_t i, const size_t CRLF_size);
				size_t				erase_tf_signature(const size_t i, const size_t CRLF_size, size_t pos);

			public :
				int					receive(const int socket);
				int					check();
				tf_body				*clone() const;

			public :
				tf_body(const std::string &msg = "", const size_t pos = 0);
				tf_body(const tf_body &x);
				tf_body &operator=(const tf_body &x);

				~tf_body();
		};

	public :
		enum {NOTHING_END, HEADER_END, BODY_END, MESSAGE_END};
		enum {HEADER_MASK = 1, BODY_MASK = 2};
	private :
		//int				fd;
		receive::header		*data;
		size_t				buf_size;

	private :
		receive::header *clone() const;
		void			clear();

	public :
		void			set_buf_size(size_t buf_size);
		void			reset(int fd, size_t buf_size);
		int				check();
		std::string		get_header_buffer();
		std::string		get_buffer();
		void			prepare_next();
		int	operator()(void);

	public :
		receive(size_t fd = -1, size_t read_size = 10);
		receive(const receive &x);
		receive &operator=(const receive &x);

		~receive();
};

class message::send : public message::exchange
{
	private:
		class internal
		{

			private :
				int			status;
				size_t		buf_size;
				std::string msg; //to store header to form of string during parsing / other utility for child class

			public :
				int	get_status() const;
				size_t get_buf_size() const;
				const std::string &get_msg() const;

			public :
				internal(int status = 1, size_t buf_size = 0, const std::string &msg = "");
				internal(const message &msg, size_t buf_size);
				internal(const internal &x);
				internal &operator=(const internal &x);

				virtual ~internal();
		};

	private :
		//int fd
		std::queue<internal>	queue;

	private: 
		static int find_status_value(const message &msg);

	public :
		int		operator()();
		void	push(const message &msg, const size_t buf_size);

	public :
		send(const std::string &msg = "", const int status = 1, const size_t buf_size = 0, const int fd = -1);
		send(const message &msg, const size_t buf_size = 0, const int fd = -1);
		send(const send &x);
		send &operator=(const send &x);

		~send();
};

#endif