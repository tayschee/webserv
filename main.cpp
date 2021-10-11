#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main()
{
    long i = 0;
    // int fd = open("/home/user42/save_current3/www/html/very_long_file.php", O_CREAT | O_RDWR);

    int fd = open("/home/user42/save_current3/www/html/very_long_file.php", O_RDWR | O_CREAT, 0666);

    if (fd < 0)
    {
        std::cout << "PROBLEM" << std::endl;
    }
//         exit(0);

    std::string s = "<html>\n<head>\n<title>Test PHP</title>\n</head>\n<body>\n<?php echo '<p>";
    write(fd, s.c_str(), s.size());

    s = "Bonjour le monde";
    
    while (i++ < 1000000)
    {
       write(fd, s.c_str(), s.size());
    }

    s = "</p>'; ?>\n</body>\n</html>";
    
    write(fd, s.c_str(), s.size());
    close(fd);
   
    return 0;
}