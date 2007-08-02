--TEST--
get/set_attribute()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php
/*
 * Try to set and get the values of a NORMALIZATION_MODE attribute
 * with Procedural and Object methods.
 */

/*
 * Return string with status of NormalizationMode attribute.
 */
function check_val( $coll )
{
    $val = ut_coll_get_attribute( $coll, Collator::NORMALIZATION_MODE );
    return sprintf( "%s\n", ( $val == Collator::OFF ? "off" : "on" ) );
}

function ut_main()
{
    $res = '';
    $coll = ut_coll_create( 'en_US' );

    ut_coll_set_attribute( $coll, Collator::NORMALIZATION_MODE, Collator::OFF );
    $res .= check_val( $coll );

    ut_coll_set_attribute( $coll, Collator::NORMALIZATION_MODE, Collator::ON );
    $res .= check_val( $coll );

    return $res;
}

include( 'ut_common.php' );
ut_run();
?>
--EXPECT--
off
on
