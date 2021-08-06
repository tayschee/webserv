func()
{
    echo $#
}

func2()
{
   func ${@:1:1}
}

func 1 2 3 4 5 6
func2 1 2 3 4 5 6