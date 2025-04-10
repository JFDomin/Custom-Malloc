#!/bin/bash

output=$(make 2>&1)

if [[ "output" == "make: Nothing to be done for 'all'." ]]; then
    echo "$output"
fi

#loop until user quits 
while true; do
    echo ""
    echo "Select heap allocation strategy (press q or Esc to exit)"
    echo "1. First Fit"
    echo "2. Next Fit"
    echo "3. Best Fit"
    echo "4. Worst Fit"
    echo "5. System Malloc"
    echo ""
    echo -n "Enter choice [1-5]: "

    read -rsn1 choice
    echo ""

    if [[ $choice == "q" || $choice == "Q" || $choice == $'\e' ]]; then
        echo -e "\nExiting..."
        exit 1
    fi

    # Determine lib to use for heap allocation
    case "$choice" in 
        1)
            LIB="lib/libmalloc-ff.so"
            TEST="tests/ffnf"
            STRATEGY="First Fit"
            ;;
        2)
            LIB="lib/libmalloc-nf.so"
            TEST="tests/ffnf"
            STRATEGY="Next Fit"
            ;;
        3)
            LIB="lib/libmalloc-bf.so"
            TEST="tests/bfwf"
            STRATEGY="Best Fit"
            ;;
        4)
            LIB="lib/libmalloc-wf.so"
            TEST="tests/bfwf"
            STRATEGY="Worst Fit"
            ;;
        5)
            echo ""
            echo "Running with system malloc"
            TEST=""
            ./tests/bfwf
            ;;
        *)
            echo "Invalid choice. Please enter a number between 1 and 5"
            continue
            ;;
    esac
    
    if [ -f "$TEST" ]; then
        echo ""
        echo "Running test with $STRATEGY"
        LD_PRELOAD=$LIB ./$TEST
        echo ""
    fi 

done




