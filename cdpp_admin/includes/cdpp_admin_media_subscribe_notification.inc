<?php

/**
 * @file
 * Contains cdpp admin theme media subscribe notification logic.
 */

/**
 * Called by cdpp_admin_form_node_form_alter().
 */
function cdpp_admin_form_node_form_alter__func__media_subscribe(&$form, &$form_state, $form_id) {
  // Only run if auto send is enabled.
  if (!cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_enabled')) {
    return;
  }
  // Do we have node to use?
  if (!isset($form_state['node']->type)) {
    return;
  }
  // Check we are on the right content type to continue.
  $is_correct_type = in_array(
    $form_state['node']->type,
    cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_types'),
    TRUE
  );
  if (!$is_correct_type) {
    return;
  }
  // Set "new" flag so can tell if node a is new after saving.
  $form['#submit'][] = __FUNCTION__ . '__new_flag_submit';
  // Run after node_form_submit().
  $form['actions']['submit']['#submit'][] = __FUNCTION__ . '__submit';
}

/**
 * Implements hook_preprocess_cdpp_admin_media_subscribe_email().
 */
function cdpp_admin_preprocess_cdpp_admin_media_subscribe_email(&$variables) {
  if (!isset($variables['logo_url']) || !$variables['logo_url']) {
    $variables['logo_url'] = theme_get_setting('logo', variable_get('theme_default', 'cdpp'));
  }
  // Check plain/markup the title and intro/closing text.
  $variables['title'] = check_plain($variables['title']);
  $variables['intro_text'] = check_markup($variables['intro_text_array']['value'], $variables['intro_text_array']['format']);
  $variables['closing_text'] = check_markup($variables['closing_text_array']['value'], $variables['closing_text_array']['format']);
  // Unsub subject.
  $variables['unsub_subject'] = check_plain($variables['unsub_subject']);
  $variables['unsub_subject_encoded'] = rawurlencode($variables['unsub_subject']);
  // Create and santize node variables.
  foreach ($variables['list'] as $list_key => $list_value) {
    $variables['list'][$list_key]['title'] = check_plain($list_value['title']);
    $variables['list'][$list_key]['link'] = url($list_value['link'], array('absolute' => TRUE));
    if (!isset($list_value['date_time']) || !$list_value['date_time']) {
      $fallback_datetime = new DateTime();
      // Format 2017-11-22 08:34:05.
      $list_value['date_time'] = $fallback_datetime->format('d-m-Y H:i:s');
    }
    $date_object = new DateObject($list_value['date_time'], new DateTimeZone($list_value['timezone_db']));
    // Convert from the database time zone to the drupal time zone.
    $date_object->setTimezone(new DateTimeZone($list_value['timezone']));
    // Date format: 25/06/2017.
    $variables['list'][$list_key]['date'] = $date_object->format('d/m/Y');
    // Time format: 3:00PM.
    $variables['list'][$list_key]['time'] = $date_object->format('g:iA');
    // Timezone format: AEST.
    $variables['list'][$list_key]['timezone_short'] = $date_object->format('T');
  }
}

/**
 * Set our 'is_new' flag so we can detect later on if it's a new node.
 */
function cdpp_admin_form_node_form_alter__func__media_subscribe__new_flag_submit($form, &$form_state) {
  if (isset($form_state['node'])) {
    $form_state['node']->cdpp_admin_media_subscribe_is_new = FALSE;
    if (!isset($form_state['node']->nid)) {
      $form_state['node']->cdpp_admin_media_subscribe_is_new = TRUE;
    }
  }
}

/**
 * Process CDPP media subscribe when a news article is published.
 */
function cdpp_admin_form_node_form_alter__func__media_subscribe__submit($form, &$form_state) {
  // No nid and specific variable means the content wasn't saved
  // or something else happened.
  if (!isset($form_state['node']->nid) || !isset($form_state['node']->cdpp_admin_media_subscribe_is_new)) {
    return;
  }
  $node = $form_state['node'];
  // Workbench moderation also sets status to 1 if published, so we are safe.
  // @see workbench_moderation_node_presave().
  $is_published = isset($node->status) ? $node->status : FALSE;
  $send_email = FALSE;
  $unpub_to_pub = FALSE;
  $is_new_node = FALSE;
  $unpublished_nodes = variable_get('cdpp_admin_theme_media_subscribe_unpub_nids', array());
  // Is this article moving from unpublihsed to published?
  if (!$node->cdpp_admin_media_subscribe_is_new) {
    if ($is_published) {
      // If we have we seen this node before,
      // move from unpublished list to publihsed.
      if (isset($unpublished_nodes[$node->nid])) {
        unset($unpublished_nodes[$node->nid]);
        variable_set('cdpp_admin_theme_media_subscribe_unpub_nids', $unpublished_nodes);
        $unpub_to_pub = TRUE;
      }
    }
  }
  // A new node.
  else {
    $is_new_node = TRUE;
    if (!$is_published) {
      // Record unpublished nodes for publication later on.
      // No need to re-set if already there.
      if (!isset($unpublished_nodes[$node->nid])) {
        $unpublished_nodes[$node->nid] = $node->nid;
        variable_set('cdpp_admin_theme_media_subscribe_unpub_nids', $unpublished_nodes);
      }
    }
  }
  if ($is_published && ($is_new_node || $unpub_to_pub)) {
    $send_email = TRUE;
  }
  if ($send_email) {
    $media_sub_node = defaultcontent_get_node('media_subscribe');
    if (!$media_sub_node) {
      // Log that we can't send the emails.
      watchdog(
        'cdpp_admin',
        'News article publication notification cannot be sent, media subscribe webform cannot be loaded. Check the machine name is "media_subscribe"',
        array(),
        WATCHDOG_ERROR
      );
      drupal_set_message(t('News article publication notification cannot be sent, media subscribe webform cannot be loaded. Check the machine name is "media_subscribe"'), 'error');
    }
    cdpp_admin_form_node_form_alter__func__media_subscribe__submit_send_email($form_state['node'], $media_sub_node);
  }
}

/**
 * Process CDPP media subscribe when a news article is published.
 */
function cdpp_admin_form_node_form_alter__func__media_subscribe__get_email_list($media_sub_node) {
  $emails = array();
  module_load_include(
    'inc',
    'webform',
    'includes/webform.report'
  );
  $submissions = array();
  if (isset($media_sub_node->nid) && isset($media_sub_node->webform['components'])) {
    $submissions = webform_get_submissions(array('nid' => $media_sub_node->nid), NULL, 0);
  }
  else {
    return array();
  }
  // Find the component id of the email key.
  $email_component_id = '';
  foreach ($media_sub_node->webform['components'] as $id => $component) {
    if ($component['form_key'] === 'email') {
      $email_component_id = $id;
      break;
    }
  }
  if (!$email_component_id) {
    return array();
  }
  // Build email list now.
  foreach ($submissions as $submission) {
    if (isset($submission->data[$email_component_id][0])) {
      $emails[] = $submission->data[$email_component_id][0];
    }
  }
  return $emails;
}

/**
 * Send media subscribe email.
 */
function cdpp_admin_form_node_form_alter__func__media_subscribe__submit_send_email($node, $media_sub_node) {
  global $user;
  $intro_text_array = cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_intro_text');
  $closing_text_array = cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_closing_text');
  $extra_bccs = cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_extra_cc');
  $email_list = cdpp_admin_form_node_form_alter__func__media_subscribe__get_email_list($media_sub_node);
  // For privacy reasons, the BCC list might get leaked
  // if a poor emil encryption system is used,
  // so lets send each email indivdually.
  $module = 'webform';
  $key = 'cdpp_admin_media_subscribe';
  // To could also be 'undisclosed-recipients:;' + BCC but heed warning above.
  // From will be overriden anyway.
  $from = 'noreply@cdpp.gov.au';
  $language = $user->uid ? user_preferred_language($user) : language_default();
  $date_field = field_get_items('node', $node, 'field_date');
  $fallback_datetime = new DateTime();
  $message = array(
    '#theme' => 'cdpp_admin_media_subscribe_email',
    '#title' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_subject'),
    '#intro_text_array' => $intro_text_array,
    '#closing_text_array' => $closing_text_array,
    '#unsub_subject' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub_subject'),
    '#list' => array(
      array(
        'title' => $node->title,
        'date_time' => (isset($date_field[0]['value']) ? $date_field[0]['value'] : $fallback_datetime->format('d-m-Y H:i:s')),
        'timezone' => (isset($date_field[0]['timezone']) ? $date_field[0]['timezone'] : 'Australia/Melbourne'),
        'timezone_db' => (isset($date_field[0]['timezone_db']) ? $date_field[0]['timezone_db'] : 'Australia/Melbourne'),
        'link' => "node/{$node->nid}",
      ),
    ),
    '#unsub_email_address' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_unsub'),
  );
  $params = array(
    'message' => render($message),
    'subject' => cdpp_admin_variable_get('cdpp_admin_theme_media_subscribe_email_subject'),
    'headers' => array(),
  );
  // Log that we going to try to send the emails.
  watchdog(
    'cdpp_admin',
    'News article publication notification process triggered for "!news" with subscribers from "!webform"',
    array(
      '!news' => l($node->title, "node/$node->nid",
        array('alias' => TRUE)
      ),
      '!webform' => l($media_sub_node->title, "node/$media_sub_node->nid/webform-results/table"),
    ),
    WATCHDOG_INFO
  );
  drupal_set_message(
    t(
      'News article publication notification process triggered for "!news" with subscribers from "!webform"',
      array(
        '!news' => l($node->title, "node/$node->nid"),
        '!webform' => l($media_sub_node->title, "node/$media_sub_node->nid/webform-results/table"),
      )
    ),
    'status'
  );
  // First send email to extra cc list.
  $to = $extra_bccs;
  // Add [CC] to subject line.
  $subject = $params['subject'];
  $params['subject'] .= ' [CC]';
  drupal_mail($module, $key, $to, $language, $params, $from);
  // Then email everyone else.
  $params['subject'] = $subject;
  foreach ($email_list as $email) {
    $to = $email;
    drupal_mail($module, $key, $to, $language, $params, $from);
  }
}
