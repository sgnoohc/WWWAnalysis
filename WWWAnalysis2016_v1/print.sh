for i in $(ls eventlists/*); do 
    cat $i | tail -n+3 | awk '{print $1, $2, $3}' | tr ' ' ':' | tr '.' ' ' | awk '{print $1}' | sort -g > ${i/.txt/.sorted}
done
