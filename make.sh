INCLUDE=/Users/tanqiang/ace_install/include
LINK=/Users/tanqiang/ace_install/lib
g++ -o server server.cpp -I $INCLUDE -L$LINK -lACE -g
g++ -o client client.cpp -I $INCLUDE -L$LINK -lACE -g
