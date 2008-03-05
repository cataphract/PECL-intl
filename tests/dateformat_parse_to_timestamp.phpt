--TEST--
datefmt_parse_to_timestamp_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_parse_to_timestamp  function
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
		"December 18, 1969 8:49:59 AM PST",
		"12/18/69 8:49 AM",
		"19691218 08:49 AM"
	);

	foreach( $text_arr as $text_entry){
                $res_str .= "\n------------\n";
                $res_str .= "\nInput text is : $text_entry";
                $res_str .= "\n------------";

                foreach( $locale_arr as $locale_entry ){
			$res_str .= "\nLocale is : $locale_entry";
			$res_str .= "\n------------";
                        foreach( $datetype_arr as $datetype_entry )
			{
				$res_str .= "\ndatetype = $datetype_entry ,timetype =$datetype_entry ";
				$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry);
				$parsed = ut_datefmt_parse_to_timestamp( $fmt , $text_entry);
				if( intl_get_error_code() == U_ZERO_ERROR){
					$res_str .= "\nParsed text is : $parsed";
				}else{
					$res_str .= "\nError while parsing as: '".intl_get_error_message()."'";
				}
			}
		}
        }


	return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
------------

Input text is : Thursday, December 18, 1969 8:49:59 AM PST
------------
Locale is : en_US_CA
------------
datetype = 0 ,timetype =0 
Parsed text is : -1149001000
datetype = 1 ,timetype =1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 2 ,timetype =2 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 3 ,timetype =3 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = -1 ,timetype =-1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
------------

Input text is : December 18, 1969 8:49:59 AM PST
------------
Locale is : en_US_CA
------------
datetype = 0 ,timetype =0 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 1 ,timetype =1 
Parsed text is : -1149001000
datetype = 2 ,timetype =2 
Parsed text is : -1149001000
datetype = 3 ,timetype =3 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = -1 ,timetype =-1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
------------

Input text is : 12/18/69 8:49 AM
------------
Locale is : en_US_CA
------------
datetype = 0 ,timetype =0 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 1 ,timetype =1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 2 ,timetype =2 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 3 ,timetype =3 
Parsed text is : -1149060000
datetype = -1 ,timetype =-1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
------------

Input text is : 19691218 08:49 AM
------------
Locale is : en_US_CA
------------
datetype = 0 ,timetype =0 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 1 ,timetype =1 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 2 ,timetype =2 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = 3 ,timetype =3 
Error while parsing as: 'Date parsing failed: U_PARSE_ERROR'
datetype = -1 ,timetype =-1 
Parsed text is : -1149060000