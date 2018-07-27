<?php

/**
 * @file
 * Miscellaneous theme related hook functions used within the CDPP site.
 */

/**
 * Implements hook_theme().
 */
function cdpp_theme() {
  $theme = array(
    // We need to change the return text on media subscribe webform confirmation
    // screen in the tpl file.
    // We have to use a custom theme as webform's hook_theme() uses the pattern
    // parameter which stops the specificity normally used, from working.
    // See https://www.drupal.org/node/1891220
    // and https://www.drupal.org/node/837594.
    'webform_confirmation__media_subscribe' => array(
      'variables' => array('node' => NULL, 'sid' => NULL),
      'template' => 'templates/webforms/webform-confirmation--media-subscribe',
    ),
  );
  return $theme;
}

/**
 * Call a theme's specific a preprocess or process functions.
 *
 * E.g. call hook_preprocess_MYHOOK().
 * Not hook_preprocess().
 *
 * @param array $variables
 *   The variables.
 * @param string $hook
 *   The hook to call.
 * @param string $function_type
 *   The function type, 'preprocess functions' or 'process functions'.
 */
function _cdpp_call_process_theme_functions(array &$variables, $hook, $function_type) {
  // Adapt a section of theme()'s code.
  $hooks = theme_get_registry(FALSE);
  $hook_length = drupal_strlen($hook);
  if (isset($hooks[$hook][$function_type])) {
    $process_functions = $hooks[$hook][$function_type];
    foreach ($process_functions as $process_function) {
      // If it ends in the specific hook - don't call our selves again.
      if (substr($process_function, -$hook_length) === $hook) {
        if (function_exists($process_function)) {
          // We don't want a poorly behaved process function changing $hook.
          $hook_clone = $hook;
          $process_function($variables, $hook_clone);
        }
      }
    }
  }
}

/**
 * Implements hook_preprocess_webform_confirmation__media_subscribe().
 *
 * Sets up variables we expect.
 */
function cdpp_preprocess_webform_confirmation__media_subscribe(&$variables, $hook) {
  // Basically calls hook_preprocess_webform_confirmation().
  $hook_to_call = 'webform_confirmation';
  _cdpp_call_process_theme_functions($variables, $hook_to_call, 'preprocess functions');
}

/**
 * Implements hook_process_webform_confirmation__media_subscribe().
 *
 * There isn't a process hook but there might be in the future.
 */
function cdpp_process_webform_confirmation__media_subscribe(&$variables, $hook) {
  // Basically calls hook_process_webform_confirmation().
  $hook_to_call = 'webform_confirmation';
  _cdpp_call_process_theme_functions($variables, $hook_to_call, 'process functions');
}
