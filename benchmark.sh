# #!/bin/bash
# output=$(make 2>&1)

# if [[ "output" == "make: Nothing to be done for 'all'." ]]; then
#     echo "$output"
# fi

# if [ -d benchmarks ]; then
#     rm -rf benchmarks
# fi

# mkdir benchmarks

# DIR="benchmarks/ff"
# mkdir $DIR

# echo ""
# echo "******************************"
# echo "** Running benchmark for First Fit**"
# echo "******************************" 

# # run benchmark 5 times
# for i in $(seq 1 5)
# do  
#     echo
#     echo "Benchamrk $i"
#     LD_PRELOAD=lib/libmalloc-ff.so tests/benchmark > $DIR/ff_$i.txt
# done

# echo "" 
# echo "******************************"
# echo "** Running benchmark for Next Fit**"
# echo "******************************" 

# DIR="benchmarks/nf"
# mkdir $DIR

# # run benchmark 5 times
# for i in $(seq 1 5)
# do  
#     echo ""
#     echo "Benchamrk $i"
#     LD_PRELOAD=lib/libmalloc-nf.so tests/benchmark > $DIR/nf_$i.txt
# done

# echo ""
# echo "******************************"
# echo "** Running benchmark for Best Fit**"
# echo "******************************" 

# DIR="benchmarks/bf"
# mkdir $DIR

# # run benchmark 5 times
# for i in $(seq 1 5)
# do  
#     echo ""
#     echo "Benchamrk $i"
#     LD_PRELOAD=lib/libmalloc-bf.so tests/benchmark > $DIR/bf_$i.txt
# done

# echo ""
# echo "******************************"
# echo "** Running benchmark for Worst Fit**"
# echo "******************************" 

# DIR="benchmarks/wf"
# mkdir $DIR

# # run benchmark 5 times
# for i in $(seq 1 5)
# do  
#     echo ""
#     echo "Benchamrk $i"
#     LD_PRELOAD=lib/libmalloc-wf.so tests/benchmark > $DIR/wf_$i.txt
# done

# echo ""
# echo "******************************"
# echo "** Running benchmark for System Malloc**"
# echo "******************************" 

# DIR="benchmarks/sys"
# mkdir $DIR

# # run benchmark 5 times
# for i in $(seq 1 5)
# do  
#     echo ""
#     echo "Benchamrk $i"
#     ./tests/benchmark > $DIR/sysmalloc_$i.txt
# done