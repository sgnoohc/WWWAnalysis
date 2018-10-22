for f in $(ls /nfs-7/userdata/phchang/WWW_babies/WWW2017_v4.0.2/skim/*.root); do
    echo "Launched a job for $f"
    ./doAnalysis $f t outputs/$(basename $f) -1 > outputs/$(basename $f).log 2>&1 &
done

wait
