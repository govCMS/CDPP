<?php

/**
 * @file
 * Contains cdpp admin menu hooks callbacks for media_subscription.
 */

/**
 * Placeholder page for media settings.
 *
 * Currently these are on the media subscription node edit page.
 *
 * @param string $media_sub_name
 *   The media subscription node's machine name.
 *
 * @return string
 *   The content to be returned.
 */
function cdpp_admin_media_subscribe_landing($media_sub_name) {
  $node = defaultcontent_get_node($media_sub_name);
  return t('Placeholder page for further media subscription configuration. To configure the settings, edit the !node.',
    array(
      '!node' => ($node ? l(t('media subscription node'), "node/{$node->nid}/edit") : '<Node not found>'),
    )
  );
}

/**
 * Test callback for template testing.
 */
function cdpp_admin_media_subscribe_email_template_test() {
  $content = array(
    '#theme' => 'cdpp_admin_media_subscribe_email',
    '#title' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_subject'),
    '#intro_text_array' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_intro_text'),
    '#closing_text_array' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_closing_text'),
    '#unsub_subject' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub_subject'),
    '#list' => array(
      array(
        'title' => 'My example article',
        'date_time' => '2017-11-22 00:00:00',
        'timezone' => 'Australia/Melbourne',
        'timezone_db' => 'Australia/Melbourne',
        'link' => '<front>',
      ),
    ),
    '#unsub_email_address' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub'),
  );
  // We might dynamically insert CSS, lets do that in our test menu link too.
  // Hook is based off mimemail_prepare_message().
  $body = render($content);
  $key = 'cdpp_admin_media_subscribe';
  $module = 'webform';
  $hook = array(
    'mimemail_message__' . $key,
    'mimemail_message__' . $module . '__' . $key,
  );
  $mail_content = array(
    '#theme' => $hook,
    '#module' => $module,
    '#key' => $key,
    '#recipient' => 'to@example.com',
    '#subject' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_subject'),
    '#body' => $body,
  );
  return render($mail_content);
}
