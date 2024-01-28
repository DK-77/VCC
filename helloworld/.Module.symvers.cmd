cmd_/home/cs695/helloworld/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/helloworld/modules.order | scripts/mod/modpost -m     -o /home/cs695/helloworld/Module.symvers -e -i Module.symvers -T - 
