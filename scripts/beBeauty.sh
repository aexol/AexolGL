#!/bin/bash
declare -a arr=("*.cpp" "*.hpp" "*.h")
for i in "${arr[@]}"
do
    find ./ -name "${i}" -exec astyle -n --break-blocks --pad-oper --unpad-paren --align-pointer=name --align-reference=name --add-brackets --close-templates --break-after-logical --align-pointer=type --align-reference=type --indent-namespaces -A1 {} \;
done
