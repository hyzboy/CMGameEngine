git clone git://github.com/redis/hiredis.git
cp ./hiredis/*.c ./hiredis/*.h ../src/RedisClient/hiredis
rm ../src/RedisClient/hiredis/test.*
