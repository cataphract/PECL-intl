--TEST--
get_available_locales()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Get list of available locales.
 */


function ut_main()
{
    $nlocales = count( ut_coll_get_available_locales() );
    return ( $nlocales !== false ? "yes" : "no" ) . "\n";
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
yes
