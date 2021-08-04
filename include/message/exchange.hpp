#ifndef EXCHANGE_HPP
# define EXCHANGE_HPP

# include "message/message.hpp"
# include <queue>

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MIN_BUFFER 131072

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

				int		get_size()
				{
					return msg.size();
				}

				virtual ~header();
		};

		class body : public header
		{
			protected :
				size_t			pos;

			public :
				virtual int			receive(const int socket) = 0;
				virtual int			check() = 0;
				virtual body		*clone() const = 0;

			public :
				std::string	get_header_buffer();
				virtual std::string	get_buffer() = 0;
				header		*previous_step(const size_t buf_size) const;

			public :
				body(const std::string &msg = "", const size_t buf_size = 0, const size_t pos = 0);
				body(const body &x);
				body &operator=(const body &x);

				virtual ~body();
		};

		class cl_body : public body
		{
			public :
				int			receive(const int socket);
				int			check();
				cl_body		*clone() const;
				std::string get_buffer();

			public :
				cl_body(const std::string &msg = "", const size_t buf_size = 0, size_t body_begin = 0);
				cl_body(const cl_body &x);
				cl_body &operator=(const cl_body &x);

				~cl_body();
		};

		class tf_body : public body
		{
			public :	
				bool			msg_begin;
	
			private :
				int					check_end(const size_t i, const size_t CRLF_size);
				size_t				erase_tf_signature(const size_t i, const size_t CRLF_size, size_t pos);
				std::string         get_buffer();

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
		enum {NOTHING_END, HEADER_END, BODY_END = 3};
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
		int	operator()();

		int		get_size();

	public :
		receive(size_t fd = -1, size_t read_size = 10);
		receive(const receive &x);
		receive &operator=(const receive &x);

		~receive();
};

#endif