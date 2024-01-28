cmd_/home/cs695/3/b/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/3/b/modules.order | scripts/mod/modpost -m     -o /home/cs695/3/b/Module.symvers -e -i Module.symvers -T - 
