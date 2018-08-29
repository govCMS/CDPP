<?php

/**
 * @file
 * Contains cdpp admin media subscribe webform admin form.
 */

/**
 * Called by hook_form_webform_node_form_alter().
 */
function cdpp_admin_form_webform_node_form_alter__func__admin__media_subscribe(&$form, &$form_state, $form_id) {
  $form['media_subscribe_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media Subscribe settings'),
    '#group' => 'additional_settings',
  );
  $form['media_subscribe_settings']['description'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#markup' => t('The media subscribe notification functionality works as follows, providing it is enabled:'),
  );
  $form['media_subscribe_settings']['list'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#items' => array(
      'Notification process is triggered at publish time (save form + published is ticked)',
      'If the media/news/case report page is brand new, all subscribers will receive a notification email.',
      'If the media/news/case report page is brand new but not published, a notification will be sent when it published (first time only)',
      'If the media/news/case report page was already published prior to this functionality, a notification will not be sent (no email for amendments to existing news)',
    ),
  );
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_enabled'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Disabled.'), 1 => t('Enabled.')),
    '#title' => t('Send media subscriber notifications automatically.'),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_enabled'),
    '#description' => t('Default is disabled.'),
    '#required' => TRUE,
  );
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_extra_cc'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra cc email address'),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_extra_cc'),
    '#description' => t("Optionally add an extra email address to be emailed to, the default is !default_to_email. Multiple address can be separated by a space and semi-colon e.g. user1@example.com; user2@example.com",
      array('!default_to_email' => 'Comms.Mailbox@cdpp.gov.au')
    ),
  );
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_unsub'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscribe email address'),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub'),
    '#description' => t("The Unsubscribe address, the default is !default_to_email.",
      array('!default_to_email' => 'Comms.Mailbox@cdpp.gov.au')
    ),
    '#required' => TRUE,
  );
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_unsub_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscribe email subject'),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub_subject'),
    '#description' => t("The Unsubscribe subject when user wants to remove themselves from the list, the default is !default.",
      array('!default' => "'Unsubscribe: CDPP Public site media Subscription'")
    ),
    '#required' => TRUE,
  );
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_subject'),
    '#description' => t("The email subject to use then sending the email, the default is 'New CDPP article published.'"),
    '#required' => TRUE,
  );
  $media_sub_intro_text = cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_intro_text');
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_intro_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Email Intro text'),
    '#default_value' => isset($media_sub_intro_text['value']) ? $media_sub_intro_text['value'] : '',
    '#format' => isset($media_sub_intro_text['format']) ? $media_sub_intro_text['format'] : '',
    '#description' => t('The intro text content included in the email.'),
    '#required' => TRUE,
  );
  $media_sub_closing_text = cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_closing_text');
  $form['media_subscribe_settings']['cdpp_admin_theme_media_subscribe_email_closing_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Email Closing text'),
    '#default_value' => isset($media_sub_closing_text['value']) ? $media_sub_closing_text['value'] : '',
    '#format' => isset($media_sub_closing_text['format']) ? $media_sub_closing_text['format'] : '',
    '#description' => t('The closing text content included in the email.'),
    '#required' => TRUE,
  );

  $form['media_subscribe_settings']['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['media_subscribe_settings']['types']['cdpp_admin_theme_media_subscribe_types'] = array(
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_types'),
    '#title' => t('The content types to watch, default is media, news and case report types.'),
  );
  $form['#submit'][] = __FUNCTION__ . '__submit';
  $form['#validate'][] = __FUNCTION__ . '__validate';
}

/**
 * Provide email validation.
 */
function cdpp_admin_form_webform_node_form_alter__func__admin__media_subscribe__validate($form, &$form_state) {
  $form_email_element_names = array(
    'cdpp_admin_theme_media_subscribe_email_extra_cc' => array(
      'optional' => TRUE,
    ),
    'cdpp_admin_theme_media_subscribe_email_unsub' => array(
      'optional' => FALSE,
    ),
  );
  foreach ($form_email_element_names as $form_element_name => $options) {
    if (isset($form_state['values'][$form_element_name])) {
      $fail = FALSE;
      // Check if optional before validation.
      if ($options['optional']) {
        if ($form_state['values'][$form_element_name] === '') {
          continue;
        }
      }
      $emails = explode('; ', $form_state['values'][$form_element_name]);
      foreach ($emails as $email) {
        if (!valid_email_address($email)) {
          $fail = TRUE;
          break;
        }
      }
      if ($fail) {
        form_error(
          $form['media_subscribe_settings']["{$form_element_name}"],
          t('An invalid email address was detected! Ckeck media subscribe settings.')
        );
      }
    }
  }
}

/**
 * Process CDPP media subscribe admin webform.
 */
function cdpp_admin_form_webform_node_form_alter__func__admin__media_subscribe__submit($form, &$form_state) {
  $form_element_names = array(
    'cdpp_admin_theme_media_subscribe_enabled',
    'cdpp_admin_theme_media_subscribe_types',
    'cdpp_admin_theme_media_subscribe_email_extra_cc',
    'cdpp_admin_theme_media_subscribe_email_unsub',
    'cdpp_admin_theme_media_subscribe_email_unsub_subject',
    'cdpp_admin_theme_media_subscribe_email_subject',
    'cdpp_admin_theme_media_subscribe_email_intro_text',
    'cdpp_admin_theme_media_subscribe_email_closing_text',
  );
  foreach ($form_element_names as $form_element_name) {
    if (isset($form_state['values'][$form_element_name])) {
      // If value has changed, set the new value.
      if (cdpp_admin_variable_get($form_element_name) !== $form_state['values'][$form_element_name]) {
        variable_set($form_element_name, $form_state['values'][$form_element_name]);
      }
    }
  }
}
