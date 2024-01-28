cmd_/home/cs695/1.1/4/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/1.1/4/modules.order | scripts/mod/modpost -m     -o /home/cs695/1.1/4/Module.symvers -e -i Module.symvers -T - 
