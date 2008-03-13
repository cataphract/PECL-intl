--TEST--
datefmt_localtime_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_localtime  function
 */


function ut_main()
{
	$locale_arr = array (
		'en_US_CA'
	);
	
	$datetype_arr = array (
                DateFormatter::FULL,
                DateFormatter::LONG,
                DateFormatter::MEDIUM,
                DateFormatter::SHORT,
                DateFormatter::NONE
        );

        $res_str = '';


	$text_arr = array (
		"Thursday, December 18, 1969 8:49:59 AM PST",
		"Thursday, December 18, 1969 8:49:59 PM PST",
		"June 18, 1969 8:49:59 AM PST",
		"12/18/69 8:49:59 AM",
		"19691218 08:49:59 AM"
	);

	$fmt1 = ut_datefmt_create( 'en_US_CA', DateFormatter::SHORT, DateFormatter::SHORT);
	$fmt2 = ut_datefmt_create( 'en_US_CA', DateFormatter::NONE, DateFormatter::NONE);
	$fmt3 = ut_datefmt_create( 'en_US_CA', DateFormatter::LONG, DateFormatter::LONG);
	$fmt4 = ut_datefmt_create( 'en_US_CA', DateFormatter::MEDIUM, DateFormatter::MEDIUM);
	$fmt5 = ut_datefmt_create( 'en_US_CA', DateFormatter::FULL, DateFormatter::FULL);
	$fmt_array  = array(
		$fmt1 , $fmt2 ,$fmt3 , $fmt4 ,$fmt5
	);
	$fmt_desc_array  = array(
		"DateType::SHORT, TimeType::SHORT",
		"DateType::NONE, TimeType::NONE",
		"DateType::LONG, TimeType::LONG",
		"DateType::MEDIUM, TimeType::MEDIUM",
		"DateType::FULL, TimeType::FULL"
	);

	foreach( $text_arr as $text_entry){
                $res_str .= "\n-------------------------------\n";
                $res_str .= "\nInput text is : $text_entry";
		$cnt =0;

		$parse_pos = 0;
                //for ( $parse_pos = 0 ; $parse_pos < strlen($text_entry) ; $parse_pos++ ){

                    foreach( $fmt_array as $fmt_entry ){
			$res_str .= "\n------------";
			$res_str .= "\nDateFormatter : ".$fmt_desc_array[$cnt];
			$cnt++;		
			$parsed_arr = ut_datefmt_localtime( $fmt_entry , $text_entry , $parse_pos );

				if( $parsed_arr){
				    $res_str .= "\n";
				    foreach( $parsed_arr as $key => $value){
					    $res_str .= "$key : '$value' , ";
				    }
				}
/*
				else{
				    //$res_str .= "No values found from LocaleTime parsing.";
				    $res_str .= "\tError : '".intl_get_error_message()."'";
				}
*/
		    }//end of for $fmt_array
		//} //end of for $parse_pos
        }


	return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
-------------------------------

Input text is : Thursday, December 18, 1969 8:49:59 AM PST
------------
DateFormatter : DateType::SHORT, TimeType::SHORT
------------
DateFormatter : DateType::NONE, TimeType::NONE
------------
DateFormatter : DateType::LONG, TimeType::LONG
------------
DateFormatter : DateType::MEDIUM, TimeType::MEDIUM
------------
DateFormatter : DateType::FULL, TimeType::FULL
tm_sec : '59' , tm_min : '49' , tm_hour : '8' , tm_year : '1969' , tm_mday : '18' , tm_wday : '5' , tm_yday : '352' , tm_mon : '11' , tm_isdst : '0' , 
-------------------------------

Input text is : Thursday, December 18, 1969 8:49:59 PM PST
------------
DateFormatter : DateType::SHORT, TimeType::SHORT
------------
DateFormatter : DateType::NONE, TimeType::NONE
------------
DateFormatter : DateType::LONG, TimeType::LONG
------------
DateFormatter : DateType::MEDIUM, TimeType::MEDIUM
------------
DateFormatter : DateType::FULL, TimeType::FULL
tm_sec : '59' , tm_min : '49' , tm_hour : '20' , tm_year : '1969' , tm_mday : '18' , tm_wday : '5' , tm_yday : '352' , tm_mon : '11' , tm_isdst : '0' , 
-------------------------------

Input text is : June 18, 1969 8:49:59 AM PST
------------
DateFormatter : DateType::SHORT, TimeType::SHORT
------------
DateFormatter : DateType::NONE, TimeType::NONE
------------
DateFormatter : DateType::LONG, TimeType::LONG
tm_sec : '59' , tm_min : '49' , tm_hour : '9' , tm_year : '1969' , tm_mday : '18' , tm_wday : '4' , tm_yday : '169' , tm_mon : '5' , tm_isdst : '1' , 
------------
DateFormatter : DateType::MEDIUM, TimeType::MEDIUM
tm_sec : '59' , tm_min : '49' , tm_hour : '8' , tm_year : '1969' , tm_mday : '18' , tm_wday : '4' , tm_yday : '169' , tm_mon : '5' , tm_isdst : '1' , 
------------
DateFormatter : DateType::FULL, TimeType::FULL
-------------------------------

Input text is : 12/18/69 8:49:59 AM
------------
DateFormatter : DateType::SHORT, TimeType::SHORT
------------
DateFormatter : DateType::NONE, TimeType::NONE
------------
DateFormatter : DateType::LONG, TimeType::LONG
------------
DateFormatter : DateType::MEDIUM, TimeType::MEDIUM
------------
DateFormatter : DateType::FULL, TimeType::FULL
-------------------------------

Input text is : 19691218 08:49:59 AM
------------
DateFormatter : DateType::SHORT, TimeType::SHORT
------------
DateFormatter : DateType::NONE, TimeType::NONE
------------
DateFormatter : DateType::LONG, TimeType::LONG
------------
DateFormatter : DateType::MEDIUM, TimeType::MEDIUM
------------
DateFormatter : DateType::FULL, TimeType::FULL
