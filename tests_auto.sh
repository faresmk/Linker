#!/bin/bash
# pour chaque fichier du dossier fichiers_test
for file in fichiers_test/*
do     
    # on teste l'étape 1
    echo $file >> test_readelf_etape1.txt
    readelf -h $file >> test_readelf_etape1.txt
    echo $file >> test_etape1.txt
    ./etape1 $file >> test_etape1.txt
    diff test_readelf_etape1.txt test_etape1.txt > test_diff_etape1.txt

    # on teste l'étape 2 : Affichage de la table des sections
    echo $file >> test_readelf_etape2.txt
    readelf -S $file >> test_readelf_etape2.txt
    echo $file >> test_etape2.txt
    ./etape2 $file >> test_etape2.txt
    diff test_readelf_etape2.txt test_etape2.txt > test_diff_etape2.txt

    # on teste l'étape 3 : Affichage du contenu d’une section
    # readelf -x .text $file > test_readelf_etape3.txt
    # ./etape3 $file > test_etape3.txt
    # diff test_readelf_etape3.txt test_etape3.txt > test_diff_etape3.txt

    # on teste l'étape 4 : Affichage de la table des symboles
    echo $file >> test_readelf_etape4.txt
    readelf -s $file >> test_readelf_etape4.txt
    echo $file >> test_etape4.txt
    ./etape4 $file >> test_etape4.txt
    diff test_readelf_etape4.txt test_etape4.txt > test_diff_etape4.txt

    # on teste l'étape 5 : Affichage des tables de réimplantation
    echo $file >> test_readelf_etape5.txt
    readelf -r $file >> test_readelf_etape5.txt
    echo $file >> test_etape5.txt
    ./etape5 $file >> test_etape5.txt
    diff test_readelf_etape5.txt test_etape5.txt > test_diff_etape5.txt
done

# si le fichier test_diff_etapeX.txt n'est pas vide, on affiche le nom du fichier testé correspondant au nom de l'étape
for file in test_diff_etape*.txt
do
    # récupère le numéro de l'étape
    # supprime test_diff_etape de la chaine
    num_etape=${file#test_diff_etape}
    # supprime .txt de la chaine
    num_etape=${num_etape%.txt}
    echo numero_etape : $num_etape
    if [ -s $file ]
    then
        echo "Erreur à l'étape $num_etape"
        echo ">>>>>>>>>>>>>>>> Résultat attendu :"
        cat test_readelf_etape$num_etape.txt
        echo ">>>>>>>>>>>>>>>> Résultat obtenu :"
        cat test_etape$num_etape.txt
    fi
done