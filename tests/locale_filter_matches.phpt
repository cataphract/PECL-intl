--TEST--
locale_filter_matches.phpt()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try parsing different Locales  
 * with Procedural and Object methods.
 */

function ut_main()
{
	$loc_ranges = array(
		'de-de',
		'sl_IT',
		'sl_IT_Nedis',
		'jbo',
		'art-lojban',
		'sl_IT'
	);
	
	$lang_tags = array(
		'de-DEVA',
		'de-DE-1996',
		'de-DE',
		'zh_Hans',
		'de-CH-1996',
		'sl_IT',
		'sl_IT_nedis-a-kirti-x-xyz',
		'sl_IT_rozaj',
		'sl_IT_NEDIS_ROJAZ_1901',
		'i-enochian',
		'sgn-CH-de',
		'art-lojban',
		'i-lux',
		'jbo',
		'en_sl_IT'
	);

    $res_str = '';
	$isCanonical =false;

	foreach($loc_ranges as $loc_range){
		$res_str .="--------------\n";
		$res_str .= "loc_range:$loc_range matches lang_tag ? \n";
		foreach($lang_tags as $lang_tag){
			$result= ut_loc_locale_filter_matches( $lang_tag , $loc_range ,$isCanonical);
			$res_str .= "$lang_tag ? ";
			if( $result){	
				$res_str .= "YES\t";
			}else{
				$res_str .= "NO\t";
			}
//canonicalized version
			$result= ut_loc_locale_filter_matches( $lang_tag , $loc_range ,!($isCanonical) );
/*
			$can_loc_range = ut_loc_canonicalize($loc_range);
			$can_lang_tag = ut_loc_canonicalize($lang_tag);
*/
			$res_str .= "canonically ? ";
			if( $result){	
				$res_str .= "YES\n";
			}else{
				$res_str .= "NO\n";
			}
		}
	}

	$res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
--------------
loc_range:de-de matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? YES	canonically ? YES
de-DE ? YES	canonically ? YES
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? NO	canonically ? NO
sl_IT_nedis-a-kirti-x-xyz ? NO	canonically ? NO
sl_IT_rozaj ? NO	canonically ? NO
sl_IT_NEDIS_ROJAZ_1901 ? NO	canonically ? NO
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? NO	canonically ? NO
i-lux ? NO	canonically ? NO
jbo ? NO	canonically ? NO
en_sl_IT ? NO	canonically ? NO
--------------
loc_range:sl_IT matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? NO	canonically ? NO
de-DE ? NO	canonically ? NO
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? YES	canonically ? YES
sl_IT_nedis-a-kirti-x-xyz ? YES	canonically ? YES
sl_IT_rozaj ? YES	canonically ? YES
sl_IT_NEDIS_ROJAZ_1901 ? YES	canonically ? YES
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? NO	canonically ? NO
i-lux ? NO	canonically ? NO
jbo ? NO	canonically ? NO
en_sl_IT ? NO	canonically ? NO
--------------
loc_range:sl_IT_Nedis matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? NO	canonically ? NO
de-DE ? NO	canonically ? NO
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? NO	canonically ? NO
sl_IT_nedis-a-kirti-x-xyz ? YES	canonically ? YES
sl_IT_rozaj ? NO	canonically ? NO
sl_IT_NEDIS_ROJAZ_1901 ? YES	canonically ? YES
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? NO	canonically ? NO
i-lux ? NO	canonically ? NO
jbo ? NO	canonically ? NO
en_sl_IT ? NO	canonically ? NO
--------------
loc_range:jbo matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? NO	canonically ? NO
de-DE ? NO	canonically ? NO
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? NO	canonically ? NO
sl_IT_nedis-a-kirti-x-xyz ? NO	canonically ? NO
sl_IT_rozaj ? NO	canonically ? NO
sl_IT_NEDIS_ROJAZ_1901 ? NO	canonically ? NO
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? NO	canonically ? YES
i-lux ? NO	canonically ? NO
jbo ? YES	canonically ? YES
en_sl_IT ? NO	canonically ? NO
--------------
loc_range:art-lojban matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? NO	canonically ? NO
de-DE ? NO	canonically ? NO
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? NO	canonically ? NO
sl_IT_nedis-a-kirti-x-xyz ? NO	canonically ? NO
sl_IT_rozaj ? NO	canonically ? NO
sl_IT_NEDIS_ROJAZ_1901 ? NO	canonically ? NO
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? YES	canonically ? YES
i-lux ? NO	canonically ? NO
jbo ? NO	canonically ? YES
en_sl_IT ? NO	canonically ? NO
--------------
loc_range:sl_IT matches lang_tag ? 
de-DEVA ? NO	canonically ? NO
de-DE-1996 ? NO	canonically ? NO
de-DE ? NO	canonically ? NO
zh_Hans ? NO	canonically ? NO
de-CH-1996 ? NO	canonically ? NO
sl_IT ? YES	canonically ? YES
sl_IT_nedis-a-kirti-x-xyz ? YES	canonically ? YES
sl_IT_rozaj ? YES	canonically ? YES
sl_IT_NEDIS_ROJAZ_1901 ? YES	canonically ? YES
i-enochian ? NO	canonically ? NO
sgn-CH-de ? NO	canonically ? NO
art-lojban ? NO	canonically ? NO
i-lux ? NO	canonically ? NO
jbo ? NO	canonically ? NO
en_sl_IT ? NO	canonically ? NO