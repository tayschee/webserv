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
COPY minion.gif /var/www/html/minion.gif
COPY minion.jpeg /var/www/html/minion.jpeg
COPY minion.png /var/www/html/minion.png
COPY giphy.mp4 /var/www/html/giphy.mp4
COPY giphy.vid /var/www/html/giphy.vid
COPY giphy.gif /var/www/html/giphy.gif


RUN chmod 777 /var/www/html/info.php \
&& chmod 777 /var/www/html/test.php \
&& chmod 777 /var/www/html/long.php \
&& chmod 777 /var/www/html/long.html \
&& chmod 777 /var/www/html/minion.gif \
&& chmod 777 /var/www/html/minion.jpeg \
&& chmod 777 /var/www/html/minion.png \
&& chmod 777 /var/www/html/giphy.mp4 \
&& chmod 777 /var/www/html/giphy.vid \
&& chmod 777 /var/www/html/giphy.gif


CMD service php7.3-fpm restart \
&& service nginx restart \
&& bash
