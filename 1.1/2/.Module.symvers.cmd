cmd_/home/cs695/1.1/2/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/1.1/2/modules.order | scripts/mod/modpost -m     -o /home/cs695/1.1/2/Module.symvers -e -i Module.symvers -T - 
