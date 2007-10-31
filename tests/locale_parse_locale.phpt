--TEST--
locale_parse_locale()
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
    $res_str = '';

    $locales = array(
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
/*Tags that use extensions (examples ONLY: extensions MUST be defined
   by revision or update to this document or by RFC): */
        'en-US-u-islamCal',
        'zh-CN-a-myExt-x-private',
        'en-a-myExt-b-another',
//Some Invalid Tags:
        'de-419-DE',
        'a-DE',
        'ar-a-aaa-b-bbb-a-ccc'
    );


    $res_str = '';

    foreach( $locales as $locale )
    {
        $arr = ut_loc_locale_parse( $locale);
        $res_str .= "---------------------\n";
        $res_str .= "$locale: \n";
        if( $arr){
            foreach( $arr as $key => $value){
                    $res_str .= "$key : '$value' , ";
            }
        }
        else{
            $res_str .= "No values found from Locale parsing.";
        }
        $res_str .= "\n";
    }

    $res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
---------------------
uk-ua_CALIFORNIA@currency=;currency=GRN: 
language : 'uk' , region : 'UA' , variant : 'CALIFORNIA' , 
---------------------
: 
No values found from Locale parsing.
---------------------
root: 
language : 'root' , 
---------------------
uk@currency=EURO: 
language : 'uk' , 
---------------------
Hindi: 
language : 'hindi' , 
---------------------
de: 
language : 'de' , 
---------------------
fr: 
language : 'fr' , 
---------------------
ja: 
language : 'ja' , 
---------------------
i-enochian: 
language : 'i-enochian' , 
---------------------
zh-Hant: 
language : 'zh' , script : 'Hant' , 
---------------------
zh-Hans: 
language : 'zh' , script : 'Hans' , 
---------------------
sr-Cyrl: 
language : 'sr' , script : 'Cyrl' , 
---------------------
sr-Latn: 
language : 'sr' , script : 'Latn' , 
---------------------
zh-Hans-CN: 
language : 'zh' , script : 'Hans' , region : 'CN' , 
---------------------
sr-Latn-CS: 
language : 'sr' , script : 'Latn' , region : 'CS' , 
---------------------
sl-rozaj: 
language : 'sl' , region : 'ROZAJ' , 
---------------------
sl-nedis: 
language : 'sl' , region : 'NEDIS' , 
---------------------
de-CH-1901: 
language : 'de' , region : 'CH' , variant : '1901' , 
---------------------
sl-IT-nedis: 
language : 'sl' , region : 'IT' , variant : 'NEDIS' , 
---------------------
sl-Latn-IT-nedis: 
language : 'sl' , script : 'Latn' , region : 'IT' , variant : 'NEDIS' , 
---------------------
de-DE: 
language : 'de' , region : 'DE' , 
---------------------
en-US: 
language : 'en' , region : 'US' , 
---------------------
es-419: 
language : 'es' , region : '419' , 
---------------------
de-CH-x-phonebk: 
language : 'de' , region : 'CH' , variant : 'X_PHONEBK' , 
---------------------
az-Arab-x-AZE-derbend: 
language : 'az' , script : 'Arab' , region : 'X' , variant : 'AZE_DERBEND' , 
---------------------
zh-min: 
language : 'zh' , region : 'MIN' , 
---------------------
zh-min-nan-Hant-CN: 
language : 'zh' , region : 'MIN' , variant : 'NAN_HANT_CN' , 
---------------------
x-whatever: 
language : 'x-whatever' , 
---------------------
qaa-Qaaa-QM-x-southern: 
language : 'qaa' , script : 'Qaaa' , region : 'QM' , variant : 'X_SOUTHERN' , 
---------------------
sr-Latn-QM: 
language : 'sr' , script : 'Latn' , region : 'QM' , 
---------------------
sr-Qaaa-CS: 
language : 'sr' , script : 'Qaaa' , region : 'CS' , 
---------------------
en-US-u-islamCal: 
language : 'en' , region : 'US' , variant : 'U_ISLAMCAL' , 
---------------------
zh-CN-a-myExt-x-private: 
language : 'zh' , region : 'CN' , variant : 'A_MYEXT_X_PRIVATE' , 
---------------------
en-a-myExt-b-another: 
language : 'en' , region : 'A' , variant : 'MYEXT_B_ANOTHER' , 
---------------------
de-419-DE: 
language : 'de' , region : '419' , variant : 'DE' , 
---------------------
a-DE: 
language : 'a' , region : 'DE' , 
---------------------
ar-a-aaa-b-bbb-a-ccc: 
language : 'ar' , region : 'A' , variant : 'AAA_B_BBB_A_CCC' ,
