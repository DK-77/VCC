cmd_/home/cs695/1.1/3/lkm3.mod := printf '%s\n'   lkm3.o | awk '!x[$$0]++ { print("/home/cs695/1.1/3/"$$0) }' > /home/cs695/1.1/3/lkm3.mod
