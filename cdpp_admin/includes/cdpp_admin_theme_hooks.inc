<?php

/**
 * @file
 * Miscellaneous theme related hook functions used within the CDPP admin site.
 */

/**
 * Implements hook_theme().
 */
function cdpp_admin_theme() {
  $theme = array(
    'cdpp_admin_media_subscribe_email' => array(
      'variables' => array(
        'logo_url' => '',
        'title' => '',
        'intro_text_array' => array(
          'value' => '',
          'format' => 'richext',
        ),
        // Intro and closing text is computed from intro_text_array
        // via check_markup().
        'intro_text' => '',
        'closing_text_array' => array(
          'value' => '',
          'format' => 'richext',
        ),
        'closing_text' => '',
        'list' => array(
          array(
            'title' => '',
            'date_time' => '',
            'timezone' => 'Australia/Melbourne',
            'timezone_db' => 'Australia/Melbourne',
            'link' => '<front>',
          ),
        ),
        'unsub_email_address' => '',
        'unsub_subject' => '',
        // Encoded is computed from above.
        'unsub_subject_encoded' => '',
      ),
      'template' => 'templates/emails/cdpp-admin-media-subscribe-email',
    ),
  );
  return $theme;
}
