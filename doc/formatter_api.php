<?php

/**
 * Number formatter class - locale-dependent number formatting/parsing.
 *
 * This class represents the ICU number formatting functionality. It allows to display
 * number according to the localized format or given pattern or set of rules, and to
 * parse strings into numbers according to the above patterns.
 *
 * Example:
 * <code>
 * $value = 1234567;
 * $formatter = new NumberFormatter("de_DE", NumberFormatter::DECIMAL);
 * echo $formatter->format($value);
 * </code>
 *
 * @see http://www.icu-project.org/apiref/icu4c/unum_8h.html
 * @see http://www.icu-project.org/apiref/icu4c/classNumberFormat.html
 *
 * The class would also contain all the constants listed in the following enums:
 * UNumberFormatStyle, UNumberFormatRoundingMode, UNumberFormatPadPosition,
 * UNumberFormatAttribute, UNumberFormatTextAttribute, UNumberFormatSymbol.
 */
class NumberFormatter {
	/*
	 * These constants define formatter/parser argument type - integer, floating point or currency.
	 */
	const TYPE_INT32 = 1;
	const TYPE_INT64 = 2;
	const TYPE_DOUBLE = 3;
	const TYPE_CURRENCY = 4;

	/**
	 * Create a number formatter
	 *
	 * Creates a number formatter from locale and pattern. This formatter would be used to
	 * format or parse numbers.
	 *
	 * @param integer $style     Style of the formatting, one of the UNumberFormatStyle constants
	 * @param string $locale     Locale in which the number would be formatted
	 * @param [string] $pattern  Pattern string in case chose style requires pattern
	 * @return NumberFormatter
	 */
	public function __construct($locale, $style, $pattern = null) {}
	
	/**
	 * Create a number formatter
	 *
	 * Creates a number formatter from locale and pattern. This formatter would be used to
	 * format or parse numbers.
	 *
	 * @param integer  $style    Style of the formatting, one of the UNumberFormatStyle constants
	 * @param string   $locale   Locale in which the number would be formatted
	 * @param [string] $pattern  Pattern string in case chose style requires pattern
	 * @return NumberFormatter
	 * @see __construct
	 * @see formatter_create
	 */
	public static function create($locale, $style, $pattern = null) {}

	/**
	 * Format a number according to current formatting rules.
	 *
	 * If the type is not specified, the type is derived from the $number parameter. I.e., if it's
	 * integer then INT32 would be chosen on 32-bit, INT64 on 64-bit, if it's double, DOUBLE would be
	 * chosen. It is possible to format 64-bit number on 32-bit machine by passing it as double and using
	 * TYPE_INT64.
	 * When formatting currency, default formatter's currency name is used.
	 *
	 * @param integer|double $number Number to format
	 * @param [integer]      $type   Type of the formatting - one of TYPE constants. If not specified, default for the type.
	 * @return string formatted number
	 */
	public function format($number, $type = 0) {}

	/**
	 * Parse a number according to current formatting rules.
	 *
	 * This parser requires type though we migh make it use INT32/INT64 by default. When parsing currency,
	 * default currency definitions are used.
	 *
	 * @param string                 $string String to parse
	 * @param integer                $type type of the formatting - one of TYPE constants.
	 * @return integer|double|false  Parsed number, false if parsing failed
	 */
	public function parse($string, $type) {}

	/**
	 * Format number as currency.
	 *
	 * Uses user-defined currency string.
	 *
	 * @param double $number    Number to format
	 * @param string $currency  Currency name to use in format
	 */
	public function formatCurrency($number, $currency) {}

	/**
	 * Parse currency string
	 *
	 * This parser would use parseCurrency API string to parse currency string. The format is defined by the
	 * formatter, returns both number and currency name.
	 *
	 * @param string $string    String to parse
	 * @param string $currency  Parameter to return parsed currency name
	 * @return double currency number
	 */
	public function parseCurrency($string, &$currency) {}

	/**
	 * Set formatter attribute.
	 *
	 * This function is used to set any of the formatter attributes. Example:
	 *
	 * $formatter->setAttribute(NumberFormat::FORMAT_WIDTH, 10);
 	 *
	 * @param integer        $attr  One of UNumberFormatAttribute constants
	 * @param integer|double $value Value of the attribute
	 * @return false if attribute is unknown or can not be set, true otherwise
	 */
	public function setAttribute($attr, $value) {}
	/**
	 * Set formatter attribute.
	 *
	 * This function is used to set any of the formatter attributes. Example:
	 *
	 * $formatter->setTextAttribute(NumberFormat::POSITIVE_PREFIX, "+");
 	 *
	 * @param integer $attr  One of UNumberFormatTextAttribute constants
	 * @param string  $value Value of the attribute
	 * @return false if attribute is unknown or can not be set, true otherwise
	 */
	public function setTextAttribute($attr, $value) {}
	/**
	 * Set formatting symbol.
	 *
	 * Example:
	 *
	 * $formatter->setSymbol(NumberFormat::EXPONENTIAL_SYMBOL, "E");
	 *
	 * @param integer|array $attr  One of UNumberFormatSymbol constants or array of symbols, indexed by
	 * 								these constants
	 * @param string        $value Value of the symbol
	 */
	public function setSymbol($attr, $value) {}
	/**
	 * Set pattern used by the formatter
	 *
	 * Valid only if the formatter is using pattern and is not rule-based.
	 * @see http://www.icu-project.org/apiref/icu4c/classDecimalFormat.html
	 * Localized patterns are not currently supported.
	 *
	 * @param string $pattern  The pattern to be used.
	 * @return boolean         false if formatter pattern could not be set, true otherwise
	 */
	public function setPattern($pattern) {}
	/**
	 * Get value of the formatter attribute
	 *
	 * @param integer $attr One of UNumberFormatAttribute constants
	 * @return integer|double value of the attribute or false if the value can not be obtained
	 */
	public function getAttribute($attr) {}
	/**
	 * Get value of the formatter attribute
	 *
	 * @param integer $attr One of UNumberFormatTextAttribute constants
	 * @return string value of the attribute or false if the value can not be obtained
	 */
	public function getTextAttribute($attr) {}
	/**
	 * Get value of the formatter symbol
	 *
	 * @param integer $attr One of UNumberFormatSymbol constants specifying the symbol
	 * @return string|false The symbol value, or false if the value can not be obtained
	 */
	public function getSymbol($attr) {}
	/**
	 * Get pattern used by the formatter.
	 *
	 * Gets current state of the formatter as a pattern.
	 * Localized patterns are not currently supported.
	 *
	 * Valid only if the formatter is UNUM_PATTERN_DECIMAL
	 * @return string|false The pattern used by the formatter or false if formatter is of a type
	 *                      that does not support patterns.
	 */
	public function getPattern() {}
	/**
	 * Get the locale for which the formatter was created.
	 *
	 * @param [integer] $type One of  ULocDataLocaleType  values
	 * @return string locale name
	 */
	public function getLocale($type = 0) {}
	/**
	 * Get the error code from last operation
	 *
	 * Returns error code from the last number formatting operation.
	 *
	 * @return integer the error code, one of UErrorCode values. Initial value is U_ZERO_ERROR.
	 */
	public function getErrorCode() {}
	/**
	 * Get the error text from the last operation.
	 *
	 * @return string Description of the last occured error.
	 */
	public public function getErrorMessage() {}

}

/** Now the same as procedural API */

/**
 * Create a number formatter
 *
 * Creates a number formatter from locale and pattern. This formatter would be used to
 * format or parse numbers.
 *
 * @param string   $locale   Locale in which the number would be formatted
 * @param integer  $style    Style of the formatting, one of the UNumberFormatStyle constants
 * @param [string] $pattern  Pattern string in case chose style requires pattern
 * @return Numberformatter resource NumberFormatter
 */
function formatter_create($locale, $style, $pattern = null) {}
/**
 * Format a number according to current formatting rules.
 *
 * If the type is not specified, the type is derived from the $number parameter. I.e., if it's
 * integer then INT32 would be chosen on 32-bit, INT64 on 64-bit, if it's double, DOUBLE would be
 * chosen. It is possible to format 64-bit number on 32-bit machine by passing it as double and using
 * TYPE_INT64.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer|double  $number	 Number to format
 * @param [integer]       $type		 Type of the formatting - one of TYPE constants. If not specified, default for the type.
 * @return string formatted number
 */
function formatter_format($formatter, $number, $type = null) {}
/**
 * Parse a number according to current formatting rules.
 *
 * This parser requires type though we migh make it use INT32/INT64 by default. When parsing currency,
 * default currency definitions are used.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param string                 $string String to parse
 * @param integer                $type   Type of the formatting - one of TYPE constants.
 * @return integer|double|false  Parsed number, false if parsing failed
 */
function formatter_parse($formatter, $string, $type) {}
/**
 * Format number as currency.
 *
 * Uses user-defined currency string.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param double          $number    Number to format
 * @param string $currency  Currency name to use in format
 */
function formatter_format_currency($formatter, $number, $currency) {}
/**
 * Parse currency string
 *
 * This parser would use parseCurrency API string to parse currency string. The format is defined by the
 * formatter, returns both number and currency name.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param string          $string    String to parse
 * @param string          $currency  Parameter to return parsed currency name
 * @return double currency number
 */
function formatter_parse_currency($formatter, $string, &$currency) {}
/**
 * Set formatter attribute.
 *
 * This function is used to set any of the formatter attributes. Example:
 *
 * formatter_format_set_attribute($formatter, NumberFormat::FORMAT_WIDTH, 10);
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer         $attr      One of UNumberFormatAttribute constants
 * @param integer|double  $value     Value of the attribute
 * @return false if attribute is unknown or can not be set, true otherwise
 */
function formatter_set_attribute($formatter, $attribute, $value) {}
/**
 * Set formatter attribute.
 *
 * This function is used to set any of the formatter attributes. Example:
 *
 * formatter_format_set_text_attribute($formatter, NumberFormat::POSITIVE_PREFIX, "+");
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer         $attr      One of UNumberFormatTextAttribute constants
 * @param string          $value     Value of the attribute
 * @return false if attribute is unknown or can not be set, true otherwise
 */
function formatter_set_text_attribute($formatter, $attribute, $value) {}
/**
 * Set formatting symbol.
 *
 * Example:
 *
 * $formatter->setSymbol(NumberFormat::EXPONENTIAL_SYMBOL, "E");
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer|array   $attr      One of UNumberFormatSymbol constants or array of symbols, 
 *                                   indexed by these constants
 * @param string $value Value of the symbol
 */
function formatter_set_symbol($formatter, $attribute, $value) {}
/**
 * Set pattern used by the formatter
 *
 * Valid only if the formatter is using pattern and is not rule-based.
 * @see http://www.icu-project.org/apiref/icu4c/classDecimalFormat.html
 * Localized patterns are not currently supported.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param string          $pattern   The pattern to be used.
 * @return boolean false if formatter pattern could not be set, true otherwise
 */
function formatter_set_pattern($formatter, $pattern) {}
/**
 * Get value of the formatter attribute
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer         $attribute One of UNumberFormatAttribute constants
 * @return integer|double value of the attribute or false if the value can not be obtained
 */
function formatter_get_attribute($formatter, $attribute) {}
/**
 * Get value of the formatter attribute
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer         $attribute One of UNumberFormatTextAttribute constants
 * @return string value of the attribute or false if the value can not be obtained
 */
function formatter_get_text_attribute($formatter, $attribute) {}
/**
 * Get value of the formatter symbol
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param integer         $attribute One of UNumberFormatSymbol constants specifying the symbol
 * @return string|false The symbol value, or false if the value can not be obtained
 */
function formatter_get_symbol($formatter, $attribute) {}
/**
 * Get pattern used by the formatter.
 *
 * Gets current state of the formatter as a pattern.
 * Localized patterns are not currently supported.
 *
 * Valid only if the formatter is   UNUM_PATTERN_DECIMAL
 * @param NumberFormatter $formatter The formatter resource
 * @return string|false The pattern used by the formatter or false if formatter is of a type
 *                      that does not support patterns.
 */
function formatter_get_pattern($formatter) {}
/**
 * Get the locale for which the formatter was created.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @param [integer]       $type      One of ULocDataLocaleType values
 * @return string locale name
 */
function formatter_get_locale($formatter, $type = 0) {}
/**
 * Get the error code from last operation
 *
 * Returns error code from the last number formatting operation.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @return integer the error code, one of UErrorCode values. Initial value is U_ZERO_ERROR.
 */
function formatter_get_error_code($formatter) {}
/**
 * Get the error text from the last operation.
 *
 * @param NumberFormatter $formatter The formatter resource
 * @return string Description of the last occured error.
 */
function formatter_get_error_message($formatter) {}

?>
