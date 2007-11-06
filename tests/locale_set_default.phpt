--TEST--
locale_set_default($locale)
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try setting the default Locale with different locales
 * with Procedural and Object methods.
 * Since the set_default and get_default manipulate the extension global
 * default_locale , get_default is also included in this testing
 * to make sure set_default and  get_default are in synch
 */

function ut_main()
{
    $res_str = '';

    $locales = array(
        'sl_IT_nedis_KIRTI',
        'sl_IT_nedis-a-kirti-x-xyz',
        'sl_IT_rozaj',
        'sl_IT_NEDIS_ROJAZ_1901',
        'i-enochian',
        'zh-hakka',
        'zh-wuu',
        'i-tay',
        'sgn-BE-nl',
        'sgn-CH-de',
        'sl_IT_rozaj@currency=EUR',
        'uk-ua_CALIFORNIA@currency=;currency=GRN',
        '',
        'root',
        'uk@currency=EURO',
        'Hindi',
//Simple language subtag
        'de',
        'fr',
        'ja',
        'i-enochian', //(example of a grandfathered tag)
//Language subtag plus Script subtag:
        'zh-Hant',
        'zh-Hans',
        'sr-Cyrl',
        'sr-Latn',
//Language-Script-Region
        'zh-Hans-CN',
        'sr-Latn-CS',
//Language-Variant
        'sl-rozaj',
        'sl-nedis',
//Language-Region-Variant
        'de-CH-1901',
        'sl-IT-nedis',
//Language-Script-Region-Variant
        'sl-Latn-IT-nedis',
//Language-Region:
        'de-DE',
        'en-US',
        'es-419',
//Private use subtags:
        'de-CH-x-phonebk',
        'az-Arab-x-AZE-derbend',
//Extended language subtags
        'zh-min',
        'zh-min-nan-Hant-CN',
//Private use registry values
        'x-whatever',
        'qaa-Qaaa-QM-x-southern',
        'sr-Latn-QM',
        'sr-Qaaa-CS',
//Tags that use extensions (examples ONLY: extensions MUST be defined
//   by revision or update to this document or by RFC): 
        'en-US-u-islamCal',
        'zh-CN-a-myExt-x-private',
        'en-a-myExt-b-another',
//Some Invalid Tags:
        'de-419-DE',
        'a-DE',
        'ar-a-aaa-b-bbb-a-ccc'
    );

/*
	$locales = array(	
		'es',
        'en-US-u-islamCal',
        'zh-CN-a-myExt-x-private',
        'en-a-myExt-b-another',
//Some Invalid Tags:
        'de-419-DE',
        'a-DE'
	);
*/
    $res_str = '';

//Try to get the default from the php.ini
    $config_loc = ut_loc_get_default( );
    $res_str .= "Before calling set_default , get locale '$config_loc'";
	$res_str .= "\n";

    foreach( $locales as $locale )
    {
        $res_str .= "Locale : $locale \n";
        $lang = ut_loc_set_default( $locale);
        $res_str .= "set locale '$lang'";
        $lang = ut_loc_get_default( );
        $res_str .= "\t get locale '$lang'";
		$res_str .= "\n";
        $res_str .= "----------------- \n";
    }

//This is to make sure the procedural API does produce the same result for the first call	
    $lang = ut_loc_set_default( $config_loc);
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
Before calling set_default , get locale 'en_US_Latn'
Locale : sl_IT_nedis_KIRTI 
set locale 'sl_IT_nedis_KIRTI'	 get locale 'sl_IT_nedis_KIRTI'
----------------- 
Locale : sl_IT_nedis-a-kirti-x-xyz 
set locale 'sl_IT_nedis-a-kirti-x-xyz'	 get locale 'sl_IT_nedis-a-kirti-x-xyz'
----------------- 
Locale : sl_IT_rozaj 
set locale 'sl_IT_rozaj'	 get locale 'sl_IT_rozaj'
----------------- 
Locale : sl_IT_NEDIS_ROJAZ_1901 
set locale 'sl_IT_NEDIS_ROJAZ_1901'	 get locale 'sl_IT_NEDIS_ROJAZ_1901'
----------------- 
Locale : i-enochian 
set locale 'i-enochian'	 get locale 'i-enochian'
----------------- 
Locale : zh-hakka 
set locale 'zh-hakka'	 get locale 'zh-hakka'
----------------- 
Locale : zh-wuu 
set locale 'zh-wuu'	 get locale 'zh-wuu'
----------------- 
Locale : i-tay 
set locale 'i-tay'	 get locale 'i-tay'
----------------- 
Locale : sgn-BE-nl 
set locale 'sgn-BE-nl'	 get locale 'sgn-BE-nl'
----------------- 
Locale : sgn-CH-de 
set locale 'sgn-CH-de'	 get locale 'sgn-CH-de'
----------------- 
Locale : sl_IT_rozaj@currency=EUR 
set locale 'sl_IT_rozaj@currency=EUR'	 get locale 'sl_IT_rozaj@currency=EUR'
----------------- 
Locale : uk-ua_CALIFORNIA@currency=;currency=GRN 
set locale 'uk-ua_CALIFORNIA@currency=;currency=GRN'	 get locale 'uk-ua_CALIFORNIA@currency=;currency=GRN'
----------------- 
Locale :  
set locale ''	 get locale 'en_US_POSIX'
----------------- 
Locale : root 
set locale 'root'	 get locale 'root'
----------------- 
Locale : uk@currency=EURO 
set locale 'uk@currency=EURO'	 get locale 'uk@currency=EURO'
----------------- 
Locale : Hindi 
set locale 'Hindi'	 get locale 'Hindi'
----------------- 
Locale : de 
set locale 'de'	 get locale 'de'
----------------- 
Locale : fr 
set locale 'fr'	 get locale 'fr'
----------------- 
Locale : ja 
set locale 'ja'	 get locale 'ja'
----------------- 
Locale : i-enochian 
set locale 'i-enochian'	 get locale 'i-enochian'
----------------- 
Locale : zh-Hant 
set locale 'zh-Hant'	 get locale 'zh-Hant'
----------------- 
Locale : zh-Hans 
set locale 'zh-Hans'	 get locale 'zh-Hans'
----------------- 
Locale : sr-Cyrl 
set locale 'sr-Cyrl'	 get locale 'sr-Cyrl'
----------------- 
Locale : sr-Latn 
set locale 'sr-Latn'	 get locale 'sr-Latn'
----------------- 
Locale : zh-Hans-CN 
set locale 'zh-Hans-CN'	 get locale 'zh-Hans-CN'
----------------- 
Locale : sr-Latn-CS 
set locale 'sr-Latn-CS'	 get locale 'sr-Latn-CS'
----------------- 
Locale : sl-rozaj 
set locale 'sl-rozaj'	 get locale 'sl-rozaj'
----------------- 
Locale : sl-nedis 
set locale 'sl-nedis'	 get locale 'sl-nedis'
----------------- 
Locale : de-CH-1901 
set locale 'de-CH-1901'	 get locale 'de-CH-1901'
----------------- 
Locale : sl-IT-nedis 
set locale 'sl-IT-nedis'	 get locale 'sl-IT-nedis'
----------------- 
Locale : sl-Latn-IT-nedis 
set locale 'sl-Latn-IT-nedis'	 get locale 'sl-Latn-IT-nedis'
----------------- 
Locale : de-DE 
set locale 'de-DE'	 get locale 'de-DE'
----------------- 
Locale : en-US 
set locale 'en-US'	 get locale 'en-US'
----------------- 
Locale : es-419 
set locale 'es-419'	 get locale 'es-419'
----------------- 
Locale : de-CH-x-phonebk 
set locale 'de-CH-x-phonebk'	 get locale 'de-CH-x-phonebk'
----------------- 
Locale : az-Arab-x-AZE-derbend 
set locale 'az-Arab-x-AZE-derbend'	 get locale 'az-Arab-x-AZE-derbend'
----------------- 
Locale : zh-min 
set locale 'zh-min'	 get locale 'zh-min'
----------------- 
Locale : zh-min-nan-Hant-CN 
set locale 'zh-min-nan-Hant-CN'	 get locale 'zh-min-nan-Hant-CN'
----------------- 
Locale : x-whatever 
set locale 'x-whatever'	 get locale 'x-whatever'
----------------- 
Locale : qaa-Qaaa-QM-x-southern 
set locale 'qaa-Qaaa-QM-x-southern'	 get locale 'qaa-Qaaa-QM-x-southern'
----------------- 
Locale : sr-Latn-QM 
set locale 'sr-Latn-QM'	 get locale 'sr-Latn-QM'
----------------- 
Locale : sr-Qaaa-CS 
set locale 'sr-Qaaa-CS'	 get locale 'sr-Qaaa-CS'
----------------- 
Locale : en-US-u-islamCal 
set locale 'en-US-u-islamCal'	 get locale 'en-US-u-islamCal'
----------------- 
Locale : zh-CN-a-myExt-x-private 
set locale 'zh-CN-a-myExt-x-private'	 get locale 'zh-CN-a-myExt-x-private'
----------------- 
Locale : en-a-myExt-b-another 
set locale 'en-a-myExt-b-another'	 get locale 'en-a-myExt-b-another'
----------------- 
Locale : de-419-DE 
set locale 'de-419-DE'	 get locale 'de-419-DE'
----------------- 
Locale : a-DE 
set locale 'a-DE'	 get locale 'a-DE'
----------------- 
Locale : ar-a-aaa-b-bbb-a-ccc 
set locale 'ar-a-aaa-b-bbb-a-ccc'	 get locale 'ar-a-aaa-b-bbb-a-ccc'
-----------------
