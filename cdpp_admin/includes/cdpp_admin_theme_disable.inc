<?php

/**
 * @file
 * Contains cdpp admin theme page hooks.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function cdpp_admin_form_system_themes_admin_form_alter(&$form, &$form_state, $form_id) {
  $form['#submit'][] = __FUNCTION__ . '__submit';
}

/**
 * Remove our admin variables if admin theme is changed.
 */
function cdpp_admin_form_system_themes_admin_form_alter__submit($form, &$form_state) {
  if (isset($form_state['values']['admin_theme'])) {
    $admin_theme = $form_state['values']['admin_theme'];
    if ($admin_theme !== 'cdpp_admin') {
      // Admin theme is disabled, lets remove any variables we have set up.
      $form_element_names = array(
        'cdpp_admin_theme_media_subscribe_enabled',
        'cdpp_admin_theme_media_subscribe_types',
        'cdpp_admin_theme_media_subscribe_email_extra_cc',
        'cdpp_admin_theme_media_subscribe_email_unsub',
        'cdpp_admin_theme_media_subscribe_email_unsub_subject',
        'cdpp_admin_theme_media_subscribe_email_closing_text',
        'cdpp_admin_theme_media_subscribe_email_subject',
        'cdpp_admin_theme_media_subscribe_email_intro_text',
        // Nid tracking.
        'cdpp_admin_theme_media_subscribe_unpub_nids',
      );
      foreach ($form_element_names as $form_element_name) {
        variable_del($form_element_name);
      }
    }
  }
}
