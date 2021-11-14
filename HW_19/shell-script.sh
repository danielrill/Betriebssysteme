#!/usr/bin/env zsh

pagenum=1
trials=1000

while getopts "h?:" opt; do
    case "$opt" in
        h|\?)
            eval "./tlb -h"
            exit 0
            ;;
    esac
done

compile_command="make clean all"


echo $compile_command "\n"
eval $compile_command


for i in 1 2 3 4 5 6 7 #8 9 10 11 12 13 14 15 16
do

    pagenum=$((pagenum *2))
    echo "\n  >>Shell parameter<< \n"
    echo $pagenum " anzahl Seiten "
    echo $trials " anzahl Versuche \n"
    
    echo " >>Programmausführung<<\n"
    eval "./tlb -n$pagenum -l$trials" 
    
done

