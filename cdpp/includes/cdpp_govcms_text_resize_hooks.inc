<?php

/**
 * @file
 * GovCMS text resize related hooks, specific to cdpp.
 */

/**
 * Implements hook_preprocess_text_resize().
 *
 * Text resize is provided by govCMS module.
 */
function cdpp_preprocess_text_resize(&$variables) {
  $variables['message'] = 'Set text size to';
}

/**
 * Called by cdpp_js_alter().
 *
 * We provide our own improved version of the js file for text resize,
 * remove the module version then.
 */
function _cdpp_js_alter__text_resize(&$javascript) {
  $resize_js_path = 'profiles/govcms/modules/custom/govcms_text_resize/js/text-resize.js';
  if (isset($javascript[$resize_js_path])) {
    unset($javascript[$resize_js_path]);
  }
}
