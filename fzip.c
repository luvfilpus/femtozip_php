/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 321634 2012-01-01 13:15:04Z felipe $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_fzip.h"
#include "femtozip.h"

/* If you declare any globals in php_fzip.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(fzip)


/* True global resources - no need for thread safety here */
static int le_fzip;

ZEND_BEGIN_ARG_INFO_EX(arginfo_fz_compress, 0, 0, 4)
        ZEND_ARG_INFO(0, model)
        ZEND_ARG_INFO(0, source)
        ZEND_ARG_INFO(0, source_len)
        ZEND_ARG_INFO(0, dest)
        ZEND_ARG_INFO(0, dest_capacity)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fz_decompress, 0, 0, 4)
		ZEND_ARG_INFO(0, model)
		ZEND_ARG_INFO(0, source)
		ZEND_ARG_INFO(0, source_len)
		ZEND_ARG_INFO(0, dest)
		ZEND_ARG_INFO(0, dest_capacity)
ZEND_END_ARG_INFO()


/* {{{ fzip_functions[]
 *
 * Every user visible function must have an entry in fzip_functions[].
 */
const zend_function_entry fzip_functions[] = {
	PHP_FE(confirm_fzip_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(fz_compress,	NULL)
	PHP_FE(fz_decompress,	NULL)
	PHP_FE_END	/* Must be the last line in fzip_functions[] */
};
/* }}} */

/* {{{ fzip_module_entry
 */
zend_module_entry fzip_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"fzip",
	fzip_functions,
	PHP_MINIT(fzip),
	PHP_MSHUTDOWN(fzip),
	NULL,		/* Replace with NULL if there's nothing to do at request start */
	NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(fzip),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FZIP
ZEND_GET_MODULE(fzip)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("fzip.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_fzip_globals, fzip_globals)
    STD_PHP_INI_ENTRY("fzip.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_fzip_globals, fzip_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_fzip_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_fzip_init_globals(zend_fzip_globals *fzip_globals)
{
	fzip_globals->global_value = 0;
	fzip_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(fzip)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(fzip)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(fzip)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(fzip)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(fzip)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "fzip support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_fzip_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_fzip_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "fzip", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto int fz_compress()
   *model, const char *source, int source_len, char *dest, int dest_capacity); */
PHP_FUNCTION(fz_compress)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	const char *get_doc_callback(int doc_index, int *doc_len, void *user_data) {
	    char **docs = (char **)(user_data);
	    *doc_len = strlen(docs[doc_index]);
	    return docs[doc_index];
	}

	void release_doc_callback(const char *buf, void *user_data) {
	}
	int dest_capacity;
	int fzip_in = strlen(FZIP_G(source));

	/* add 600 like bz2 should be safe? */

	int fzip_out = fzip_in + 600;
	int fzip_next_out = dest_capacity = emalloc(fzip_out + 1);

	int num_docs = sizeof(FZIP_G(source)) / sizeof(FZIP_G(source[0]));

	void *model = fz_build_model(num_docs, get_doc_callback, release_doc_callback, FZIP_G(source));

	fz_release_model(model);

	int compressed_len = fz_compress(model, FZIP_G(source), strlen(FZIP_G(source)), FZIP_G(dest), dest_capacity TSRMLS_DC);

}
/* }}} */

/* {{{ proto int fz_decompress()
   *model, const char *source, int source_len, char *dest, int dest_capacity); */
PHP_FUNCTION(fz_decompress)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	const char *get_doc_callback(int doc_index, int *doc_len, void *user_data) {
	    char **docs = (char **)(user_data);
	    *doc_len = strlen(docs[doc_index]);
	    return docs[doc_index];
	}

	void release_doc_callback(const char *buf, void *user_data) {
	}

	int dest_capacity;
	int fzip_in = strlen(FZIP_G(source));

	/* in most cases bz2 offers at least 2:1 compression, so we use 3:1 as our base to be safe */

	int fzip_out = fzip_in * 3;
	int fzip_next_out = dest_capacity = emalloc(fzip_out + 1);

	int num_docs = sizeof(FZIP_G(source)) / sizeof(FZIP_G(source[0]));

	void *model = fz_build_model(num_docs, get_doc_callback, release_doc_callback, FZIP_G(source));

	fz_release_model(model);


	int decompressed_len = fz_decompress(model, FZIP_G(source), strlen(FZIP_G(source)), FZIP_G(dest), dest_capacity TSRMLS_DC);

}
/* }}} */

/*
char compressed[1024];
const char *test_doc = "check out http://www.facebook.com/someone";
int compressed_len = fz_compress(model, test_doc, strlen(test_doc), compressed, sizeof(compressed));
if (compressed_len < 0) {
    return 1;
}

char decompressed[1024];
int decompressed_len = fz_decompress(model, compressed, compressed_len, decompressed, sizeof(decompressed));
if (decompressed_len < 0 ||
    (unsigned int)decompressed_len != strlen(test_doc) ||
    memcmp(decompressed, test_doc, decompressed_len) != 0)
{
    return 2;
}
*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
