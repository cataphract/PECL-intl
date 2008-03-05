--TEST--
datefmt_set_lenient and datefmt_set_lenient()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--

<?php

/*
 * Test for the datefmt_get_lenient & datefmt_set_lenient function
 */


function ut_main()
{

        $res_str = '';

	//Create
        $fmt = ut_datefmt_create( "en-US",  DateFormatter::SHORT, DateFormatter::SHORT , 'America/New_York', DateFormatter::GREGORIAN );
        $res_str .= "\nDateFormatter Created.\n";

        $resLenient1 = ut_datefmt_is_lenient( $fmt);
        $res_str .= "After call to get_lenient :  lenient= ";
	if( $resLenient1){
		$res_str .= "TRUE\n";
	}else{
		$res_str .= "FALSE\n";
	}

	//Set and test
        $res_str .= "--------------------\n";
        $isLenient = TRUE;
	$res_str .= "Setting DateFormatter with lenient = ";
	if( $isLenient){
		$res_str .= "TRUE\n";
	}else{
		$res_str .= "FALSE\n";
	}
	ut_datefmt_set_lenient( $fmt , $isLenient );
	$resLenient = ut_datefmt_is_lenient( $fmt);
	$res_str .= "After call to is_lenient :  lenient= ";
	if( $resLenient){
		$res_str .= "TRUE\n";
	}else{
		$res_str .= "FALSE\n";
	}


	//Set and test
        $res_str .= "--------------------\n";
        $isLenient = FALSE;
	$res_str .= "Setting DateFormatter with lenient =";
	if( $isLenient){
		$res_str .= "TRUE\n";
	}else{
		$res_str .= "FALSE\n";
	}
	ut_datefmt_set_lenient( $fmt , $isLenient);
	$resLenient = ut_datefmt_is_lenient( $fmt);
	$res_str .= "After call to is_lenient :  lenient= ";
	if( $resLenient){
		$res_str .= "TRUE\n";
	}else{
		$res_str .= "FALSE\n";
	}

        $res_str .= "--------------------\n";

        return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
DateFormatter Created.
After call to get_lenient :  lenient= TRUE
--------------------
Setting DateFormatter with lenient = TRUE
After call to is_lenient :  lenient= TRUE
--------------------
Setting DateFormatter with lenient =FALSE
After call to is_lenient :  lenient= FALSE
--------------------
