--TEST--
datefmt_get_pattern_code and datefmt_set_pattern_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--

<?php

/*
 * Test for the datefmt_get_pattern & datefmt_set_pattern function
 */


function ut_main()
{
        $pattern_arr = array (
                'DD-MM-YY',
                "yyyy/MM/dd",
                "yyyyMMdd"
        );

        $res_str = '';

        $start_pattern = 'DD-MM-YY';
        $res_str .= "\nCreating DateFormatter with pattern = $start_pattern ";
        $fmt = ut_datefmt_create( "en-US",  DateFormatter::SHORT, DateFormatter::SHORT , 'America/New_York', DateFormatter::GREGORIAN , $start_pattern );
        $pattern = ut_datefmt_get_pattern( $fmt);
        $res_str .= "\nAfter call to get_pattern :  pattern= $pattern";


        foreach( $pattern_arr as $pattern_entry )
        {
                $res_str .= "\nSetting DateFormatter with pattern = $pattern_entry ";
                ut_datefmt_set_pattern( $fmt , $pattern_entry );
                $pattern = ut_datefmt_get_pattern( $fmt);
                $res_str .= "\nAfter call to get_pattern :  pattern= $pattern";
                $res_str .= "\n";

        }

        return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
Creating DateFormatter with pattern = DD-MM-YY 
After call to get_pattern :  pattern= M/d/yy h:mm a
Setting DateFormatter with pattern = DD-MM-YY 
After call to get_pattern :  pattern= DD-MM-YY

Setting DateFormatter with pattern = yyyy/MM/dd 
After call to get_pattern :  pattern= yyyy/MM/dd

Setting DateFormatter with pattern = yyyyMMdd 
After call to get_pattern :  pattern= yyyyMMdd