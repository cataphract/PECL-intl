--TEST--
numfmt_format()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Format a number using misc locales/patterns.
 */


function ut_main()
{
    $styles = array(
        NumberFormatter::PATTERN_DECIMAL => '##.#####################',
        NumberFormatter::DECIMAL => '',
        NumberFormatter::CURRENCY => '',
        NumberFormatter::PERCENT => '',
        NumberFormatter::SCIENTIFIC => '',
        NumberFormatter::SPELLOUT => '@@@@@@@',
        NumberFormatter::ORDINAL => '',
        NumberFormatter::DURATION => '',
        NumberFormatter::PATTERN_RULEBASED => '#####.###'
        // NumberFormatter::IGNORE => ''
    );

    $locales = array(
        'en_US',
        'ru_UA',
        'de',
        'en_UK'
    );

    $str_res = '';
    $number = 1234567.891234567890000;

    foreach( $locales as $locale )
    {
        $str_res .= "\n  Locale is: $locale\n";
        foreach( $styles as $style => $pattern )
        {
            $fmt = ut_nfmt_create( $locale, $style, $pattern );
            $str_res .= dump( ut_nfmt_format( $fmt, $number ) ) . "\n";
        }
    }
    return $str_res;
}

include_once( 'ut_common.php' );

// Run the test
ut_run();

?>
--EXPECT--
Locale is: en_US
'1234567.89123457'
'1,234,567.891'
'$1,234,567.89'
'123,456,789%'
'1.23456789123457E6'
false
'1,234,568th'
'342:56:08'
'#####.###'

  Locale is: ru_UA
'1234567,89123457'
'1 234 567,891'
'1 234 567,89 грн.'
'123 456 789%'
'1,23456789123457E6'
false
'1 234 568'
'1 234 568'
'#####.###'

  Locale is: de
'1234567,89123457'
'1.234.567,891'
'¤ 1.234.567,89'
'123.456.789%'
'1,23456789123457E6'
false
'1.234.568'
'1.234.568'
'#####.###'

  Locale is: en_UK
'1234567.89123457'
'1,234,567.891'
'¤1,234,567.89'
'123,456,789%'
'1.23456789123457E6'
false
'1,234,568th'
'342:56:08'
'#####.###'
