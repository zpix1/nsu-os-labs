echo "using $1 as test path"
#gcc lsld.c -o lsld
./lsld $1 > my
ls -ld $1 > they
diff my they
if (( $? == 0 ))
then 
        echo "ALL OK";
    else
        echo "VERY VERY BAD";
fi
