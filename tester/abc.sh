test()
{
    
    test2 "$1"
}

test2()
{
    echo $1
    curl -i "$1" 127.0.0.1:80/
}

test "-i -X OPTIONS"

