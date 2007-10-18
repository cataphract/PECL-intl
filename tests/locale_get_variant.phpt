--TEST--
locale_get_variant()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try getting the variant for different locales
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
        $scr = ut_loc_get_variant( $locale);
        $res_str .= "$locale:  variant='$scr'";
        $res_str .= "\n";
    }

    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
uk-ua_CALIFORNIA@currency=;currency=GRN:  variant='CALIFORNIA'
:  variant=''
root:  variant=''
uk@currency=EURO:  variant=''
Hindi:  variant=''
de:  variant=''
fr:  variant=''
ja:  variant=''
i-enochian:  variant=''
zh-Hant:  variant=''
zh-Hans:  variant=''
sr-Cyrl:  variant=''
sr-Latn:  variant=''
zh-Hans-CN:  variant=''
sr-Latn-CS:  variant=''
sl-rozaj:  variant=''
sl-nedis:  variant=''
de-CH-1901:  variant='1901'
sl-IT-nedis:  variant='NEDIS'
sl-Latn-IT-nedis:  variant='NEDIS'
de-DE:  variant=''
en-US:  variant=''
es-419:  variant=''
de-CH-x-phonebk:  variant='X_PHONEBK'
az-Arab-x-AZE-derbend:  variant='AZE_DERBEND'
zh-min:  variant=''
zh-min-nan-Hant-CN:  variant='NAN_HANT_CN'
x-whatever:  variant=''
qaa-Qaaa-QM-x-southern:  variant='X_SOUTHERN'
sr-Latn-QM:  variant=''
sr-Qaaa-CS:  variant=''
en-US-u-islamCal:  variant='U_ISLAMCAL'
zh-CN-a-myExt-x-private:  variant='A_MYEXT_X_PRIVATE'
en-a-myExt-b-another:  variant='MYEXT_B_ANOTHER'
de-419-DE:  variant='DE'
a-DE:  variant=''
ar-a-aaa-b-bbb-a-ccc:  variant='AAA_B_BBB_A_CCC'

