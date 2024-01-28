cmd_/home/cs695/helloworld/helloworld.mod := printf '%s\n'   helloworld.o | awk '!x[$$0]++ { print("/home/cs695/helloworld/"$$0) }' > /home/cs695/helloworld/helloworld.mod
