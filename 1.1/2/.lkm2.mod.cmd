cmd_/home/cs695/1.1/2/lkm2.mod := printf '%s\n'   lkm2.o | awk '!x[$$0]++ { print("/home/cs695/1.1/2/"$$0) }' > /home/cs695/1.1/2/lkm2.mod
