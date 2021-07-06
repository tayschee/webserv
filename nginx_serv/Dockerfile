FROM debian:latest

RUN apt update \
&& apt install -y vim

# nginx config
RUN apt install -y nginx-full \
&& rm /etc/nginx/sites-enabled/default

# php config
RUN apt install -y php-fpm \
&& service php7.3-fpm start

RUN chmod 777 var \
&& chmod 777 var/www \
&& chmod 777 var/www/html

#server page
VOLUME ["/config"] 

#nginx config
VOLUME ["/etc/nginx/conf.d"]

CMD service php7.3-fpm restart \
&& service nginx restart \
&& bash
