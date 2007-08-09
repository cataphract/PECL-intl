--TEST--
asort()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Sort associative arrays using various locales.
 */


/*
 * Sort various arrays in specified locale.
 */
function sort_arrays( $locale, $test_arrays, $sort_flag = Collator::SORT_REGULAR )
{
    $res_str = '';

    $coll = ut_coll_create( $locale );

    foreach( $test_arrays as $test_array )
    {
        // Convert strings to UTF-16 if needed.
        $u_test_array = u( $test_array );

        // Try to sort test data.
        $res_val = ut_coll_asort( $coll, $u_test_array, $sort_flag );

        // Return output data.
        $res_str .= "\n" . dump( $u_test_array ) .
                    "\n Result: " . dump( $res_val );
    }

    return $res_str;
}

/*
 * Test main function.
 */
function ut_main()
{
    $res_str = '';

    // Sort an array in SORT_REGULAR mode using en_US locale.
    $test_params = array(
        array( 'd' => 'y'  ,
               'c' => 'i'  ,
               'a' => 'k'  ),

        array( 'a' => 'a'  ,
               'b' => 'aaa',
               'c' => 'aa' ),

        array( 'a'  => 'a' ,
               'aaa'=> 'a' ,
               'aa' => 'a' ),

        array( '1' => 'abc',
               '5' => '!'  ,
               '2' => null ,
               '7' => ''   ),

        array( '1' => '100',
               '2' => '25' ,
               '3' => '36' ),

        array( '1' => 5    ,
               '2' => '30' ,
               '3' => 2    )
    );

    $res_str .= sort_arrays( 'en_US', $test_params );

    // Sort an array in SORT_STRING mode using en_US locale.
    $test_params = array(
        array( '1' => '100',
               '2' => '25' ,
               '3' => '36' ),

        array( '1' => 5    ,
               '2' => '30' ,
               '3' => 2    ),

        array( '1' => 'd'  ,
               '2' => ''   ,
               '3' => ' a' ),

        array( '1' => 'y'  ,
               '2' => 'k'  ,
               '3' => 'i'  )
    );

    $res_str .= sort_arrays( 'en_US', $test_params, Collator::SORT_STRING );

    // Sort a non-ASCII array using ru_RU locale.
    $test_params = array(
        array( 'п' => 'у',
               'б' => 'в',
               'е' => 'а' ),

        array( '1' => 'п',
               '4' => '',
               '7' => 'd',
               '2' => 'пп' )
    );

    $res_str .= sort_arrays( 'ru_RU', $test_params );


    // Sort an array using Lithuanian locale.
    $test_params = array(
        array( 'd' => 'y',
               'c' => 'i',
               'a' => 'k' )
    );

    $res_str .= sort_arrays( 'lt_LT', $test_params );

    return $res_str . "\n";
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
array (
  'c' => 'i',
  'a' => 'k',
  'd' => 'y',
)
 Result: true
array (
  'a' => 'a',
  'c' => 'aa',
  'b' => 'aaa',
)
 Result: true
array (
  'aa' => 'a',
  'aaa' => 'a',
  'a' => 'a',
)
 Result: true
array (
  7 => '',
  2 => NULL,
  5 => '!',
  1 => 'abc',
)
 Result: true
array (
  2 => '25',
  3 => '36',
  1 => '100',
)
 Result: true
array (
  3 => 2,
  1 => 5,
  2 => '30',
)
 Result: true
array (
  1 => '100',
  2 => '25',
  3 => '36',
)
 Result: true
array (
  3 => 2,
  2 => '30',
  1 => 5,
)
 Result: true
array (
  2 => '',
  3 => ' a',
  1 => 'd',
)
 Result: true
array (
  3 => 'i',
  2 => 'k',
  1 => 'y',
)
 Result: true
array (
  'е' => 'а',
  'б' => 'в',
  'п' => 'у',
)
 Result: true
array (
  4 => '',
  7 => 'd',
  1 => 'п',
  2 => 'пп',
)
 Result: true
array (
  'c' => 'i',
  'd' => 'y',
  'a' => 'k',
)
 Result: true
