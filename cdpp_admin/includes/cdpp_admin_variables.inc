<?php

/**
 * @file
 * Provides variable helper functions.
 */

/**
 * Variable get wrapper function.
 */
function cdpp_admin_variable_get($variable_name) {
  $result = NULL;
  switch ($variable_name) {
    case 'cdpp_admin_theme_media_subscribe_email_intro_text':
      $result = variable_get(
        $variable_name,
        array(
          'value' => '<p>The following article has been published:</p>',
          'format' => 'rich_text',
        )
      );
      break;

    case 'cdpp_admin_theme_media_subscribe_email_closing_text':
      $result = variable_get(
        $variable_name,
        array(
          'value' => '<p>Regards, <br>Communications and Media Team <br>Commonwealth Director of Public Prosecutions <br>Phone: 02 6206 5708 Email:<a href="mailto:communications@cdpp.gov.au">communications@cdpp.gov.au <br></a></p><p style="font-size: 11px;">The Media Team operates Monday to Friday during business hours.</p>',
          'format' => 'rich_text',
        )
      );
      break;

    case 'cdpp_admin_theme_media_subscribe_types':
      $result = variable_get($variable_name, array(
        'news_article',
        'media_release',
        'case_report',
      ));
      break;

    case 'cdpp_admin_theme_media_subscribe_email_subject':
      $result = variable_get($variable_name, 'New CDPP article published.');
      break;

    case 'cdpp_admin_theme_media_subscribe_email_extra_cc':
      $result = variable_get($variable_name, 'Comms.Mailbox@cdpp.gov.au');
      break;

    case 'cdpp_admin_theme_media_subscribe_email_unsub':
      $result = variable_get($variable_name, 'Comms.Mailbox@cdpp.gov.au');
      break;

    case 'cdpp_admin_theme_media_subscribe_email_unsub_subject':
      $result = variable_get($variable_name, 'Unsubscribe: CDPP Public site media Subscription');
      break;

    case 'cdpp_admin_theme_media_subscribe_enabled':
      $result = variable_get($variable_name, 0);
      break;

  }
  return $result;
}
