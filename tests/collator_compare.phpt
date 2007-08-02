--TEST--
compare()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Compare various string pairs in various locales
 * with Procedural and Object methods.
 */


// Converts comparison result to a character.
function get_printable_sign( $comp_res )
{
    switch( $comp_res )
    {
    case 0:            // UCOL_EQUAL
        return '=';
    case 1:            // UCOL_GREATER
        return '>';
    case -1:           // UCOL_LESS
        return '<';
    default:
        return '?';
    }
}

/*
 * Compare strings in specified locales
 */
function compare_in_locale( $locale_name, $test_array )
{
    $res_str = '';

    $coll = ut_coll_create( $locale_name );

    foreach( $test_array as $test_strings )
    {
        list( $str1, $str2 ) = $test_strings;

        // Convert both strings to UTF-16 if needed.
        $str1 = u( $str1 );
        $str2 = u( $str2 );

        // Compare strings.
        $res_val = get_printable_sign( ut_coll_compare( $coll, $str1, $str2 ) );

        // Concatenate result strings.
        $res_str .= dump( $str1 ) .
                    ' ' . $res_val . ' ' .
                    dump( $str2 ) . "\n";
    }

    return $res_str;

}

function ut_main()
{
    $locale_name = 'en_US';

    $res_str = '';

    // Compare strings using en_US locale.
    $test_params = array(
        array( 'abc', 'abc' ),
        array( 'Abc', 'abc' ),
        array( 'a'  , 'abc' ),
        array( 'a'  , ''    ),
        array( ''  , ''     ),
        array( 'a'  , 'b'   ),
        array( 'ab'  , 'b'  ),
        array( 'ab'  , 'a'  ),
        array( 123  , 'abc' ),
        array( 'ac' , null  ),
        array( '.'  , '.'   ),
        // Try to compare long strings.
        array( 'abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcde',
               'abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdea'),
        array( null , null  )
    );

    $res_str .= compare_in_locale( 'en_US', $test_params );


    // Compare strings using ru_RU locale.
    $test_params = array(
        array( 'а',   'б' ),
        array( 'а',   'аа' ),
        array( 'аб', 'ба' ),
        array( 'а',   ',' ),
        array( 'а',   'b' ),
        array( 'а',   'bb' ),
        array( 'а',   'ab' ),
        array( 'а',   null )
    );

    $res_str .= compare_in_locale( 'ru_RU', $test_params );


    // Compare strings using lt_LT locale.
    $test_params = array(
        array( 'y', 'k' )
    );

    $res_str .= compare_in_locale( 'lt_LT', $test_params );

    return $res_str;
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
'abc' = 'abc'
'Abc' > 'abc'
'a' < 'abc'
'a' > ''
'' = ''
'a' < 'b'
'ab' < 'b'
'ab' > 'a'
123 < 'abc'
'ac' > NULL
'.' = '.'
'abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcde' < 'abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdea'
NULL = NULL
'а' < 'б'
'а' < 'аа'
'аб' < 'ба'
'а' > ','
'а' > 'b'
'а' > 'bb'
'а' > 'ab'
'а' > NULL
'y' < 'k'
