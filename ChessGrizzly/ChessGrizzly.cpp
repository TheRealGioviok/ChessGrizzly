// ChessGrizzly.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "BBmacros.h"
#include "tables.h"
#include <intrin.h>
int main()
{
    std::cout << "Chess Grizzly by Gioviok\n";
    initAll();

    // Create an occupancy to test move generation 
    forEachSquare(square) {
        BitBoard occupancy = 0ULL;
        setBit(occupancy, square);
        printBitBoard(occupancy);
    }

    BitBoard block = 0ULL;
    block |= squareBitBoard(e4) | squareBitBoard(e5) | squareBitBoard(d4) | squareBitBoard(d5);

    forEachSquare(square) {
        std::cout << squareNames[square] << "\n";
        //printBitBoard(squareBitBoard(square));
        printBitBoard(getQueenAttack(square, block));
    }

    printBitBoard(block);
    
}

// Per eseguire il programma: CTRL+F5 oppure Debug > Avvia senza eseguire debug
// Per eseguire il debug del programma: F5 oppure Debug > Avvia debug

// Suggerimenti per iniziare: 
//   1. Usare la finestra Esplora soluzioni per aggiungere/gestire i file
//   2. Usare la finestra Team Explorer per connettersi al controllo del codice sorgente
//   3. Usare la finestra di output per visualizzare l'output di compilazione e altri messaggi
//   4. Usare la finestra Elenco errori per visualizzare gli errori
//   5. Passare a Progetto > Aggiungi nuovo elemento per creare nuovi file di codice oppure a Progetto > Aggiungi elemento esistente per aggiungere file di codice esistenti al progetto
//   6. Per aprire di nuovo questo progetto in futuro, passare a File > Apri > Progetto e selezionare il file con estensione sln
