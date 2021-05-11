FROM nginx:latest

COPY default.conf /etc/nginx/conf.d/default.conf

RUN chmod 777 /etc/nginx/conf.d/default.conf \
&&	chmod 777 etc \
&&	chmod 777 etc/nginx \
&&	chmod 777 etc/nginx/conf.d \
&&	chmod 777 /usr \
&&	chmod 777 /usr/share \
&&	chmod 777 /usr/share/nginx \
&&	chmod 777 /usr/share/nginx/html \
&&	chmod 777 /usr/share/nginx/html/50x.html \
&&	chmod 777 /usr/share/nginx/html/index.html


#CMD bash