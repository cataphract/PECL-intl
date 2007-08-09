--TEST--
sort()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Sort arrays using various locales.
 */


/*
 * Sort arrays in the given list using specified locale.
 */
function sort_arrays( $locale, $arrays, $sort_flag = Collator::SORT_REGULAR )
{
    $res_str = '';

    $coll = ut_coll_create( $locale );

    foreach( $arrays as $array )
    {
        // Convert strings to UTF-16 if needed.
        $u_array = u( $array );

        // Sort array values
        $res_val = ut_coll_sort( $coll, $u_array, $sort_flag );

        // Concatenate the sorted array and function result
        // with output string.
        $res_str .= "\n" .
                    dump( $u_array ) .
                    "\n Result: " .
                    dump( $res_val );
    }

    return $res_str;
}

function ut_main()
{
    $res_str = '';

    // Sort an array in SORT_REGULAR mode using en_US locale.
    $test_params = array(
        array( 'abc', 'abd', 'aaa' ),
        array( 'm'  , '1'  , '_'   ),
        array( 'a'  , 'aaa', 'aa'  ),
        array( 'ba' , 'b'  , 'ab'  ),
        array( 'e'  , 'c'  , 'a'   ),
        array( '100', '25' , '36'  ),
        array( 5    , '30' , 2     ),
        array( 'd'  , ''   , ' a'  ),
        array( 'd ' , 'f ' , ' a'  ),
        array( 'a'  , null , '3'   ),
        array( 'y'  , 'k'  , 'i' )
    );

    $res_str .= sort_arrays( 'en_US', $test_params );

    $test_params = array(
        array( '100', '25' , '36'  ),
        array( 5    , '30' , 2     ),
        array( 'd'  , ''   , ' a'  ),
        array( 'y'  , 'k'  , 'i' )
    );

    // Sort in en_US locale with SORT_STRING flag
    $res_str .= sort_arrays( 'en_US', $test_params, Collator::SORT_STRING );


    // Sort a non-ASCII array using ru_RU locale.
    $test_params = array(
        array( 'абг', 'абв', 'ааа', 'abc' ),
        array( 'аа', 'ааа' , 'а' )
    );

    $res_str .= sort_arrays( 'ru_RU', $test_params );

    // Sort an array using Lithuanian locale.
    $test_params = array(
        array( 'y'  , 'k'  , 'i' )
    );

    $res_str .= sort_arrays( 'lt_LT', $test_params );

    return $res_str;
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
array (
  0 => 'aaa',
  1 => 'abc',
  2 => 'abd',
)
 Result: true
array (
  0 => '_',
  1 => '1',
  2 => 'm',
)
 Result: true
array (
  0 => 'a',
  1 => 'aa',
  2 => 'aaa',
)
 Result: true
array (
  0 => 'ab',
  1 => 'b',
  2 => 'ba',
)
 Result: true
array (
  0 => 'a',
  1 => 'c',
  2 => 'e',
)
 Result: true
array (
  0 => '25',
  1 => '36',
  2 => '100',
)
 Result: true
array (
  0 => 2,
  1 => 5,
  2 => '30',
)
 Result: true
array (
  0 => '',
  1 => ' a',
  2 => 'd',
)
 Result: true
array (
  0 => ' a',
  1 => 'd ',
  2 => 'f ',
)
 Result: true
array (
  0 => NULL,
  1 => '3',
  2 => 'a',
)
 Result: true
array (
  0 => 'i',
  1 => 'k',
  2 => 'y',
)
 Result: true
array (
  0 => '100',
  1 => '25',
  2 => '36',
)
 Result: true
array (
  0 => 2,
  1 => '30',
  2 => 5,
)
 Result: true
array (
  0 => '',
  1 => ' a',
  2 => 'd',
)
 Result: true
array (
  0 => 'i',
  1 => 'k',
  2 => 'y',
)
 Result: true
array (
  0 => 'abc',
  1 => 'ааа',
  2 => 'абв',
  3 => 'абг',
)
 Result: true
array (
  0 => 'а',
  1 => 'аа',
  2 => 'ааа',
)
 Result: true
array (
  0 => 'i',
  1 => 'y',
  2 => 'k',
)
 Result: true