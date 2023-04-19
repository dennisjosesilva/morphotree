echo "echo Restoring environment" > "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanbuildenv-release-x86_64.sh"
for v in 
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanbuildenv-release-x86_64.sh"
    else
        echo unset $v >> "/mnt/HDD-Ubuntu/documents/phd/code/simple-morph-tree/python/morphotreepy/deactivate_conanbuildenv-release-x86_64.sh"
    fi
done

