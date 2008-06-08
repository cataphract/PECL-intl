--TEST--
msgfmt creation failures
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

function err($fmt) {
	if(!$fmt) {
		echo var_export(intl_get_error_message(), true)."\n";
	}
}

function crt($t, $l, $s) {
	switch(true) {
		case $t == "O":
			return new MessageFormatter($l, $s);
			break;
		case $t == "C":
			return MessageFormatter::create($l, $s);
			break;
		case $t == "P":
			return msgfmt_create($l, $s);
			break;
	}
}

$args = array(
	array("whatever", "{0,whatever}"),
	array(array(), array()),
	array("en", "{0,choice}"),
	array("fr", "{0,"),
	array("en_US", b"\xD0"),
);

$fmt = new MessageFormatter();
err($fmt); 
$fmt = msgfmt_create();
err($fmt); 
$fmt = MessageFormatter::create();
err($fmt); 
$fmt = new MessageFormatter('en');
err($fmt); 
$fmt = msgfmt_create('en');
err($fmt); 
$fmt = MessageFormatter::create('en');
err($fmt); 

foreach($args as $arg) {
	$fmt = crt("O", $arg[0], $arg[1]);
	err($fmt);
	$fmt = crt("C", $arg[0], $arg[1]);
	err($fmt);
	$fmt = crt("P", $arg[0], $arg[1]);
	err($fmt);
}

?>
--EXPECTF--
Warning: MessageFormatter::__construct() expects exactly 2 parameters, 0 given in %s on line %d
'__construct: unable to parse input params: U_ILLEGAL_ARGUMENT_ERROR'

Warning: msgfmt_create() expects exactly 2 parameters, 0 given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'

Warning: MessageFormatter::create() expects exactly 2 parameters, 0 given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'

Warning: MessageFormatter::__construct() expects exactly 2 parameters, 1 given in %s on line %d
'__construct: unable to parse input params: U_ILLEGAL_ARGUMENT_ERROR'

Warning: msgfmt_create() expects exactly 2 parameters, 1 given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'

Warning: MessageFormatter::create() expects exactly 2 parameters, 1 given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'
'__construct: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'
'msgfmt_create: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'
'msgfmt_create: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'

Warning: MessageFormatter::__construct() expects parameter 1 to be binary string, array given in %s on line %d
'__construct: unable to parse input params: U_ILLEGAL_ARGUMENT_ERROR'

Warning: MessageFormatter::create() expects parameter 1 to be binary string, array given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'

Warning: msgfmt_create() expects parameter 1 to be binary string, array given in %s on line %d
'msgfmt_create: unable to parse input parameters: U_ILLEGAL_ARGUMENT_ERROR'
'__construct: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'
'msgfmt_create: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'
'msgfmt_create: message formatter creation failed: U_ILLEGAL_ARGUMENT_ERROR'
'__construct: message formatter creation failed: U_UNMATCHED_BRACES'
'msgfmt_create: message formatter creation failed: U_UNMATCHED_BRACES'
'msgfmt_create: message formatter creation failed: U_UNMATCHED_BRACES'

Warning: Could not convert binary string to Unicode string (converter UTF-8 failed on bytes (0xD0) at offset 0) in %s on line %d

Warning: Could not convert binary string to Unicode string (converter UTF-8 failed on bytes (0xD0) at offset 0) in %s on line %d

Warning: Could not convert binary string to Unicode string (converter UTF-8 failed on bytes (0xD0) at offset 0) in %s on line %d
