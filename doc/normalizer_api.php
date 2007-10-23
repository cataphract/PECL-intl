<?php
#############################################################################
# Object-oriented API
#############################################################################

/**
 * Normalizer class.
 *
 * Normalizer provides access to Unicode normalization of strings. This class consists
 * only of static methods. The iterator interface to normalizer is rarely used, so is
 * not provided here.
 *
 * Example:
 * <code>
 *
 * </code>
 *
 * @see http://www.icu-project.org/apiref/icu4c/unorm_8h.html
 * @see http://www.icu-project.org/apiref/icu4c/classNormalizer.html
 *
 */
class Normalizer {
#############################################################################
# Common constants.
#############################################################################

	/**
     * Valid normalization form values.
     *
     * @see Normalizer::normalize()
     * @see Normalizer::isNormalize()
     * @see normalizer_normalize()
     * @see normalizer_is_normalized()
     */
   const FORM_C  = 'UNORM_FORM_C';
   const FORM_D  = 'UNORM_FORM_D';
   const FORM_KC = 'UNORM_FORM_KC';
   const FORM_KD = 'UNORM_FORM_KD';
   const NONE    = 'UNORM_NONE';

	/**
     * Valid normalizer options.
     *
     * @see Normalizer::normalize()
     * @see Normalizer::isNormalize()
     * @see normalizer_normalize()
     * @see normalizer_is_normalized()
     */
   const OPTION_DEFAULT = 'OPTION_DEFAULT';

   /**
    * Normalizes the input provided and returns the normalized string
    * @param  string     $input    The input string to normalize
    * @param  [integer]  $form     One of the normalization forms
    * @param  [array]    $options  Normalization options
    * @return string               The normalized string or false if an error occurred.
    */
   public static function normalize($input, $form = self::FORM_C, $options = null) {}

   /**
    * Checks if the provided string is already in the specified normalization form.
    * @param  string    $input    The input string to normalize
    * @param  [integer] $form     One of the normalization forms
    * @param  [array]   $options  Normalization options
    * @return boolean             True if normalized, false otherwise
    */
   public static function isNormalized($input, $form = self::FORM_C, $options = null) {}

}

#############################################################################
# Procedural API
#############################################################################

/**
 * Normalizes the input provided and returns the normalized string
 * @param  string    $input    The input string to normalize
 * @param  [integer] $form     One of the normalization forms
 * @param  [array]   $options  Normalization options
 * @return string              The normalized string or false if an error occurred.
 */
function normalizer_normalize($input, $form = self::FORM_C, $options = null) {}

/**
 * Checks if the provided string is already in the specified normalization form.
 * @param  string  $input    The input string to normalize
 * @param  integer $form     One of the normalization forms
 * @param  array   $options  Normalization options
 * @return boolean           True if normalized, false otherwise
 */
function normalizer_is_normalized($input, $form = self::FORM_C, $options = null) {}


?>

