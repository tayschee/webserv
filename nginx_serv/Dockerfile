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

COPY default.conf /etc/nginx/conf.d/default.conf
COPY test.php /var/www/html/test.php
COPY info.php /var/www/html/info.php
COPY long.php /var/www/html/long.php
COPY long.html /var/www/html/long.html

RUN chmod 777 var/www/html/info.php \
&& chmod 777 var/www/html/test.php \
&& chmod 777 var/www/html/long.php \
&& chmod 777 var/www/html/long.html


CMD service php7.3-fpm restart \
&& service nginx restart \
&& bash
