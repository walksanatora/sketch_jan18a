#!/usr/bin/env fish
echo -en "const static byte AlphabetLUT[256] = {\n\t"

rm chars log 2>/dev/null

for char in (cat font.h | grep -oe '\/\/ .*' 2>/dev/null | awk '{print $2 " " $4}')
    set byte (echo $char | awk '{print $1}')
    set idx (./util/h2b "0x$byte")
    set new (echo $char | awk '{print $2}')
    echo -e "char$idx\t$new" >>chars
end

set alarm 16
set cma ","
itr=0 while test $itr -lt '256'
    if test "$alarm" -lt 1;
        echo -ne "\n\t"
        set alarm 16
    end
    if test "$itr" -eq 255;
        set -e cma
    end
    if cat chars | grep -w "char$itr" >/dev/null
        echo "mapped: $itr to "(cat chars | grep char$itr | head -n1 | awk '{print $2}') >> log
        echo -n (cat chars | grep -w char$itr | head -n1 | awk '{print $2}')"$cma "
    else
        echo -n "31$cma "
    end
    set itr (math "$itr + 1")
    set alarm (math "$alarm - 1")
end
echo -en "\n};"
