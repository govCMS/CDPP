<?php

/**
 * @file
 * Contains cdpp webform hooks.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function cdpp_form_webform_client_form_alter(&$form, &$form_state, $form_id) {
  // Optionally run webform machine_name specific form alter hook.
  // E.g. hook__form_webform_client_form_alter__subscribe().
  if (isset($form['#node']->machine_name)) {
    $machine_name = $form['#node']->machine_name;
    $function = __FUNCTION__ . '__' . $machine_name;

    if (function_exists($function)) {
      $function($form, $form_state, $form_id);
    }
  }
}

/**
 * Add ajax for 'media_subscribe' webform.
 */
function cdpp_form_webform_client_form_alter__media_subscribe(&$form, &$form_state, $form_id) {
  cdpp_theme_form_webform_client_form_alter__add_ajax($form, $form_state, $form_id);
}

/**
 * Helper function - called by hook_form_FORM_ID_alter().
 *
 * Add ajax to Subscribe webform so users stay on same page after submit.
 * Based off Webform Ajax module.
 */
function cdpp_theme_form_webform_client_form_alter__add_ajax(&$form, &$form_state, $form_id) {
  // Get the intro text from the node so we can attach it to webform block.
  $field_introduction = field_view_field('node', $form['#node'], 'field_introduction', 'full');
  if (!empty($field_introduction)) {
    // Show before existing webform controls.
    $field_introduction['#weight'] = '-1';
    // Add the intro text field on now.
    $form['submitted']['field_introduction'] = $field_introduction;
  }
  // Add wrapper for Ajax to work.
  // The wrapper ID will have to follow the webform all the time, to be unique,
  // and allow AJAX commands to perform correctly.
  // We have many ways to get it back depending on the situation.
  $form['webform_ajax_wrapper_id'] = array(
    '#type' => 'hidden',
  );
  if (isset($form_state['values']['webform_ajax_wrapper_id'])) {
    $form['webform_ajax_wrapper_id']['#value'] = $form_state['values']['webform_ajax_wrapper_id'];
  }
  elseif (isset($form['#node']->webform['webform_ajax_wrapper_id'])) {
    $form['webform_ajax_wrapper_id']['#value'] = $form['#node']->webform['webform_ajax_wrapper_id'];
  }
  else {
    // At last, generate a unique ID.
    $form['webform_ajax_wrapper_id']['#value'] = drupal_html_id('webform-ajax-wrapper-' . $form['#node']->nid);
  }

  $form['#prefix'] = '<div id="' . $form['webform_ajax_wrapper_id']['#value'] . '">' . (isset($form['#prefix']) ? $form['#prefix'] : '');
  $form['#suffix'] = (isset($form['#suffix']) ? $form['#suffix'] : '') . '</div>';

  foreach (array('previous', 'next', 'submit', 'draft') as $button) {
    if (isset($form['actions'][$button])) {
      $form['actions'][$button]['#ajax'] = array(
        'callback' => 'cdpp_theme_webform_client_form__ajax_handler',
        'wrapper' => $form['webform_ajax_wrapper_id']['#value'],
        'progress' => array(
          'message' => '',
          'type' => 'throbber',
        ),
      );
      if (in_array($button, array('next', 'submit'))) {
        $form['actions'][$button]['#ajax']['event'] = 'click';
      }
      // Workaround for Drupal core bug http://drupal.org/node/1548976.
      // As long as buttons HTML id are causing problems, and it has bad
      // consequences on pages where Webform AJAX is active, we'll force
      // custom ids on AJAXed Webform's buttons.
      $submit_id = drupal_html_id('edit-webform-ajax-' . $button . '-' . $form['#node']->nid);
      $form['actions'][$button]['#attributes']['id'] = $submit_id;
      $form['actions'][$button]['#id'] = $submit_id;
    }
  }
}

/**
 * AJAX callback for Subscribe webform Prev/Next page and Submit buttons.
 *
 * Returns the new computed webform, unless it has been completed.
 */
function cdpp_theme_webform_client_form__ajax_handler($form, &$form_state) {
  $output = array();
  // If user completed his submission, determine what to do.
  if (!empty($form_state['webform_completed']) && empty($form_state['save_draft'])) {
    $output = array(
      '#type' => 'markup',
      '#markup' => theme(
        array(
          'webform_confirmation__' . $form['#node']->machine_name,
          'webform_confirmation_' . $form['#node']->nid,
          'webform_confirmation',
        ),
        array(
          'node' => $form['#node'],
          'sid' => $form_state['values']['details']['sid'],
        )
      ),
    );
    // Unset confirmation message previously set with drupal_set_message()
    // by Webform module, as we displayed it themed.
    // Get messages without clearing queue.
    $status_messages = drupal_get_messages('status', FALSE);
    // If there are status messages, we may want to clear one.
    if (isset($status_messages['status'])) {
      // Load node and submission to pass to webform_replace_tokens()
      $node = node_load($form_state['values']['details']['nid']);
      $submission = webform_get_submission($form_state['values']['details']['nid'], $form_state['values']['details']['sid']);
      // This is the message we want to erase.
      $confirmation = webform_replace_tokens($form['#node']->webform['confirmation'], $node, $submission, NULL, $form['#node']->webform['confirmation_format']);
      $index = array_search($confirmation, $status_messages['status']);
      // If message found, then remove it from the list,
      // clear the messages queue, then reset all messages
      // (except the one we removed).
      if ($index !== FALSE) {
        unset($status_messages['status'][$index]);
        drupal_get_messages('status');
        foreach ($status_messages['status'] as $message) {
          drupal_set_message($message);
        }
      }
    }
  }
  // Else, we're just switching page, or saving draft.
  else {
    $output = $form;
  }
  return $output;
}

/**
 * Called by cdpp_preprocess_node().
 *
 * Run node machine name specific code.
 */
function cdpp_preprocess_node__webform(&$variables) {
  if (isset($variables['node']->machine_name)) {
    $machine_name = $variables['node']->machine_name;
    $function_name = __FUNCTION__ . "__func__{$machine_name}";
    if (function_exists($function_name)) {
      $function_name($variables);
    }
  }
}

/**
 * Called by cdpp_preprocess_node__webform().
 *
 * Don't print the intro text twice, it is already inside the webform node.
 */
function cdpp_preprocess_node__webform__func__media_subscribe(&$variables) {
  if (isset($variables['content']['field_introduction'])) {
    hide($variables['content']['field_introduction']);
  }
}
