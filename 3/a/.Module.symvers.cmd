cmd_/home/cs695/3/a/Module.symvers :=  sed 's/ko$$/o/'  /home/cs695/3/a/modules.order | scripts/mod/modpost -m     -o /home/cs695/3/a/Module.symvers -e -i Module.symvers -T - 
