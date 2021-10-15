# WEBSERV TESTER
## Configuration
You must configure some server like nginx server configured in directory nginx_config use different ip
In file tester_utils/include.sh  
MODIFY : 
  - WEBSERV_IP
  - WEBSERV_PORT
  - NGINX_IP
  - NGINX_PORT
## Run test
sudo bash tester.sh
Then check output to see all difference between nginx
## Explanation
#### This tester test methods :
  - PUT
  - GET
  - HEAD
  - POST
  - DELETE  
#### This tester test keyword : 
  - return
  - auth_basic
  - auth_basic_user_file
  - location
  - autoindex
  - listen
  - root
  - error_page
  - server_name
