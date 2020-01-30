// test



// INCLUDES

#include "../cpu/defines.h"
// #include "util/asm_utils.h"
#include "util/io_utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>



// MAIN FUNCTION

int
main( void )
{
    println( "main: Entered." );

    char  char_arr1[ TEST_INPUT_BUFFER_SIZE ];
    char *char_arr2 = malloc( TEST_INPUT_BUFFER_SIZE );
    uint32_t word1;
    uint32_t word2;

    while ( true ) {
        println( "" );
        println( "main: Choose an action:" );
        println( "main:   ( 0) Exit with given status." );
        println( "main:   ( 1) Output random numbers." );
        println( "main:   ( 2) Calculate quotient and remainder." );
        println( "main:   ( 3) Greet politely." );
        println( "main:   (-1) Stress test (semi-infinite loop)." );
        println( "" );

        print( "main: action $ " );  scan_word( &word1 );

        switch ( word1 ) {
            case 0:
                print( "main: (0): status $ " );  scan_word( &word1 );

                return word1;
            case 1:
                print( "main: (1): seed $ " );  scan_word( &word1 );
                print( "main: (1): count $ " );  scan_word( &word2 );

                srand( word1 );
                for ( uint32_t i_ = 0;  i_ < word2;  ++i_ ) {
                    print( "main: (1): " );  print_word( rand() );
                }

                break;
            case 2: {
                print( "main: (2): word1 $ " );  scan_word( &word1 );
                print( "main: (2): word2 $ " );  scan_word( &word2 );

                uint32_t quot = word1 / word2;
                uint32_t rem  = word1 % word2;

                snprintf( char_arr1, TEST_INPUT_BUFFER_SIZE,
                        "word1 / word2 = %lu", quot );
                print( "main: (2): " );  println( char_arr1 );
                snprintf( char_arr1, TEST_INPUT_BUFFER_SIZE,
                        "word1 %% word2 = %lu", rem );
                print( "main: (2): " );  println( char_arr1 );

                uint32_t check = quot * word2 + rem - word1;

                snprintf( char_arr1, TEST_INPUT_BUFFER_SIZE,
                        "quot * word2 + rem - word1 = %lu", check );
                print( "main: (2): " );  println( char_arr1 );

                if ( check != 0 ) {
                    println( "main: (2): check != 0: failure." );

                    return EXIT_FAILURE;
                }

                break;
            }
            case 3:
                print( "main: (3): name $ " );  scan( char_arr2 );

                snprintf( char_arr1, TEST_INPUT_BUFFER_SIZE,
                        "Hello, '%s'.", char_arr2 );
                println( char_arr1 );

                break;
            case -1:
                srand( 1 );
                while ( rand() != 0 ) {}

                break;

            default:  break;
        }
    }
}
