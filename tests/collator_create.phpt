--TEST--
create()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try creating collator with different locales
 * with Procedural and Object methods.
 */

function ut_main()
{
    $res_str = '';

    $test_params = array(
        'EN-US-ODESSA',
        'UK_UA_ODESSA',
        'uk-ua_CALIFORNIA@currency=;currency=GRN',
        // locale for default ICU values.
        '',
        'root',
        'uk@currency=EURO'
    );

    foreach( $test_params as $test_param )
    {
        // Create Collator with the current locale.
        $coll = ut_coll_create( $test_param );

        // Get the requested, valid and actual locales.
        $rloc = ut_coll_get_locale( $coll, ULOC_REQUESTED_LOCALE );
        $vloc = ut_coll_get_locale( $coll, ULOC_VALID_LOCALE );
        $aloc = ut_coll_get_locale( $coll, ULOC_ACTUAL_LOCALE );

        // Show them.
        $res_str .= "Locale: '$test_param'\n" .
            "  ULOC_REQUESTED_LOCALE = '$rloc'\n" .
            "  ULOC_VALID_LOCALE     = '$vloc'\n" .
            "  ULOC_ACTUAL_LOCALE    = '$aloc'\n";
    }

    // Test creation of Collator with empty locale name.
    $coll = ut_coll_create( '' );
    if( !isset( $coll ) )
        print( "Error - collator was not created with default parameter!\n" );

    return $res_str;
}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
Locale: 'EN-US-ODESSA'
  ULOC_REQUESTED_LOCALE = 'EN-US-ODESSA'
  ULOC_VALID_LOCALE     = 'en_US'
  ULOC_ACTUAL_LOCALE    = 'en'
Locale: 'UK_UA_ODESSA'
  ULOC_REQUESTED_LOCALE = 'UK_UA_ODESSA'
  ULOC_VALID_LOCALE     = 'uk_UA'
  ULOC_ACTUAL_LOCALE    = 'uk'
Locale: 'uk-ua_CALIFORNIA@currency=;currency=GRN'
  ULOC_REQUESTED_LOCALE = 'uk-ua_CALIFORNIA@currency=;currency=GRN'
  ULOC_VALID_LOCALE     = 'uk_UA'
  ULOC_ACTUAL_LOCALE    = 'uk'
Locale: ''
  ULOC_REQUESTED_LOCALE = ''
  ULOC_VALID_LOCALE     = 'root'
  ULOC_ACTUAL_LOCALE    = 'root'
Locale: 'root'
  ULOC_REQUESTED_LOCALE = 'root'
  ULOC_VALID_LOCALE     = 'root'
  ULOC_ACTUAL_LOCALE    = 'root'
Locale: 'uk@currency=EURO'
  ULOC_REQUESTED_LOCALE = 'uk@currency=EURO'
  ULOC_VALID_LOCALE     = 'uk'
  ULOC_ACTUAL_LOCALE    = 'uk'
