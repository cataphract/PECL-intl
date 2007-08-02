--TEST--
get_locale()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try to specify valid and invalid locale types when getting locale
 * with Procedural and Object methods.
 */

function ut_main()
{
    $res_str = '';
    $coll = ut_coll_create( 'en_US' );

    // Array with locales for test.
    $test_params = array(
        ULOC_REQUESTED_LOCALE,
        ULOC_VALID_LOCALE,
        ULOC_ACTUAL_LOCALE,
        100,
        -100,
        -9999999999999,
        9999999999999,
        1.2,
        9999999999999999999999999999999999999999999999
    );

    foreach( $test_params as $test_param )
    {
        $res_val = ut_coll_get_locale( $coll, $test_param );

        $res_str .= sprintf(
            "Locale of type %s is %s\n",
            dump( $test_param ),
            dump( $res_val ) );
    }

    return $res_str . "\n";
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
Locale of type 2 is 'en_US'
Locale of type 1 is 'en_US'
Locale of type 0 is 'en'
Locale of type 100 is false
Locale of type -100 is false
Locale of type -9999999999999 is false
Locale of type 9999999999999 is false
Locale of type 1.2 is 'en_US'
Locale of type 1.0E+46 is 'en'
