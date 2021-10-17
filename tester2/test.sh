my_cgi=$(grep "monsite" /etc/hosts)
echo $my_cgi
if [[ -z "$my_cgi" ]]; then
    echo -e "127.0.0.1\tmonsite" >> /etc/hosts
fi
sudo chmod 000 ../www/html/private
sudo chmod 000 ../www/html/private2/index.html
sudo chmod 000 ../www/html/private3
cd .. && make && ./webserv conf && cd tester2
sudo chmod 755 ../www/html/private
sudo chmod 755 ../www/html/private2/index.html
sudo chmod 755 ../www/html/private3
sudo chmod 755 ../www/html/private3/index.html