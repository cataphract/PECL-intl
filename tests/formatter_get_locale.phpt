--TEST--
numfmt_get_locale()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Get locale.
 */

function ut_main()
{
    $locales = array(
        'en_UK',
        'en_US@California',
        'uk',
    );

    $loc_types = array(
        ULOC_ACTUAL_LOCALE    => 'actual',
        ULOC_VALID_LOCALE     => 'valid',
    );

    $res_str = '';

    foreach( $locales as $locale )
    {
        $fmt = ut_nfmt_create( $locale, NumberFormatter::DECIMAL );
        $res_str .= "$locale: ";
        foreach( $loc_types as $loc_type => $loc_type_name )
            $res_str .= sprintf( " %s=%s",
            $loc_type_name,
            dump( ut_nfmt_get_locale( $fmt, $loc_type ) ) );
        $res_str .= "\n";
    }

    return $res_str;
}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
en_UK:  actual='en' valid='en'
en_US@California:  actual='en' valid='en'
uk:  actual='root' valid='uk'
