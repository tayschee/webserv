FROM debian:latest

RUN apt update \
&& apt install -y vim

# nginx config
RUN apt install -y nginx-full \
&& rm /etc/nginx/sites-enabled/default

# php config
RUN apt install -y php-fpm \
&& service php7.3-fpm start 

RUN touch /etc/nginx/conf.d/default.conf

#server page
VOLUME ["/srcs"] 

#nginx config
VOLUME ["/etc/nginx/conf.d"]

CMD service php7.3-fpm restart \
&& service nginx restart \
&& bash
