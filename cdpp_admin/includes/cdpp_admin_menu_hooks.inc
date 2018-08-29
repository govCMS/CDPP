<?php

/**
 * @file
 * Contains cdpp admin menu hooks.
 */

/**
 * Implements hook_menu_alter().
 *
 * Add custom administrative pages.
 */
function cdpp_admin_menu_alter(&$items) {
  $items['admin/config/cdpp'] = array(
    'title' => 'CDPP',
    'description' => 'Settings for the CDPP site.',
    'position' => 'right',
    // After workflow.
    'weight' => 6,
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array('access administration pages'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
  );
  $items['admin/config/cdpp/media-subscribe'] = array(
    'title' => 'Media Subscribe',
    'description' => 'Media Subscription.',
    'page callback' => '_cdpp_admin_menu_callback_caller',
    'access arguments' => array('edit any webform content'),
    // Can't use 'file' _menu_router_build(), it calls
    // drupal_get_path('module',..) so the theme path would not be right,
    // we simulate it instead.
    'page arguments' => array(
      'admin_includes/cdpp_admin.admin.media_subscribe.inc',
      'cdpp_admin_media_subscribe_landing',
      FALSE,
      'media_subscribe',
    ),
  );
  $items['admin/config/cdpp/media-subscribe/settings'] = array(
    'title' => 'Settings',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
  );
  $items['admin/config/cdpp/media-subscribe/email-template'] = array(
    'title' => 'Media Subscribe Email Template',
    'description' => 'Media Subscription Template for testing.',
    'page callback' => '_cdpp_admin_menu_callback_caller',
    'access arguments' => array('edit any webform content'),
    'page arguments' => array(
      'admin_includes/cdpp_admin.admin.media_subscribe.inc',
      'cdpp_admin_media_subscribe_email_template_test',
      TRUE,
      'media_subscribe',
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );
}

/**
 * A simple function to simulate inlcuding a admin callback's file.
 *
 * @param string $admin_path
 *   The path, no leading slash, to the file relative to the theme folder.
 * @param string $function_name
 *   The name of the function to deliver the content.
 * @param bool $print
 *   Optionally, print the content directly, useful for templates.
 *
 * @return mixed|null
 *   The content to return if print is false.
 */
function _cdpp_admin_menu_callback_caller($admin_path, $function_name, $print = FALSE) {
  include_once drupal_get_path('theme', 'cdpp_admin') . "/{$admin_path}";
  $args = func_get_args();
  // Remove $admin_path.
  array_shift($args);
  // Remove $function_name.
  array_shift($args);
  // Remove $print.
  if (count($args)) {
    array_shift($args);
  }
  $content = 'Unable to load Content';
  if (function_exists($function_name)) {
    $content = call_user_func_array($function_name, $args);
  }
  if ($print) {
    print $content;
  }
  else {
    return $content;
  }
}
