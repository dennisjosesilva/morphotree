echo "echo Restoring environment" > "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanrunenv-release-x86_64.sh"
for v in LD_LIBRARY_PATH DYLD_LIBRARY_PATH
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanrunenv-release-x86_64.sh"
    else
        echo unset $v >> "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanrunenv-release-x86_64.sh"
    fi
done


export LD_LIBRARY_PATH="/home/dennis/.conan2/p/morphb75acff3f35fd/p/lib:$LD_LIBRARY_PATH"
export DYLD_LIBRARY_PATH="/home/dennis/.conan2/p/morphb75acff3f35fd/p/lib:$DYLD_LIBRARY_PATH"