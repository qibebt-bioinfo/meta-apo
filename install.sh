###Meta-Apo installer
###Bioinformatics Group, Single-Cell Research Center, QIBEBT, CAS
###Updated at Nov. 07, 2019 
###Updated by Xiaoquan Su, Honglei Wang, Gongchao Jing
#!/bin/bash
##Users can change the default environment variables configuration file here
PATH_File=~/.bashrc
if [ ! -f "$PATH_File" ]
   then
       PATH_File=~/.bash_profile
fi
DM_PATH=`pwd`
Sys_ver=`uname`
###Checking that environment variable of Meta-Apo exists###
Check_old_pm=`grep "export MetaApo"  $PATH_File|awk -F '=' '{print $1}'`
Check_old_path=`grep "MetaApo/bin"  $PATH_File |sed 's/\(.\).*/\1/' |awk '{if($1!="#"){print "Ture";}}'`
Add_Part="####DisabledbyMeta-Apo####"
echo "**Meta-Apo Installation**"

###Build source code for src package###
if [ -f "Makefile" ]
   then
       echo -e "\n**Meta-Apo src package**"
       make
       echo -e "\n**Build Complete**"
else
   echo -e "\n**Meta-Apo bin package**"
fi
###Configure environment variables###

if [ "$Check_old_pm" != "" ]
   then
      Checking=`grep ^export\ MetaApo  $PATH_File|awk -F '=' '{print $2}'`
      if [ "$Checking" != "$DM_PATH" ]
         then
         if [ "$Sys_ver" == "Darwin" ]
            then
            sed -i "" "s/^export\ MetaApo/$Add_Part\ &/g" $PATH_File
            sed -i "" -e $"`grep -n "$Add_Part" $PATH_File | cut -d ":" -f 1 | head -1` a\ 
            export\ MetaApo=$DM_PATH
            " $PATH_File
         else
             sed -i "s/^export\ MetaApo/$Add_Part\ &/g" $PATH_File
             sed -i "/$Add_Part\ export\ MetaApo/a export\ MetaApo=$DM_PATH" $PATH_File
         fi
     fi    
elif [ "$Check_old_pm" == "" ]
    then
      echo "export MetaApo="${DM_PATH} >> $PATH_File
fi
if [ "$Check_old_path" == "" ]
    then
      echo "export PATH=\$PATH:\$MetaApo/bin" >> $PATH_File
fi
###Source the environment variable file###
source $PATH_File
echo -e "\n**Environment Variables Configuration Complete**"
###End
echo -e "\n**Meta-Apo Installation Complete**"
echo -e "\n**An example dataset with demo script is available in \"example\"**"


